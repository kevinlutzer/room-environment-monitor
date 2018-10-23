package googleiot

import (
	"time"
)

type PowerState string

const (
	Off    PowerState = "Off"
	On     PowerState = "On"
	Reboot PowerState = "Reboot"
)

type ConfigMessage struct {
	PowerState      PowerState    `json:"power_state"`
	DataPublishRate time.Duration `json:"data_publish_rate"`
}

type DeviceStatus struct {
	CpuTemp float32 `json:"cpu_temp"`
}
