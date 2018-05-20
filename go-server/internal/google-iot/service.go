package googleiot

import (
	"context"
	"errors"

	"github.com/kml183/room-environment-monitor/go-server/internal/sensors"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	PublishSensorData(ctx context.Context) error
}

type service struct {
	SensorsService sensors.Service
}

func NewGoogleIOTService(ss sensors.Service) Service {
	return &service{
		SensorsService: ss,
	}
}

func (s *service) PublishSensorData(ctx context.Context) error {
	return errors.New("Not implemented")
}
