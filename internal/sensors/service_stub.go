package sensors

import (
	"context"
	"time"
)

type stub struct {}

// NewSensorService returns a new instance of the Service interface
func NewSensorServiceStub() SensorsService {
	return &stub{}
}

func (s *stub) FetchSensorData(ctx context.Context) (*SensorData, error) {
	return &SensorData{
		Lux:       1635,
		Pressure:  102456,
		Humidity:  123,
		RoomTemp:  23.5,
		CO2:       10923,
		TVOC:      532,
		CPUTemp:   45.6,
		TimeStamp: time.Now(),
	}, nil
}

func (s *stub) FetchCPUTemp() (float32, error) {
	return 45.6, nil
}