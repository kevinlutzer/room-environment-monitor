package iot

import (
	"context"
	"room-environment-monitor-client/internal/sensors"
)

//go:generate mockery -inpkg -name Interface -case underscore
type Interface interface {
	PublishSensorDataSnapshot(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
	FetchSensorDataSnapshot(ctx context.Context) (*sensors.SensorData, error)
}
