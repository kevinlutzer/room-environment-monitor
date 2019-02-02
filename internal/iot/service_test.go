package iot

import (
	"context"
	"errors"
	"testing"

	"github.com/stretchr/testify/mock"

	"github.com/kml183/room-environment-monitor/internal/mocks"
	"github.com/stretchr/testify/suite"
)

type IOTServerServiceTestSuite struct {
	suite.Suite
	sensors   mocks.SensorsService
	googleiot mocks.GoogleIOTService
	logger    mocks.LoggerService
	iot       IOTServerService
	ctx       context.Context
}

func (s *IOTServerServiceTestSuite) SetupTest() {
	s.sensors = mocks.SensorsService{}
	s.googleiot = mocks.GoogleIOTService{}
	s.logger = mocks.LoggerService{}
	s.iot = NewIOTService(&s.logger, &s.sensors, &s.googleiot)
}

func (s *IOTServerServiceTestSuite) Test_PublishDeviceStatus_ShouldReturnErrorWhenFetchCPUTempErrors() {

	fetchCpuTempErr := errors.New("Something It doesn't matter")

	s.logger.On("StdOut", mock.Anything)
	s.logger.On("StdErr", mock.Anything)
	s.sensors.On("FetchCPUTemp").Return(float32(0), fetchCpuTempErr)

	err := s.iot.PublishDeviceStatus(s.ctx)
	s.Assert().Equal(fetchCpuTempErr, err)
}

func (s *IOTServerServiceTestSuite) Test_PublishDeviceStatus_ShouldReturnErrorWhenPublishDeviceStateErrors() {

	publishDeviceStateErr := errors.New("Something It doesn't matter")

	s.logger.On("StdOut", mock.Anything)
	s.logger.On("StdErr", mock.Anything)
	s.sensors.On("FetchCPUTemp").Return(float32(0), nil)
	s.googleiot.On("PublishDeviceState", mock.Anything, mock.Anything).Return(publishDeviceStateErr)

	err := s.iot.PublishDeviceStatus(s.ctx)
	s.Assert().Equal(publishDeviceStateErr, err)
}

func (s *IOTServerServiceTestSuite) Test_SubscribeToIOTCoreConfig_ShouldReturnErrorWhenSubsribeToConfigChangesErrors() {
	subscribeToIOTCoreConfigErr := errors.New("Something It doesn't matter")
	s.logger.On("StdOut", mock.Anything)
	s.logger.On("StdErr", mock.Anything)
	s.googleiot.On("SubsribeToConfigChanges", mock.Anything).Return(nil, subscribeToIOTCoreConfigErr)

	err := s.iot.SubscribeToIOTCoreConfig(s.ctx)
	s.Assert().Equal(subscribeToIOTCoreConfigErr, err)
}

// func (s *IOTServerServiceTestSuite) Test_SubscribeToIOTCoreConfig_ShouldReturnErrorWhenSubsribeToConfigChangesErrors() {

// 	msg := &googleiot.ConfigMessage{
// 		PowerState: googleiot.On,
// 	}
// 	s.logger.On("StdOut", mock.Anything)
// 	s.logger.On("StdErr", mock.Anything)
// 	s.googleiot.On("SubsribeToConfigChanges", mock.Anything).Return(msg, nil)
// 	err := s.iot.SubscribeToIOTCoreConfig(s.ctx)
// 	s.Assert().Equal(subscribeToIOTCoreConfigErr, err)
// }

func Test_IOTServerServiceTestSuite(t *testing.T) {
	suite.Run(t, new(IOTServerServiceTestSuite))
}
