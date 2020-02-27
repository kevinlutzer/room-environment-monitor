package iot

import (
	"context"
	"os/exec"

	googleiot "room-environment-monitor-client/internal/google-iot"
	"room-environment-monitor-client/internal/logger"
	"room-environment-monitor-client/internal/sensors"
)

type PowerConfigMessage string

const (
	OnMessage     PowerConfigMessage = "leave device on \n"
	OffMessage    PowerConfigMessage = "turning device off \n"
	RebootMessage PowerConfigMessage = "rebooting the device \n"
)

type iot struct {
	sensors   sensors.Interface
	googleiot googleiot.Interface
	logger    logger.LoggerService
}

// NewIOTService returns a instance of the iot service
func NewIOTService(logger logger.LoggerService, ss sensors.Interface, gs googleiot.Interface) Interface {
	i := &iot{
		sensors:   ss,
		googleiot: gs,
		logger:    logger,
	}
	return i
}

func (i *iot) PublishSensorDataSnapshot(ctx context.Context) error {
	data, err := i.sensors.FetchSensorData(ctx)
	if err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	if err := i.googleiot.PublishSensorData(ctx, data); err != nil {
		i.logger.StdErr(err.Error())
		return err
	}
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
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
	return nil
}

func (i *iot) handlePowerStatus(p googleiot.PowerState) error {
	var err error
	if p != "" {
		if p == googleiot.Off {
			i.logger.StdOut(string(OffMessage))
			err = i.turnOffDevice()
		}

		if p == googleiot.Reboot {
			i.logger.StdOut(string(RebootMessage))
			err = i.rebootTheDevice()
		}

		if p == googleiot.On {
			i.logger.StdOut(string(OnMessage))
		}
	} else {
		i.logger.StdOut("Message does not have a valid power status\n")
	}
	return err
}

func (i *iot) FetchSensorDataSnapshot(ctx context.Context) (*sensors.SensorData, error) {
	return i.sensors.FetchSensorData(ctx)
}

var execCommand = exec.Command

func (i *iot) turnOffDevice() error {
	cmd := execCommand("sudo", "/sbin/shutdown", "-P", "now")
	_, err := cmd.Output()
	return err
}

func (i *iot) rebootTheDevice() error {
	cmd := execCommand("sudo", "/sbin/reboot")
	_, err := cmd.Output()
	return err
}
