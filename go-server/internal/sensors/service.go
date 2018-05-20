package sensors

import (
	"context"
	"encoding/json"
	"errors"
	"time"

	"golang.org/x/sync/errgroup"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//IntializeSensors initializes the sensors
	IntializeSensors() error
}

type service struct{}

func NewSensorService() Service {
	return &service{}
}

func (s *service) FetchSensorData(ctx context.Context) (*SensorData, error) {
	g, ctx := errgroup.WithContext(ctx)

	gd := &GasData{}
	g.Go(func() error {
		return s.fetchGasData(gd)
	})

	ld := &LightData{}
	g.Go(func() error {
		return s.fetchLightData(ld)
	})

	var cpuTemp string
	g.Go(func() error {
		s, err := s.fetchCpuTemp()
		cpuTemp = s
		return err
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}

	sd := &SensorData{}
	sd.convertFromLightAndGasData(gd, ld, time.Now(), cpuTemp)

	return sd, nil
}

func (s *service) fetchCpuTemp() (string, error) {
	return "45.2 C", nil
}

func (s *service) fetchGasData(gd *GasData) error {
	m := "{\"temp\": \"32.6\", \"tvoc\": 2000, \"co2\": 23.5}"

	err := json.Unmarshal([]byte(m), gd)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) fetchLightData(ld *LightData) error {
	t := "{\"lux\": 3000}"

	err := json.Unmarshal([]byte(t), ld)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) IntializeSensors() error {
	return errors.New("Not yet implemented")
}
