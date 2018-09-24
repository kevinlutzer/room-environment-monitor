package server

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"

	"github.com/kml183/room-environment-monitor/internal/config"

	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type Message struct {
	Message string `json:"message"`
}

type DataMessage struct {
	Message string `json:"message"`
	Data    *sensors.SensorData
}

const (
	// HTTPPort is the port to run the server on
	HTTPPort = "192.168.1.111:8080"
)

type server struct {
	SensorsService   sensors.Service
	GoogleIOTService googleiot.Service
	Logger           *log.Logger
	apiKey           string
}

// StartHTTPServer returns a instance of the http server
func StartHTTPServer(logger *log.Logger, ss sensors.Service, gs googleiot.Service) error {

	//Load The api key from file
	val, err := ioutil.ReadFile(config.APIKeyFile)
	if err != nil {
		logger.Fatalf("Failed to read api key file", err.Error())
		return err
	}

	s := &server{
		SensorsService:   ss,
		GoogleIOTService: gs,
		Logger:           logger,
		apiKey:           string(val),
	}

	//Setup Handlers
	mux := http.NewServeMux()
	mux.HandleFunc("/get-sensor-data-snapshot", s.GetSensorDataSnapshotHandler)
	mux.HandleFunc("/publish-sensor-data-snapshot", s.PublishSensorDataSnapshotHandler)
	mux.HandleFunc("/publish-device-status", s.PublishDeviceStatus)
	mux.HandleFunc("/toggle-fan", s.ToggleFanHandler)
	mux.HandleFunc("/subscribe-iot-config", s.SubscribeToIOTCoreConfig)

	//Start the http server (blocking)
	fmt.Printf("Started HTTP handler on port %s \n", HTTPPort)
	if err := http.ListenAndServe(HTTPPort, mux); err != nil {
		return errors.New("Couldn't start http server")
	}

	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *server) ToggleFanHandler(wr http.ResponseWriter, r *http.Request) {

	s.Logger.Printf("Request - calling handler: GetSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Printf("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	//s.SensorsService.ToggleFan()

	str := "Request - successfully toggled the fan"
	s.Logger.Println(str)
	s.setStringResponse(wr, string(str), 200)

}

// Handler is the main http handler for the room environment monitor app
func (s *server) GetSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Printf("\nRequest - calling handler: GetSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Printf("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	d, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.Logger.Printf("Request - ERROR: failed to fetch sensor data > %s\n", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("could't fetch the sensor data > %s", err.Error()), 500)
		return
	}

	md, err := json.Marshal(d)
	if err != nil {
		s.Logger.Printf("Request - ERROR: failed to marshal sensor data > %s \n", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("couldn't marshal the data > %s", err.Error()), 500)
		return
	}

	s.Logger.Printf("Request - resulting data: %s\n", string(md))
	s.setStringResponse(wr, string(md), 200)
}

func (s *server) PublishSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Printf("\nRequest - calling handler: PublishSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Printf("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	data, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.Logger.Printf("Request - ERROR to fetch sensor data > %s \n", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("can't get snapshot of sensor data > %s", err.Error()), 500)
		return
	}

	if err := s.GoogleIOTService.PublishSensorData(ctx, data); err != nil {
		s.Logger.Printf("Request - ERROR: failed to publish the sensor data to cloud iot > %s \n", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("can't publish the sensor data > %s", err.Error()), 500)
		return
	}

	s.Logger.Println("Request - successfully published the sensor data\n")
	s.setStringResponse(wr, "Successfully published the sensor data", 200)
}

func (s *server) PublishDeviceStatus(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Printf("\nRequest - calling handler: PublishDeviceStatus")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Printf("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	data := &googleiot.SensorStatus{
		Status: "Active",
	}

	if err := s.GoogleIOTService.PublishDeviceState(ctx, data); err != nil {
		s.Logger.Printf("Request - ERROR: failed to publish the device status to cloud iot > %s \n", err.Error())
		s.setStringResponse(wr, "can't publish the device status ", 500)
		return
	}

	s.Logger.Println("Request - successfully published the device status\n")
	s.setStringResponse(wr, "Successfully published the device status", 200)
}

func (s *server) SubscribeToIOTCoreConfig(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Printf("\nRequest - calling handler: SubscribeToIOTCoreConfig")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Printf("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	msg, err := s.GoogleIOTService.SubsribeToConfigChanges(ctx)
	if err != nil {
		s.Logger.Printf("Request - ERROR: failed to subscribe to the config changes > %s \n", err.Error())
		s.setStringResponse(wr, "can't publish the device status ", 500)
		return
	}

	s.Logger.Printf("%+v", msg)

	if msg.FanState != "" {
		s.Logger.Println("Request - Setting fan state")
		err = s.SensorsService.SetFanStatus(msg.FanState)
	} else {
		s.Logger.Println("Request - ERROR: Message does not have a valid fan state")
	}

	if err != nil {
		s.Logger.Println("Request - ERROR: Failed to toggle fan")
		s.setStringResponse(wr, "can't toggle fan", 500)
	}

	s.Logger.Println("Request - subscribed to google iot config changes published the device status")
	s.setStringResponse(wr, string(msg.FanState), 200)
}

func (s *server) setStringResponse(wr http.ResponseWriter, message string, statusCode int) {
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")

	msg := &Message{Message: message}
	b, _ := json.Marshal(msg)

	wr.Write(b)
	return
}

func (s *server) setDataResponse(wr http.ResponseWriter, data *sensors.SensorData, statusCode int) {
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")

	msg := &DataMessage{
		Message: "Recieved Data",
		Data:    data,
	}

	b, _ := json.Marshal(msg)

	wr.Write(b)
	return
}
