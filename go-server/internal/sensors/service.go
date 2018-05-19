package sensors

import (
	"encoding/json"
	"errors"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	FetchSensorData() (*SensorData, error)
	//IntializeSensors initializes the sensors
	IntializeSensors() error
}

type service struct{}

func NewSensorService() Service {
	return &service{}
}

func (s *service) FetchSensorData() (*SensorData, error) {
	// g, ctx := errgroup.WithContext(ctx)

	// g.Go(s.fetchGasData)
	// g.Go(s.fetchLightData)

	// err := g.Wait()
	// if err != nil {
	// 	return nil, err
	// }

	return &SensorData{}, nil
}

func (s *service) fetchGasData() error {
	m := "{temp: \"32.6\", tvoc: 2000, co2: 23.5}"

	d := &GasData{}
	err := json.Unmarshal([]byte(m), d)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) fetchLightData() error {
	t := "{lux: 3000}"

	res := &LightData{}
	err := json.Unmarshal([]byte(t), res)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) IntializeSensors() error {
	return errors.New("Not yet implemented")
}
