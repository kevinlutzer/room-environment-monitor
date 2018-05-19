package sensors

import (
	"errors"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	FetchSensorData() error
	//IntializeSensors initializes the sensors
	IntializeSensors() error
}

type service struct{}

func NewSensorService() Service {
	s := service{}
	return s
}

func (s service) FetchSensorData() error {
	return nil
}

func (s service) fetchGasData() (*GasData, error) {
	return nil, nil
}

func (s service) fetchLightData() (*LightData, error) {
	return nil, nil
}

func (s service) IntializeSensors() error {
	return errors.New("Not yet implemented")
}
