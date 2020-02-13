package iot

import (
	"context"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	"time"
)

type stub struct {
}

// NewIOTServiceStub returns a stub version of the Interface.
func NewIOTServiceStub() Interface {
	return &stub{
	}
}

func (i *stub) PublishSensorDataSnapshot(ctx context.Context) error {
	return nil
}

func (i *stub) PublishDeviceStatus(ctx context.Context) error {
	return nil
}

func (i *stub) SubscribeToIOTCoreConfig(ctx context.Context) error {
	return nil
}

func (i *stub) FetchSensorDataSnapshot(ctx context.Context) (*sensors.SensorData, error) {
	return &sensors.SensorData{
		Lux:       1635,
		Pressure:  102456,
		Humidity:  123,
		RoomTemp:  23.5,
		CO2:       10923,
		TVOC:      532,
		CPUTemp:   45.6,
		TimeStamp: time.Now(),
	}, nil
}