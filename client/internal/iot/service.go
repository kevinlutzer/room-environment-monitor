package iot

import (
	"context"
	"fmt"
	MQTT "github.com/eclipse/paho.mqtt.golang"
	"os/exec"

	googleiot "room-environment-monitor-client/internal/google-iot"
	"room-environment-monitor-client/internal/logger"
	"room-environment-monitor-client/internal/sensors"
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
	fmt.Println("Setting up subscription")
	err := i.googleiot.SubsribeToConfigChanges(ctx, func(client MQTT.Client, msg MQTT.Message) {
		i.logger.StdOut("Subscribing to change in configuration. Payout: %s", string(msg.Payload()))
		err := i.handleConfig(msg.Payload())
		if err != nil {
			i.logger.StdErr(err.Error())
		}
	})
	if err != nil {
		i.logger.StdErr(err.Error())
		return err
	}

	return nil
}

func (i *iot) FetchSensorDataSnapshot(ctx context.Context) (*sensors.SensorData, error) {
	return i.sensors.FetchSensorData(ctx)
}

var execCommand = exec.Command

func (i *iot) handleConfig(data []byte) error {
	if string(data) == "REBOOT" {
		fmt.Printf("Hello World\n")
		// cmd := execCommand("sudo", "/sbin/reboot")
		// _, err := cmd.Output()
		// return err
	}
	return nil
}
