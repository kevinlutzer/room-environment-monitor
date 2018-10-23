package iot

import (
	"context"
	"errors"
	"fmt"
	"log"
	"os/exec"
	"time"

	"gobot.io/x/gobot"

	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	"golang.org/x/sync/errgroup"
)

const (
	DefaultDataPublishRate     = 1
	DefaultConfigSubscribeRate = 1
	DefaultStatusPublishRate   = 1
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
	logger                      *log.Logger
	DataPublishRate             time.Duration
	DataPublishTickerReferences *time.Ticker
}

// NewIOTService returns a instance of the iot service
func NewIOTService(logger *log.Logger, ss sensors.Service, gs googleiot.Service) Service {
	i := &iot{
		sensors:   ss,
		googleiot: gs,
		logger:    logger,
	}
	return i
}

func (i *iot) PublishSensorDataSnapshotHandler(ctx context.Context) error {

	i.logger.Println("INFO: Fetching sensor data")
	data, err := i.sensors.FetchSensorData(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: failed to fetch the sensor data: %+s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	if err := i.googleiot.PublishSensorData(ctx, data); err != nil {
		e := fmt.Sprintf("ERROR: failed to publish the sensor data to cloud iot: %+s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	i.logger.Println("INFO: Successfully published the sensor data")
	return nil
}

func (i *iot) PublishDeviceStatus(ctx context.Context) error {

	i.logger.Println("INFO: fetching cpu temperature")
	var cpuTemp float32
	err := i.sensors.FetchCPUTemp(&cpuTemp)
	if err != nil {
		e := fmt.Sprintf("ERROR to get cpu temp: %+s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	data := &googleiot.DeviceStatus{
		CpuTemp: cpuTemp,
	}

	if err := i.googleiot.PublishDeviceState(ctx, data); err != nil {
		e := fmt.Sprintf("ERROR: failed to publish the device status to cloud iot: %s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	i.logger.Println("INFO: successfully published the device status")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.logger.Println("INFO: subscribing to google iot config changes")

	msg, err := i.googleiot.SubsribeToConfigChanges(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: failed to subscribe to the config changes: %s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	err = i.handleIOTCOnfigMessage(msg, ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: Failed to handle config update with error: %s", err.Error())
		i.logger.Println(e)
		return errors.New(e)
	}

	i.logger.Println("INFO: subscribed to google iot config changes published the device status")
	return nil
}

func (i *iot) IntializeIOTFunctions(ctx context.Context) {
	i.logger.Println("INFO: initialize the base iot functions")
	i.handleDataPublishRateUpdate(DefaultDataPublishRate, ctx)
	i.setupDefaultTimedFunctions(ctx)
}

func (i *iot) handleIOTCOnfigMessage(msg *googleiot.ConfigMessage, ctx context.Context) error {
	g, ctx := errgroup.WithContext(ctx)

	g.Go(func() error {
		i.logger.Println("INFO: handling power status updates")
		return i.handlePowerStatus(msg.PowerState)
	})

	g.Go(func() error {
		i.logger.Println("INFO: handling data publish rate update")
		i.handleDataPublishRateUpdate(msg.DataPublishRate, ctx)
		return nil
	})

	return g.Wait()
}

func (i *iot) setupDefaultTimedFunctions(ctx context.Context) {
	gobot.Every(DefaultStatusPublishRate*time.Minute, func() {
		err := i.PublishDeviceStatus(ctx)
		if err != nil {
			i.logger.Printf("ERROR: Failed to publish the device status: %+s", err.Error())
			return
		}
	})

	gobot.Every(DefaultConfigSubscribeRate*time.Minute, func() {
		err := i.SubscribeToIOTCoreConfig(ctx)
		if err != nil {
			i.logger.Printf("ERROR: Failed to subscribe to the iot config: %+s", err.Error())
			return
		}
	})
}

func (i *iot) handleDataPublishRateUpdate(rate time.Duration, ctx context.Context) {
	if rate == i.DataPublishRate {
		i.logger.Println("INFO: Data publish rate is the same as current")
		return
	}

	if rate == 0 {
		i.logger.Println("INFO: Data publish rate is 0, do not act")
		return
	}

	i.logger.Println("INFO: Updating the rate of the publish data process")
	t := gobot.Every(rate*time.Minute, func() {

		if i.DataPublishTickerReferences != nil {
			i.DataPublishTickerReferences.Stop()
		}

		err := i.PublishSensorDataSnapshotHandler(ctx)
		if err != nil {
			i.logger.Printf("ERROR: Failed to publish the sensor data: %+s", err.Error())
			return
		}
	})
	i.DataPublishTickerReferences = t
}

func (i *iot) handlePowerStatus(p googleiot.PowerState) error {
	var err error
	if p != "" {
		if p == googleiot.Off {
			i.logger.Println("INFO: turn off device")
			err = i.turnOffDevice()
		}

		if p == googleiot.Reboot {
			i.logger.Println("INFO: reboot device")
			err = i.rebootTheDevice()
		}
	} else {
		i.logger.Println("WARNING: Message does not have a valid power status")
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
