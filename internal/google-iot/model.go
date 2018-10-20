package googleiot

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
	PowerStatus PowerStatus `json:"power_status"`
}

type DeviceStatus struct {
	Status  Status  `json:"status"`
	CpuTemp float32 `json:"cpu_temp"`
}
