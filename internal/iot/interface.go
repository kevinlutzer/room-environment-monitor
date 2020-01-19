package iot

import "context"

type IOTServerService interface {
	PublishSensorDataSnapshot(context.Context) error
	PublishDeviceStatus(context.Context) error
	SubscribeToIOTCoreConfig(context.Context) error
}