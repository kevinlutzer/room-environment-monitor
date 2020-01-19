package iot

import (
	"context"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
)

type stub struct {
}

// NewIOTServiceStub returns a stub version of the IOTServerService.
func NewIOTServiceStub() IOTServerService {
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

func (i *stub) handlePowerStatus(p googleiot.PowerState) error {
	return nil
}
