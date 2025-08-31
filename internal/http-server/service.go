package httpserver

import (
	"encoding/json"
	"fmt"
	"net"
	"net/http"

	"github.com/kml183/room-environment-monitor/internal/config"
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

type MacAddressMessage struct {
	Message    string `json:"message"`
	MacAddress string `json:"mac_address"`
}

type HttpServerService interface {
	Start(ip string) error
}

type httpService struct {
	iot     iot.IOTServerService
	sensors sensors.SensorsService
	logger  config.LoggerService
	mux     *http.ServeMux
}

// NewHTTPService returns a instance of the http service
func NewHTTPService(logger config.LoggerService, sensors sensors.SensorsService, iot iot.IOTServerService) HttpServerService {

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
	mux.HandleFunc("/get-mac-address", s.getMacAddressHandler)

	s.mux = mux
	return s
}

func (s *httpService) Start(ip string) error {
	//Start the http server (blocking)
	port := fmt.Sprintf("%s:8080", ip)
	fmt.Printf("Started HTTP handler on port %s\n", port)
	if err := http.ListenAndServe(port, s.mux); err != nil {
		return err
	}
	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *httpService) getSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.StdOut("http: calling handler getSensorDataSnapshotHandler\n")

	d, err := s.sensors.FetchSensorData(ctx)
	if err != nil {
		s.logger.StdErr("http: %v\n", err.Error())
		s.setStringResponse(wr, "could't fetch the sensor data", 500)
		return
	}

	s.setDataResponse(wr, d, 200)
}

func (s *httpService) publishSensorDataSnapshotHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.StdOut("http: calling handler publishSensorDataSnapshotHandler\n")

	err := s.iot.PublishSensorDataSnapshot(ctx)
	if err != nil {
		s.logger.StdErr("http: %v\n", err.Error())
		s.setStringResponse(wr, "could't publish the sensor data", 500)
		return
	}

	s.setStringResponse(wr, "successfully published the data", 200)

}

func (s *httpService) publishDeviceStatusHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.StdOut("http: calling handler publishDeviceStatusHandler\n")

	err := s.iot.PublishDeviceStatus(ctx)
	if err != nil {
		s.logger.StdErr("http: %v\n", err.Error())
		s.setStringResponse(wr, "could't publish the status", 500)
		return
	}

	s.setStringResponse(wr, "published the status", 200)
}

func (s *httpService) subscribeToIOTCoreConfigHandler(wr http.ResponseWriter, r *http.Request) {
	ctx := r.Context()
	s.logger.StdOut("http: calling handler subscribeToIOTCoreConfigHandler\n")

	err := s.iot.SubscribeToIOTCoreConfig(ctx)
	if err != nil {
		s.logger.StdErr("http: %v\n", err.Error())
		s.setStringResponse(wr, "could't subscribe to the iot config", 500)
		return
	}

	s.setStringResponse(wr, "subscribed to the iot config", 200)
}

func (s *httpService) getMacAddressHandler(wr http.ResponseWriter, r *http.Request) {
	s.logger.StdOut("http: calling handler getMacAddressHandler\n")

	macAddress, err := getEth0MacAddress()
	if err != nil {
		s.logger.StdErr("http: %v\n", err.Error())
		s.setStringResponse(wr, "could't get MAC address for eth0", 500)
		return
	}

	s.setMacAddressResponse(wr, macAddress, 200)
}

// getEth0MacAddress returns the MAC address of the eth0 interface
func getEth0MacAddress() (string, error) {
	iface, err := net.InterfaceByName("eth0")
	if err != nil {
		return "", fmt.Errorf("failed to get eth0 interface: %w", err)
	}

	if iface.HardwareAddr == nil {
		return "", fmt.Errorf("eth0 interface has no hardware address")
	}

	return iface.HardwareAddr.String(), nil
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
		s.logger.StdErr("Request -  failed to marshal the data %v\n", err.Error())
		s.setStringResponse(wr, "can't marshal the data", 500)
		return
	}

	wr.Write(b)
	wr.WriteHeader(statusCode)
	wr.Header().Set("Content-Type", "application/json")
	return
}

func (s *httpService) setMacAddressResponse(wr http.ResponseWriter, macAddress string, statusCode int) {
	msg := &MacAddressMessage{
		Message:    "successfully fetched MAC address for eth0",
		MacAddress: macAddress,
	}

	b, err := json.Marshal(msg)
	if err != nil {
		s.logger.StdErr("Request - failed to marshal the MAC address data %v\n", err.Error())
		s.setStringResponse(wr, "can't marshal the MAC address data", 500)
		return
	}

	wr.Header().Set("Content-Type", "application/json")
	wr.WriteHeader(statusCode)
	wr.Write(b)
	return
}
