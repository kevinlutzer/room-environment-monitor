package sensors

import "context"

//go:generate mockery -inpkg -name Interface -case underscore
//Service represents the structure of the service layer
type Interface interface {
	//FetchSensorData fetches sensors data
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//FetchCPUTemp fetches the cpu temp
	FetchCPUTemp() (float32, error)
}