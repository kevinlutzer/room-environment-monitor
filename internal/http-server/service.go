package httpserver

import (
	"encoding/json"
	"fmt"
	"log"
	"net/http"

	"github.com/kml183/room-environment-monitor/internal/iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
)

type Message struct {
	Message string `json:"message"`
}

type DataMessage struct {
	Message string              `json:"message"`
	Data    *sensors.SensorData `json:"data"`
}

type Service interface {
	Start(ip string) error
}

type httpService struct {
	iot     iot.Service
	sensors sensors.Service
	logger  *log.Logger
	mux     *http.ServeMux
}

// NewHTTPService returns a instance of the http service
func NewHTTPService(logger *log.Logger, sensors sensors.Service, iot iot.Service) Service {

	s := &httpService{
		iot:     iot,
		sensors: sensors,
		logger:  logger,
	}

	//Setup Handlers
	mux := http.NewServeMux()
	mux.HandleFunc("/get-sensor-data-snapshot", s.getSensorDataSnapshotHandler)
	mux.HandleFunc("/publish-sensor-data-snapshot", s.publishSensorDataSnapshotHandler)
	mux.HandleFunc("/publish-device-status", s.publishDeviceStatusHandler)
	mux.HandleFunc("/subscribe-iot-config", s.subscribeToIOTCoreConfigHandler)

	s.mux = mux
	return s
}

func (s *httpService) Start(ip string) error {
	//Start the http server (blocking)
	port := fmt.Sprintf("%+s:8080", ip)
	fmt.Printf("Started HTTP handler on port %+s\n", port)
	if err := http.ListenAndServe(port, s.mux); err != nil {
		return err
	}
	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *httpService) getSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.Println("http: calling handler getSensorDataSnapshotHandler")

	d, err := s.sensors.FetchSensorData(ctx)
	if err != nil {
		s.logger.Println("http: ", err.Error())
		s.setStringResponse(wr, "could't fetch the sensor data", 500)
		return
	}

	s.setDataResponse(wr, d, 200)
}

func (s *httpService) publishSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.Println("http: calling handler publishSensorDataSnapshotHandler")

	err := s.iot.PublishSensorDataSnapshotHandler(ctx)
	if err != nil {
		s.logger.Println("http: ", err.Error())
		s.setStringResponse(wr, "could't publish the sensor data", 500)
		return
	}

	s.setStringResponse(wr, "successfully published the data", 200)

}

func (s *httpService) publishDeviceStatusHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.Println("http: calling handler publishDeviceStatusHandler")

	err := s.iot.PublishDeviceStatus(ctx)
	if err != nil {
		s.logger.Println("http: ", err.Error())
		s.setStringResponse(wr, "could't publish the status", 500)
		return
	}

	s.setStringResponse(wr, "published the status", 200)
}

func (s *httpService) subscribeToIOTCoreConfigHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.Println("http: calling handler subscribeToIOTCoreConfigHandler")

	err := s.iot.SubscribeToIOTCoreConfig(ctx)
	if err != nil {
		s.logger.Println("http: ", err.Error())
		s.setStringResponse(wr, "could't subscribe to the iot config", 500)
		return
	}

	s.setStringResponse(wr, "subscribed to the iot config", 200)
}

func (s *httpService) setStringResponse(wr http.ResponseWriter, message string, statusCode int) {
	msg := &Message{Message: message}
	b, _ := json.Marshal(msg)

	wr.Write(b)
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")
	return
}

func (s *httpService) setDataResponse(wr http.ResponseWriter, data *sensors.SensorData, statusCode int) {

	msg := &DataMessage{
		Message: "successfuly fetched data from the sensors",
		Data:    data,
	}

	b, err := json.Marshal(msg)
	if err != nil {
		s.logger.Printf("Request - ERROR: failed to marshal the data %s", err.Error())
		s.setStringResponse(wr, "can't marshal the data", 500)
		return
	}

	wr.Write(b)
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")
	return
}
