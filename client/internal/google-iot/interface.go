package googleiot

import (
	"context"
	"room-environment-monitor-client/internal/sensors"

	MQTT "github.com/eclipse/paho.mqtt.golang"
)

//go:generate mockery -inpkg -name Interface -case underscore
//Interface represents the structure of the service layer
type Interface interface {
	//PublishSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data *sensors.SensorData) error
	//SubsribeToConfigChanges subscribe to any logger changes
	SubsribeToConfigChanges(ctx context.Context, f func(client MQTT.Client, msg MQTT.Message)) error
}
