package iot

import (
	"context"
	"errors"
	"fmt"
	"os/exec"
	"time"

	"gobot.io/x/gobot"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

const (
	DefaultDataPublishRate     = 15
	DefaultConfigSubscribeRate = 1
	DefaultStatusPublishRate   = 5 // rate of every 12 hours
)

type IOTServerService interface {
	PublishSensorDataSnapshotHandler(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
	IntializeIOTFunctions(ctx context.Context)
}

type iot struct {
	sensors                     sensors.SensorsService
	googleiot                   googleiot.GoogleIOTService
	logger                      config.LoggerService
	DataPublishRate             time.Duration
	DataPublishTickerReferences *time.Ticker
}

// NewIOTService returns a instance of the iot service
func NewIOTService(logger config.LoggerService, ss sensors.SensorsService, gs googleiot.GoogleIOTService) IOTServerService {
	i := &iot{
		sensors:   ss,
		googleiot: gs,
		logger:    logger,
	}
	return i
}

func (i *iot) PublishSensorDataSnapshotHandler(ctx context.Context) error {

	i.logger.StdOut("Fetching sensor data\n")
	data, err := i.sensors.FetchSensorData(ctx)
	if err != nil {
		e := fmt.Sprintf("failed to fetch the sensor data: %v\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	if err := i.googleiot.PublishSensorData(ctx, data); err != nil {
		e := fmt.Sprintf("failed to publish the sensor data to cloud iot: %v\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	i.logger.StdOut("Successfully published the sensor data\n")
	return nil
}

func (i *iot) PublishDeviceStatus(ctx context.Context) error {

	i.logger.StdOut("fetching cpu temperature\n")
	var cpuTemp float32
	err := i.sensors.FetchCPUTemp(&cpuTemp)
	if err != nil {
		e := fmt.Sprintf("ERROR to get cpu temp: %v\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	data := &googleiot.DeviceStatus{
		CpuTemp: cpuTemp,
	}

	if err := i.googleiot.PublishDeviceState(ctx, data); err != nil {
		e := fmt.Sprintf("failed to publish the device status to cloud iot: %s\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	i.logger.StdOut("successfully published the device status\n")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.logger.StdOut("subscribing to google iot config changes\n")

	msg, err := i.googleiot.SubsribeToConfigChanges(ctx)
	if err != nil {
		e := fmt.Sprintf("failed to subscribe to the config changes: %s\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	err = i.handleIOTCOnfigMessage(msg, ctx)
	if err != nil {
		e := fmt.Sprintf("Failed to handle config update with %s\n", err.Error())
		i.logger.StdErr(e)
		return errors.New(e)
	}

	i.logger.StdOut("subscribed to google iot config changes published the device status\n")
	return nil
}

func (i *iot) IntializeIOTFunctions(ctx context.Context) {
	i.logger.StdOut("initialize the base iot functions\n")
	i.handleDataPublishRateUpdate(DefaultDataPublishRate, ctx)
	i.setupDefaultTimedFunctions(ctx)
}

func (i *iot) handleIOTCOnfigMessage(msg *googleiot.ConfigMessage, ctx context.Context) error {
	go func() {
		i.logger.StdOut("handling data publish rate update\n")
		i.handleDataPublishRateUpdate(msg.DataPublishRate, ctx)
	}()

	i.logger.StdOut("handling power status updates\n")
	return i.handlePowerStatus(msg.PowerState)
}

func (i *iot) setupDefaultTimedFunctions(ctx context.Context) {
	gobot.Every(DefaultStatusPublishRate*time.Minute, func() {
		err := i.PublishDeviceStatus(ctx)
		if err != nil {
			i.logger.StdErr("Failed to publish the device status: %v\n", err.Error())
			return
		}
	})

	gobot.Every(DefaultConfigSubscribeRate*time.Minute, func() {
		err := i.SubscribeToIOTCoreConfig(ctx)
		if err != nil {
			i.logger.StdErr("Failed to subscribe to the iot config: %v\n", err.Error())
			return
		}
	})
}

func (i *iot) handleDataPublishRateUpdate(rate time.Duration, ctx context.Context) {
	if rate == i.DataPublishRate {
		i.logger.StdErr("Data publish rate is the same as current\n")
		return
	}

	if rate == 0 {
		i.logger.StdErr("Data publish rate is 0, do not act\n")
		return
	}

	if i.DataPublishTickerReferences != nil {
		i.DataPublishTickerReferences.Stop()
	}

	t := gobot.Every(rate*time.Minute, func() {
		err := i.PublishSensorDataSnapshotHandler(ctx)
		if err != nil {
			i.logger.StdErr("Failed to publish the sensor data: %v\n", err.Error())
		}
	})
	i.DataPublishTickerReferences = t
}

func (i *iot) handlePowerStatus(p googleiot.PowerState) error {
	var err error
	if p != "" {
		if p == googleiot.Off {
			i.logger.StdOut("turn off device\n")
			err = i.turnOffDevice()
		}

		if p == googleiot.Reboot {
			i.logger.StdOut("reboot device\n")
			err = i.rebootTheDevice()
		}

		if p == googleiot.On {
			i.logger.StdOut("leave device on\n")
		}
	} else {
		i.logger.StdOut("Message does not have a valid power status\n")
	}
	return err
}

func (i *iot) turnOffDevice() error {
	cmd := exec.Command("sudo", "/sbin/shutdown", "-P", "now")
	_, err := cmd.Output()
	return err
}

func (i *iot) rebootTheDevice() error {
	cmd := exec.Command("sudo", "/sbin/reboot")
	_, err := cmd.Output()
	return err
}
