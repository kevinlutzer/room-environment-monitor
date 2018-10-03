package server

import (
	"encoding/json"
	"errors"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os/exec"

	"github.com/kml183/room-environment-monitor/internal/config"

	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type Message struct {
	Message string `json:"message"`
}

type DataMessage struct {
	Message string              `json:"message"`
	Data    *sensors.SensorData `json:"data"`
}

const (
	// HTTPPort is the port to run the server on
	HTTPPort = "192.168.1.144:8080"
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
		fmt.Printf("Failed to read api key file", err.Error())
		return errors.New("Failed to read the api key file")
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
	fmt.Printf("Started HTTP handler on port %s\n", HTTPPort)
	if err := http.ListenAndServe(HTTPPort, mux); err != nil {
		return err
	}

	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *server) ToggleFanHandler(wr http.ResponseWriter, r *http.Request) {

	s.Logger.Println("Request - calling handler: GetSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Println("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	err := s.SensorsService.SetFanStatus(sensors.ToggleFan)
	if err != nil {
		s.Logger.Panicln("Request - ERROR: failed to set the fan state ")
		return
	}

	s.Logger.Println("Request - successfully toggled the fan")
	s.setStringResponse(wr, "successfully toggled the fan", 200)
}

// Handler is the main http handler for the room environment monitor app
func (s *server) GetSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Println("Request - calling handler: GetSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Println("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	d, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.Logger.Println("Request - ERROR: failed to fetch sensor data > %s", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("could't fetch the sensor data > %s", err.Error()), 500)
		return
	}

	s.setDataResponse(wr, d, 200)
}

func (s *server) PublishSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Println("Request - calling handler: PublishSensorDataSnapshotHandler")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Println("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	data, err := s.SensorsService.FetchSensorData(ctx)
	if err != nil {
		s.Logger.Println("Request - ERROR to fetch sensor data > %s", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("can't get snapshot of sensor data > %s", err.Error()), 500)
		return
	}

	if err := s.GoogleIOTService.PublishSensorData(ctx, data); err != nil {
		s.Logger.Println("Request - ERROR: failed to publish the sensor data to cloud iot > %s", err.Error())
		s.setStringResponse(wr, fmt.Sprintf("can't publish the sensor data > %s", err.Error()), 500)
		return
	}

	s.Logger.Println("Request - successfully published the sensor data")
	s.setStringResponse(wr, "Successfully published the sensor data", 200)
}

func (s *server) PublishDeviceStatus(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Println("\nRequest - calling handler: PublishDeviceStatus")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Println("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	data := &googleiot.SensorStatus{
		Status: "Active",
	}

	if err := s.GoogleIOTService.PublishDeviceState(ctx, data); err != nil {
		s.Logger.Println("Request - ERROR: failed to publish the device status to cloud iot > %s", err.Error())
		s.setStringResponse(wr, "can't publish the device status ", 500)
		return
	}

	s.Logger.Println("Request - successfully published the device status")
	s.setStringResponse(wr, "Successfully published the device status", 200)
}

func (s *server) turnOffDevice(wr http.ResponseWriter, status googleiot.PowerStatus) error {
	if status != googleiot.Off {
		return nil
	}
	cmd := exec.Command("sudo", "/sbin/shutdown", "-P", "now")
	_, err := cmd.Output()
	return err
}

func (s *server) handleIOTCOnfigMessage(wr http.ResponseWriter, msg *googleiot.ConfigMessage) error {

	var err error

	// Handle fan state
	if msg.FanState != "" {
		s.Logger.Println("Request - setting fan state")
		err = s.SensorsService.SetFanStatus(msg.FanState)
	} else {
		s.Logger.Println("Request - WARNING: Message does not have a valid fan state")
	}

	// Handler power state
	if msg.PowerStatus != "" {
		s.Logger.Println("Request - turn off device")
		if msg.PowerStatus == "Off" {
			err = s.turnOffDevice(wr, msg.PowerStatus)
		}
	} else {
		s.Logger.Println("Request - WARNING: Message does not have a valid power status")
	}

	return err
}

func (s *server) SubscribeToIOTCoreConfig(wr http.ResponseWriter, r *http.Request) {

	ctx := r.Context()
	s.Logger.Printf("\nRequest - calling handler: SubscribeToIOTCoreConfig")

	p := r.URL.Query().Get("api_key")
	if p != s.apiKey {
		s.Logger.Println("Request - WARNING: api key \"%s\" does not match known key \"%s\"", p, s.apiKey)
		s.setStringResponse(wr, "api key is incorrect or was not passed", 403)
		return
	}

	msg, err := s.GoogleIOTService.SubsribeToConfigChanges(ctx)
	if err != nil {
		s.Logger.Printf("Request - ERROR: failed to subscribe to the config changes > %s", err.Error())
		s.setStringResponse(wr, "can't publish the device status ", 500)
		return
	}

	err = s.handleIOTCOnfigMessage(wr, msg)
	if err != nil {
		s.Logger.Printf("Request - ERROR: Failed to handle config update with error > %s", err.Error())
		s.setStringResponse(wr, "can't handle config update", 500)
		return
	}

	s.Logger.Println("Request - subscribed to google iot config changes published the device status")
	s.setStringResponse(wr, "successfully subscribed to the iot config data", 200)
}

func (s *server) setStringResponse(wr http.ResponseWriter, message string, statusCode int) {
	msg := &Message{Message: message}
	b, _ := json.Marshal(msg)

	wr.Write(b)
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")
	return
}

func (s *server) setDataResponse(wr http.ResponseWriter, data *sensors.SensorData, statusCode int) {

	msg := &DataMessage{
		Message: "successfuly fetched data from the sensors",
		Data:    data,
	}

	b, err := json.Marshal(msg)
	if err != nil {
		s.Logger.Printf("Request - ERROR: failed to marshal the data %s", err.Error())
		s.setStringResponse(wr, "can't marshal the data", 500)
		return
	}

	wr.Write(b)
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")
	return
}
