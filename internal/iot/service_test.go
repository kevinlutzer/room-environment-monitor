package iot

import (
	"context"
	"testing"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	"github.com/stretchr/testify/suite"
)

type IOTServerServiceTestSuite struct {
	suite.Suite
	sensors   sensors.SensorsService
	googleiot googleiot.GoogleIOTService
	logger    config.LoggerService
	iot       IOTServerService
	ctx       context.Context
}

func (s *IOTServerServiceTestSuite) SetupTest() {
	// s.iot = NewIOTService(mocks.LoggerService{}, mocks.SensorsService{}, mocks.GoogleIOTService{})
}

func (s *IOTServerServiceTestSuite) TestReturnsError() {
	// actual, actualErr := s.whitelabelService.GetWhitelabeledDomainNames(context.TODO(), "", []string{"", ""})
	// s.Assert().Equal(expected, actual)
	// s.Assert().Equal(expectedErr, actualErr)
}

func Test_IOTServerServiceTestSuite(t *testing.T) {
	suite.Run(t, new(IOTServerServiceTestSuite))
}
