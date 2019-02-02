package iot

import (
	"context"
	"errors"
	"fmt"
	"os/exec"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type IOTServerService interface {
	PublishSensorDataSnapshotHandler(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
}

type iot struct {
	sensors   sensors.SensorsService
	googleiot googleiot.GoogleIOTService
	logger    config.LoggerService
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
	cpuTemp, err := i.sensors.FetchCPUTemp()
	if err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	data := &googleiot.DeviceStatus{
		CpuTemp: cpuTemp,
	}

	if err := i.googleiot.PublishDeviceState(ctx, data); err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	i.logger.StdOut("successfully published the device status\n")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.logger.StdOut("subscribing to google iot config changes\n")

	msg, err := i.googleiot.SubsribeToConfigChanges(ctx)
	if err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	err = i.handlePowerStatus(msg.PowerState)
	if err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	i.logger.StdOut("subscribed to google iot config changes published the device status\n")
	return nil
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
