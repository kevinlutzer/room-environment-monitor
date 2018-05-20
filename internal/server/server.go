package server

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"net/http"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

const (
	// HTTPPort is the port to run the server on
	HTTPPort = ":8080"
)

type server struct {
	SensorsService   sensors.Service
	GoogleIOTService googleiot.Service
}

// NewHTTPServer returns a instance of the http server
func NewHTTPServer() error {

	// Create SSL Certs
	certs, err := config.GetSSLCerts()
	if err != nil {
		return err
	}

	// Fetch Google IOT Config
	iotConfig := config.GetGoogleIOTConfig()

	// Setup Services and Server
	ss := sensors.NewSensorService()
	gs := googleiot.NewGoogleIOTService(certs, iotConfig)
	s := &server{
		SensorsService:   ss,
		GoogleIOTService: gs,
	}

	//Setup Handlers
	http.HandleFunc("/get-sensor-data", s.GetSensorDataHandler)
	http.HandleFunc("/initialize-sensors", s.InitializeSensorsHandler)
	http.HandleFunc("/publish-sensor-data-snapshot", s.PublishSensorDataSnapshot)

	//Start the http server
	fmt.Printf("Started HTTP handler on port %s", HTTPPort)
	if err := http.ListenAndServe(HTTPPort, nil); err != nil {
		return errors.New("Couldn't start http server")
	}

	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *server) GetSensorDataHandler(wr http.ResponseWriter, r *http.Request) {

	ctx := context.TODO()

	d, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.setResponse(wr, fmt.Sprintf("could't fetch the sensor data > %s", err.Error()), 500)
	}

	md, err := json.Marshal(d)
	if err != nil {
		s.setResponse(wr, fmt.Sprintf("couldn't marshal the data > %s", err.Error()), 500)
	}

	s.setResponse(wr, string(md), 200)
	return
}

func (s *server) InitializeSensorsHandler(wr http.ResponseWriter, r *http.Request) {
	if err := s.SensorsService.IntializeSensors(); err != nil {
		s.setResponse(wr, fmt.Sprintf("can't initialize the sensors > %s", err.Error()), 500)
		return
	}
	s.setResponse(wr, "Successfully intialized the sensors", 200)
}

func (s *server) PublishSensorDataSnapshot(wr http.ResponseWriter, r *http.Request) {

	ctx := context.TODO()

	data, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.setResponse(wr, fmt.Sprintf("can't get snapshot of sensor data > %s", err.Error()), 500)
		return
	}

	mData, err := json.Marshal(data)
	if err != nil {
		s.setResponse(wr, fmt.Sprintf("can marshal sensor snapshot data > %s", err.Error()), 500)
		return
	}

	if err := s.GoogleIOTService.PublishSensorData(ctx, string(mData)); err != nil {
		s.setResponse(wr, fmt.Sprintf("can't publish the sensor data > %s", err.Error()), 500)
		return
	}
	s.setResponse(wr, "Successfully published the sensor data", 200)
}

func (s *server) setResponse(wr http.ResponseWriter, message string, statusCode int) {
	wr.Write([]byte(message))
	wr.WriteHeader(statusCode)
	return
}
