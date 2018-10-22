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
	DefaultDataPublishRate = 0
)

type Service interface {
	PublishSensorDataSnapshotHandler(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
	IntializeIOTFunctions(ctx context.Context) error
}

type iot struct {
	SensorsService              sensors.Service
	GoogleIOTService            googleiot.Service
	Logger                      *log.Logger
	DataPublishRate             time.Duration
	DataPublishTickerReferences *time.Ticker
}

// NewIOTService returns a instance of the iot service
func NewIOTService(logger *log.Logger, ss sensors.Service, gs googleiot.Service) Service {
	i := &iot{
		SensorsService:   ss,
		GoogleIOTService: gs,
		Logger:           logger,
	}
	return i
}

func (i *iot) PublishSensorDataSnapshotHandler(ctx context.Context) error {

	i.Logger.Println("INFO: Fetching sensor data")
	data, err := i.SensorsService.FetchSensorData(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: failed to fetch the sensor data: %+s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	if err := i.GoogleIOTService.PublishSensorData(ctx, data); err != nil {
		e := fmt.Sprintf("ERROR: failed to publish the sensor data to cloud iot: %+s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	i.Logger.Println("INFO: Successfully published the sensor data")
	return nil
}

func (i *iot) PublishDeviceStatus(ctx context.Context) error {

	i.Logger.Println("INFO: fetching cpu temperature")
	var cpuTemp float32
	err := i.SensorsService.FetchCPUTemp(&cpuTemp)
	if err != nil {
		e := fmt.Sprintf("ERROR to get cpu temp: %+s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	data := &googleiot.DeviceStatus{
		Status:  "Active",
		CpuTemp: cpuTemp,
	}

	if err := i.GoogleIOTService.PublishDeviceState(ctx, data); err != nil {
		e := fmt.Sprintf("ERROR: failed to publish the device status to cloud iot: %s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	i.Logger.Println("INFO: successfully published the device status")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.Logger.Println("INFO: subscribing to google iot config changes")

	msg, err := i.GoogleIOTService.SubsribeToConfigChanges(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: failed to subscribe to the config changes: %s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	err = i.handleIOTCOnfigMessage(msg, ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR: Failed to handle config update with error: %s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	i.Logger.Println("INFO: subscribed to google iot config changes published the device status")
	return nil
}

func (i *iot) IntializeIOTFunctions(ctx context.Context) error {
	i.Logger.Println("INFO: initialize the base iot functions")

	g, ctx := errgroup.WithContext(ctx)
	g.Go(func() error {
		i.Logger.Println("INFO: handling data publish rate update")
		return i.handleDataPublishRateUpdate(5, ctx)
	})

	return g.Wait()
}

func (i *iot) handleIOTCOnfigMessage(msg *googleiot.ConfigMessage, ctx context.Context) error {
	i.Logger.Printf("INFO: supplied msg is %+v\n", msg)
	g, ctx := errgroup.WithContext(ctx)

	g.Go(func() error {
		i.Logger.Println("INFO: handling power status updates")
		return i.handlePowerStatus(msg.PowerStatus)
	})

	g.Go(func() error {
		i.Logger.Println("INFO: handling data publish rate update")
		return i.handleDataPublishRateUpdate(msg.DataPublishRate, ctx)
	})

	return g.Wait()
}

func (i *iot) handleDataPublishRateUpdate(rate time.Duration, ctx context.Context) error {
	if rate == i.DataPublishRate {
		i.Logger.Println("INFO: Data publish rate is the same as current")
		return nil
	}

	if rate == 0 {
		i.Logger.Println("INFO: Data publish rate is 0, do not act")
		return nil
	}

	i.Logger.Println("INFO: Updating the rate of the publish data process")
	t := gobot.Every(rate*time.Minute, func() {

		if i.DataPublishTickerReferences != nil {
			i.DataPublishTickerReferences.Stop()
		}

		err := i.PublishSensorDataSnapshotHandler(ctx)
		if err != nil {
			i.Logger.Printf("ERROR: Failed to publish the sensor data: %+s", err.Error())
			return
		}
	})
	i.DataPublishTickerReferences = t
	return nil
}

func (i *iot) handlePowerStatus(p googleiot.PowerStatus) error {
	var err error
	if p != "" {
		if p == googleiot.Off {
			i.Logger.Println("INFO: turn off device")
			err = i.turnOffDevice()
		}

		if p == googleiot.Reboot {
			i.Logger.Println("INFO: reboot device")
			err = i.rebootTheDevice()
		}
	} else {
		i.Logger.Println("WARNING: Message does not have a valid power status")
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
