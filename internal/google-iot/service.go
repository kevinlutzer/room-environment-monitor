package googleiot

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"fmt"
	"log"
	"time"

	jwt "github.com/dgrijalva/jwt-go"
	MQTT "github.com/eclipse/paho.mqtt.golang"
	"github.com/kml183/room-environment-monitor/internal/config"

	"github.com/kml183/room-environment-monitor/internal/sensors"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data string) error
}

type service struct {
	SensorsService  sensors.Service
	Certs           *config.SSLCerts
	GoogleIOTConfig *config.GoogleIOTConfig
}

func NewGoogleIOTService(certs *config.SSLCerts, iotConfig *config.GoogleIOTConfig) Service {
	return &service{
		Certs:           certs,
		GoogleIOTConfig: iotConfig,
	}
}

func (s *service) getTokenString() (string, error) {
	token := jwt.New(jwt.SigningMethodRS256)

	token.Claims = jwt.StandardClaims{
		Audience:  s.GoogleIOTConfig.ProjectID,
		IssuedAt:  time.Now().Unix(),
		ExpiresAt: time.Now().Add(24 * time.Hour).Unix(),
	}

	key, err := jwt.ParseRSAPrivateKeyFromPEM([]byte(s.Certs.RSAPrivate))
	if err != nil {
		return "", err
	}

	tokenString, err := token.SignedString(key)
	if err != nil {
		return "", err
	}

	return tokenString, nil
}

func (s *service) getMQTTOptions(clientID string, tls *tls.Config) (*MQTT.ClientOptions, error) {
	opts := MQTT.NewClientOptions()

	broker := fmt.Sprintf("ssl://%v:%v", s.GoogleIOTConfig.Bridge.Host, s.GoogleIOTConfig.Bridge.Port)
	fmt.Printf("[main] Broker '%v' \n", broker)

	opts.AddBroker(broker)
	opts.SetClientID(clientID).SetTLSConfig(tls)

	opts.SetUsername("unused")

	tokenString, err := s.getTokenString()
	if err != nil {
		return nil, err
	}
	fmt.Printf("token > '%v'", tokenString)
	opts.SetPassword(tokenString)

	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		fmt.Printf("[handler] Topic: %v\n", msg.Topic())
		fmt.Printf("[handler] Payload: %v\n", msg.Payload())
	})

	return opts, nil
}

func (s *service) PublishSensorData(ctx context.Context, data string) error {

	certpool := x509.NewCertPool()
	certpool.AppendCertsFromPEM([]byte(s.Certs.Roots))

	fmt.Println("Creating TLS Config")

	tlsConfig := &tls.Config{
		RootCAs:            certpool,
		ClientAuth:         tls.NoClientCert,
		ClientCAs:          nil,
		InsecureSkipVerify: true,
		Certificates:       []tls.Certificate{},
		MinVersion:         tls.VersionTLS12,
	}

	clientID := fmt.Sprintf("projects/%v/locations/%v/registries/%v/devices/%v",
		"personal-website-klutzer",
		"us-central1",
		"klutzer-devices",
		"raspberry-pi-room-monitor-rs256-device",
	)
	fmt.Printf("Client id > %s\n", clientID)

	opts, err := s.getMQTTOptions(clientID, tlsConfig)
	if err != nil {
		return err
	}

	client := MQTT.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}

	topic := struct {
		config    string
		telemetry string
	}{
		config:    fmt.Sprintf("/devices/%v/config", s.GoogleIOTConfig.DeviceID),
		telemetry: fmt.Sprintf("/devices/%v/events", s.GoogleIOTConfig.DeviceID),
	}

	token := client.Publish(
		topic.telemetry,
		0,
		false,
		data)

	token.WaitTimeout(5 * time.Second)

	err = token.Error()
	if err != nil {
		fmt.Printf("Error with publishing ---> %s \n", token.Error().Error())
		return err
	}

	client.Disconnect(250)
	return nil
}
