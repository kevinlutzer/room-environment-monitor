package googleiot

import "time"

type PowerState string

const (
	Off    PowerState = "Off"
	On     PowerState = "On"
	Reboot PowerState = "Reboot"
)

type ConfigMessage struct {
	PowerState PowerState `json:"power_state"`
}

type DeviceStatus struct {
	CpuTemp   float32   `json:"cpu_temp"`
	TimeStamp time.Time `json:"timestamp"`
}
