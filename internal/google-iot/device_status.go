package googleiot

// The global status of the sensor

type Status string

const (
	Active   Status = "Active"
	Unactive Status = "Unactive"
)

type SensorStatus struct {
	Status Status `json:"status"`
}
