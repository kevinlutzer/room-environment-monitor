package iot

import (
	"context"
	"errors"
	googleiot2 "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/logger"
	sensors2 "github.com/kml183/room-environment-monitor/internal/sensors"
	"testing"

	"github.com/stretchr/testify/mock"

	"github.com/stretchr/testify/suite"
)

type IOTServerServiceTestSuite struct {
	suite.Suite
	sensors   *sensors2.MockInterface
	googleiot *googleiot2.MockInterface
	logger	logger.LoggerService
	iot       Interface
	ctx       context.Context
}

func (s *IOTServerServiceTestSuite) SetupTest() {
	s.sensors = &sensors2.MockInterface{}
	s.googleiot = &googleiot2.MockInterface{}
	s.logger = logger.NewLogger()
	s.iot = NewIOTService(s.logger, s.sensors, s.googleiot)
}

func (s *IOTServerServiceTestSuite) Test_PublishSensorDataSnapshot_ShouldReturnErrorWhenFetchSensorDataErrors() {

	fetchSensorDataErr := errors.New("Something It doesn't matter")
	s.sensors.On("FetchSensorData", mock.Anything).Return(nil, fetchSensorDataErr)

	err := s.iot.PublishSensorDataSnapshot(s.ctx)
	s.Assert().Equal(fetchSensorDataErr, err)
	s.sensors.AssertExpectations(s.T())
	s.googleiot.AssertExpectations(s.T())
}

func (s *IOTServerServiceTestSuite) Test_PublishSensorDataSnapshot_ShouldReturnErrorWhenPublishSensorDataErrors() {

	publishSensorDataErr := errors.New("Something It doesn't matter")

	s.sensors.On("FetchSensorData", mock.Anything).Return(&sensors2.SensorData{}, nil)
	s.googleiot.On("PublishSensorData", mock.Anything, mock.Anything).Return(publishSensorDataErr)
	err := s.iot.PublishSensorDataSnapshot(s.ctx)
	s.Assert().Equal(publishSensorDataErr, err)

	s.sensors.AssertExpectations(s.T())
	s.googleiot.AssertExpectations(s.T())
}

func (s *IOTServerServiceTestSuite) Test_PublishDeviceStatus_ShouldReturnErrorWhenFetchCPUTempErrors() {

	fetchCpuTempErr := errors.New("Something It doesn't matter")

	s.sensors.On("FetchCPUTemp").Return(float32(0), fetchCpuTempErr)

	err := s.iot.PublishDeviceStatus(s.ctx)
	s.Assert().Equal(fetchCpuTempErr, err)

	s.sensors.AssertExpectations(s.T())
	s.googleiot.AssertExpectations(s.T())
}

func (s *IOTServerServiceTestSuite) Test_PublishDeviceStatus_ShouldReturnErrorWhenPublishDeviceStateErrors() {

	publishDeviceStateErr := errors.New("Something It doesn't matter")

	s.sensors.On("FetchCPUTemp").Return(float32(0), nil)
	s.googleiot.On("PublishDeviceState", mock.Anything, mock.Anything).Return(publishDeviceStateErr)

	err := s.iot.PublishDeviceStatus(s.ctx)
	s.Assert().Equal(publishDeviceStateErr, err)

	s.sensors.AssertExpectations(s.T())
	s.googleiot.AssertExpectations(s.T())
}

func (s *IOTServerServiceTestSuite) Test_SubscribeToIOTCoreConfig_ShouldReturnErrorWhenSubsribeToConfigChangesErrors() {
	subscribeToIOTCoreConfigErr := errors.New("Something It doesn't matter")
	s.googleiot.On("SubsribeToConfigChanges", mock.Anything).Return(nil, subscribeToIOTCoreConfigErr)

	err := s.iot.SubscribeToIOTCoreConfig(s.ctx)
	s.Assert().Equal(subscribeToIOTCoreConfigErr, err)

	s.sensors.AssertExpectations(s.T())
	s.googleiot.AssertExpectations(s.T())
}

func Test_IOTServerServiceTestSuite(t *testing.T) {
	suite.Run(t, new(IOTServerServiceTestSuite))
}
