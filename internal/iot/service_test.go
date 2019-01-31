package iot

import (
	"testing"

	"github.com/stretchr/testify/assert"
)

func Test_PublishSensorDataSnapshotHandler(t *testing.T) {
	// loggingMock := &mocks.LoggerService{}
	// sensorServiceMock := &mocks.SensorsService{}
	// googleIOTServiceMock := &mocks.GoogleIOTService{}

	// testService := NewIOTService(loggingMock, sensorServiceMock, googleIOTServiceMock)
	// loggingMock.On()

	cases := []struct {
		name           string
		inputDrafts    string
		inputPublished string
		output         []string
	}{}

	for _, c := range cases {
		t.Run(c.name, func(t *testing.T) {
			actual := ""
			assert.Equal(t, c.output, actual)
		})
	}
}
