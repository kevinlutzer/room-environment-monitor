package googleiot

import (
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

// The global status of the sensor

type Status string

const (
	Active   Status = "Active"
	Unactive Status = "Unactive"
)

type PowerStatus string

const (
	Off    PowerStatus = "Off"
	On     PowerStatus = "On"
	Reboot PowerStatus = "Reboot"
)

type ConfigMessage struct {
	FanState    sensors.FanState `json:"fan_status"`
	PowerStatus PowerStatus      `json:"power_status"`
}

type SensorStatus struct {
	Status Status `json:"status"`
}
