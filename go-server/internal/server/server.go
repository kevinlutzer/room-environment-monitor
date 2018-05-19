package server

import (
	"encoding/json"
	"errors"
	"fmt"
	"net/http"

	"github.com/kml183/room-environment-monitor/go-server/internal/sensors"
)

const (
	// HTTPPort is the port to run the server on
	HTTPPort = ":8080"
)

type server struct {
	SensorsService sensors.Service
}

// NewHTTPServer returns a instance of the http server
func NewHTTPServer() error {

	// Setup Sensors Service
	ss := sensors.NewSensorService()
	s := &server{
		SensorsService: ss,
	}

	//Setup Handlers
	http.HandleFunc("/get-sensor-data", s.GetSnapshotHandler)
	http.HandleFunc("/initialize-sensors", s.InitializeSensorsHandler)

	fmt.Printf("Started HTTP handler on port %s", HTTPPort)
	if err := http.ListenAndServe(HTTPPort, nil); err != nil {
		return errors.New("Couldn't start http server")
	}

	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *server) GetSnapshotHandler(wr http.ResponseWriter, r *http.Request) {

	d, err := s.SensorsService.FetchSensorData()
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
	s.setResponse(wr, "Hello World", 200)
}

func (s *server) setResponse(wr http.ResponseWriter, message string, statusCode int) {
	wr.Write([]byte(message))
	wr.WriteHeader(statusCode)
	return
}
