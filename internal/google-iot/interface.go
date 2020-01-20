package googleiot

import (
	"context"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

//GoogleIOTService represents the structure of the service layer
type GoogleIOTService interface {
	//PublishSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data *sensors.SensorData) error
	//PublishDeviceState fetches sensors data
	PublishDeviceState(ctx context.Context, status *DeviceStatus) error
	//SubsribeToConfigChanges subscribe to any config changes
	SubsribeToConfigChanges(ctx context.Context) (*ConfigMessage, error)
}
