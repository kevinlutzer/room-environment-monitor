package googleiot

import (
	"context"
	"room-environment-monitor-client/internal/sensors"
)

//go:generate mockery -inpkg -name Interface -case underscore
//Interface represents the structure of the service layer
type Interface interface {
	//PublishSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data *sensors.SensorData) error
	//PublishDeviceState fetches sensors data
	PublishDeviceState(ctx context.Context, status *DeviceStatus) error
	//SubsribeToConfigChanges subscribe to any logger changes
	SubsribeToConfigChanges(ctx context.Context) (*ConfigMessage, error)
}
