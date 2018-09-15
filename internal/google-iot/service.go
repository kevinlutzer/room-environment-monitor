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
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data string) error
}

type topics struct {
	config    string
	telemetry string
}

type service struct {
	Certs           *config.SSLCerts
	GoogleIOTConfig *config.GoogleIOTConfig
	Logger          *log.Logger
	topics          topics
	clientOptions   *MQTT.ClientOptions
}

func NewGoogleIOTService(certs *config.SSLCerts, iotConfig *config.GoogleIOTConfig, logger *log.Logger) Service {
	return &service{
		Certs:           certs,
		GoogleIOTConfig: iotConfig,
		Logger:          logger,
		topics: topics{
			telemetry: fmt.Sprintf("/devices/%v/events", iotConfig.DeviceID),
			config:    fmt.Sprintf("/devices/%v/config", iotConfig.DeviceID),
		},
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
	opts.AddBroker(broker)
	opts.SetClientID(clientID).SetTLSConfig(tls)
	opts.SetUsername("unused")

	tokenString, err := s.getTokenString()
	if err != nil {
		return nil, err
	}

	opts.SetPassword(tokenString)
	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		s.Logger.Printf("GoogleIOT - topic > %s\n", msg.Topic())
		s.Logger.Printf("GoogleIOT - payload > %s\n", msg.Payload())
	})

	return opts, nil
}

func (s *service) PublishSensorData(ctx context.Context, data string) error {

	certpool := x509.NewCertPool()
	certpool.AppendCertsFromPEM([]byte(s.Certs.Roots))

	tlsConfig := &tls.Config{
		RootCAs:            certpool,
		ClientAuth:         tls.NoClientCert,
		ClientCAs:          nil,
		InsecureSkipVerify: true,
		Certificates:       []tls.Certificate{},
		MinVersion:         tls.VersionTLS12,
	}

	clientID := fmt.Sprintf("projects/%v/locations/%v/registries/%v/devices/%v",
		"room-env-monitor-klutzer",
		"us-central1",
		"devices-klutzer",
		"raspberry-pi-room-monitor-rs256-device",
	)

	opts, err := s.getMQTTOptions(clientID, tlsConfig)
	if err != nil {
		return err
	}

	client := MQTT.NewClient(opts)
	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}

	token := client.Publish(
		s.topics.telemetry,
		0,
		false,
		data)

	token.WaitTimeout(5 * time.Second)
	err = token.Error()
	if err != nil {
		s.Logger.Println("GoogleIOT - ERROR: failed to publish the payload")
		return err
	}

	client.Disconnect(250)
	return nil
}
