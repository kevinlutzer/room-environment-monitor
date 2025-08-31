package httpserver

import (
	"encoding/json"
	"net/http/httptest"
	"testing"

	"github.com/kml183/room-environment-monitor/internal/mocks"
	"github.com/stretchr/testify/assert"
)

func TestGetMacAddressHandler(t *testing.T) {
	// Create mock services
	mockLogger := &mocks.LoggerService{}
	mockSensors := &mocks.SensorsService{}
	mockIOT := &mocks.IOTServerService{}

	// Setup the logger mock to expect the debug calls
	mockLogger.On("StdOut", "http: calling handler getMacAddressHandler\n")

	// Create the HTTP service
	service := NewHTTPService(mockLogger, mockSensors, mockIOT).(*httpService)

	// Create a test request
	req := httptest.NewRequest("GET", "/get-mac-address", nil)
	w := httptest.NewRecorder()

	// Call the handler
	service.getMacAddressHandler(w, req)

	// Check the response
	resp := w.Result()
	assert.Equal(t, "application/json", resp.Header.Get("Content-Type"))
	
	// Parse the response body
	var response MacAddressMessage
	err := json.NewDecoder(resp.Body).Decode(&response)
	assert.NoError(t, err)

	// Verify the response structure
	assert.Equal(t, "successfully fetched MAC address for eth0", response.Message)
	assert.NotEmpty(t, response.MacAddress)
	
	// Verify MAC address format (should be in format xx:xx:xx:xx:xx:xx)
	assert.Regexp(t, `^([0-9a-f]{2}:){5}[0-9a-f]{2}$`, response.MacAddress)

	// Verify mocks were called as expected
	mockLogger.AssertExpectations(t)
}

func TestGetEth0MacAddress(t *testing.T) {
	// Test the helper function directly
	macAddress, err := getEth0MacAddress()
	
	// The function should return a MAC address if eth0 exists
	// Note: This test might fail in environments without eth0
	if err != nil {
		// If eth0 doesn't exist, that's a valid scenario for some environments
		t.Logf("eth0 interface not found: %v", err)
	} else {
		assert.NotEmpty(t, macAddress)
		// Verify MAC address format
		assert.Regexp(t, `^([0-9a-f]{2}:){5}[0-9a-f]{2}$`, macAddress)
	}
}