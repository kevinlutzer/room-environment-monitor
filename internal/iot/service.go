package iot

import (
	"context"
	"errors"
	"fmt"
	"log"
	"os/exec"

	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type Service interface {
	PublishSensorDataSnapshotHandler(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
}

type iot struct {
	SensorsService   sensors.Service
	GoogleIOTService googleiot.Service
	Logger           *log.Logger
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

	i.Logger.Println("INFO - Fetching sensor data")
	data, err := i.SensorsService.FetchSensorData(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR - failed to fetch the sensor data: %+s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	if err := i.GoogleIOTService.PublishSensorData(ctx, data); err != nil {
		e := fmt.Sprintf("ERROR - failed to publish the sensor data to cloud iot: %+s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	i.Logger.Println("INFO - Successfully published the sensor data")
	return nil
}

func (i *iot) PublishDeviceStatus(ctx context.Context) error {

	i.Logger.Println("INFO - fetching cpu temperature")
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

	i.Logger.Println("INFO - successfully published the device status")
	return nil
}

func (i *iot) SubscribeToIOTCoreConfig(ctx context.Context) error {
	i.Logger.Println("INFO - subscribing to google iot config changes")

	msg, err := i.GoogleIOTService.SubsribeToConfigChanges(ctx)
	if err != nil {
		e := fmt.Sprintf("ERROR - failed to subscribe to the config changes: %s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	err = i.handleIOTCOnfigMessage(msg)
	if err != nil {
		e := fmt.Sprintf("ERROR - Failed to handle config update with error: %s", err.Error())
		i.Logger.Println(e)
		return errors.New(e)
	}

	i.Logger.Println("Request - subscribed to google iot config changes published the device status")
	return nil
}

func (i *iot) handleIOTCOnfigMessage(msg *googleiot.ConfigMessage) error {

	var err error

	// Handler power state
	if msg.PowerStatus != "" {
		i.Logger.Println("Request - turn off device")
		if msg.PowerStatus == googleiot.Off {
			err = i.turnOffDevice()
		}

		if msg.PowerStatus == googleiot.Reboot {
			err = i.rebootTheDevice()
		}
	} else {
		i.Logger.Println("Request - WARNING: Message does not have a valid power status")
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
