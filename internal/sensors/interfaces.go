package sensors

import "context"

//Service represents the structure of the service layer
type TSL2561DriverInterface interface {
	//FetchSensorData fetches sensors data
	// GetLuminocity
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//FetchCPUTemp fetches the cpu temp
	FetchCPUTemp(cpuTemp *float32) error
}

// //Service represents the structure of the service layer
// type SensorsService interface {
// 	//FetchSensorData fetches sensors data
// 	FetchSensorData(ctx context.Context) (*SensorData, error)
// 	//FetchCPUTemp fetches the cpu temp
// 	FetchCPUTemp(cpuTemp *float32) error
// }

// //Service represents the structure of the service layer
// type SensorsService interface {
// 	//FetchSensorData fetches sensors data
// 	FetchSensorData(ctx context.Context) (*SensorData, error)
// 	//FetchCPUTemp fetches the cpu temp
// 	FetchCPUTemp(cpuTemp *float32) error
// }
