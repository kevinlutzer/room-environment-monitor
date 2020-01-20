package iot

import (
	"context"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type Interface interface {
	PublishSensorDataSnapshot(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
	FetchSensorDataSnapshot(ctx context.Context) (*sensors.SensorData, error)
}