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

type Service interface {
	PublishSensorDataSnapshotHandler(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
	IntializeIOTFunctions(ctx context.Context)
}

type iot struct {
	sensors                     sensors.Service
	googleiot                   googleiot.Service
	logger                      *config.Logger
	DataPublishRate             time.Duration
	DataPublishTickerReferences *time.Ticker
}

// NewIOTService returns a instance of the iot service
func NewIOTService(logger *config.Logger, ss sensors.Service, gs googleiot.Service) Service {
	i := &iot{
		sensors:   ss,
		googleiot: gs,
		logger:    logger,
	}
	return i
}

func (i *iot) PublishSensorDataSnapshotHandler(ctx context.Context) error {

	i.logger.StdOut.Println("Fetching sensor data")
	data, err := i.sensors.FetchSensorData(ctx)
	if err != nil {
		e := fmt.Sprintf("failed to fetch the sensor data: %+s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	if err := i.googleiot.PublishSensorData(ctx, data); err != nil {
		e := fmt.Sprintf("failed to publish the sensor data to cloud iot: %+s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	i.logger.StdOut.Println("Successfully published the sensor data")
	return nil
}

func (i *iot) PublishDeviceStatus(ctx context.Context) error {

	i.logger.StdOut.Println("fetching cpu temperature")
	var cpuTemp float32
	err := i.sensors.FetchCPUTemp(&cpuTemp)
	if err != nil {
		e := fmt.Sprintf("ERROR to get cpu temp: %+s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	data := &googleiot.DeviceStatus{
		CpuTemp: cpuTemp,
	}

	if err := i.googleiot.PublishDeviceState(ctx, data); err != nil {
		e := fmt.Sprintf("failed to publish the device status to cloud iot: %s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	i.logger.StdOut.Println("successfully published the device status")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.logger.StdOut.Println("subscribing to google iot config changes")

	msg, err := i.googleiot.SubsribeToConfigChanges(ctx)
	if err != nil {
		e := fmt.Sprintf("failed to subscribe to the config changes: %s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	err = i.handleIOTCOnfigMessage(msg, ctx)
	if err != nil {
		e := fmt.Sprintf("Failed to handle config update with %s", err.Error())
		i.logger.StdErr.Println(e)
		return errors.New(e)
	}

	i.logger.StdOut.Println("subscribed to google iot config changes published the device status")
	return nil
}

func (i *iot) IntializeIOTFunctions(ctx context.Context) {
	i.logger.StdOut.Println("initialize the base iot functions")
	i.handleDataPublishRateUpdate(DefaultDataPublishRate, ctx)
	i.setupDefaultTimedFunctions(ctx)
}

func (i *iot) handleIOTCOnfigMessage(msg *googleiot.ConfigMessage, ctx context.Context) error {
	go func() {
		i.logger.StdOut.Println("handling data publish rate update")
		i.handleDataPublishRateUpdate(msg.DataPublishRate, ctx)
	}()

	i.logger.StdOut.Println("handling power status updates")
	return i.handlePowerStatus(msg.PowerState)
}

func (i *iot) setupDefaultTimedFunctions(ctx context.Context) {
	gobot.Every(DefaultStatusPublishRate*time.Minute, func() {
		err := i.PublishDeviceStatus(ctx)
		if err != nil {
			i.logger.StdErr.Printf("Failed to publish the device status: %+s", err.Error())
			return
		}
	})

	gobot.Every(DefaultConfigSubscribeRate*time.Minute, func() {
		err := i.SubscribeToIOTCoreConfig(ctx)
		if err != nil {
			i.logger.StdErr.Printf("Failed to subscribe to the iot config: %+s", err.Error())
			return
		}
	})
}

func (i *iot) handleDataPublishRateUpdate(rate time.Duration, ctx context.Context) {
	if rate == i.DataPublishRate {
		i.logger.StdErr.Println("Data publish rate is the same as current")
		return
	}

	if rate == 0 {
		i.logger.StdErr.Println("Data publish rate is 0, do not act")
		return
	}

	if i.DataPublishTickerReferences != nil {
		i.DataPublishTickerReferences.Stop()
	}

	t := gobot.Every(rate*time.Minute, func() {
		err := i.PublishSensorDataSnapshotHandler(ctx)
		if err != nil {
			i.logger.StdErr.Printf("Failed to publish the sensor data: %+s", err.Error())
		}
	})
	i.DataPublishTickerReferences = t
}

func (i *iot) handlePowerStatus(p googleiot.PowerState) error {
	var err error
	if p != "" {
		if p == googleiot.Off {
			i.logger.StdOut.Println("turn off device")
			err = i.turnOffDevice()
		}

		if p == googleiot.Reboot {
			i.logger.StdOut.Println("reboot device")
			err = i.rebootTheDevice()
		}

		if p == googleiot.On {
			i.logger.StdOut.Println("leave device on")
		}
	} else {
		i.logger.StdOut.Println("Message does not have a valid power status")
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
