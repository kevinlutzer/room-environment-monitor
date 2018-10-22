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
	PowerStatus         PowerStatus `json:"power_status"`
	DataPublishRate     uint64      `json:"data_publish_rate"`
	StatusPublishRate   uint64      `json:"status_publish_rate"`
	ConfigSubscribeRate uint64      `json:"config_subscribe_rate"`
}

type DeviceStatus struct {
	Status  Status  `json:"status"`
	CpuTemp float32 `json:"cpu_temp"`
}
