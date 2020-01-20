package sensors

import "context"

//Service represents the structure of the service layer
type Interface interface {
	//FetchSensorData fetches sensors data
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//FetchCPUTemp fetches the cpu temp
	FetchCPUTemp() (float32, error)
}