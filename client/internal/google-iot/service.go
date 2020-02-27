package googleiot

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"encoding/json"
	"fmt"
	"log"
	"time"

	"github.com/dgrijalva/jwt-go"
	MQTT "github.com/eclipse/paho.mqtt.golang"
	"room-environment-monitor-client/internal/logger"
	"room-environment-monitor-client/internal/sensors"
)

type topics struct {
	state     string
	telemetry string
	config    string
}

type service struct {
	logger logger.LoggerService
	topics topics
	certs  *SSLCerts
	client MQTT.Client
}

// NewGoogleIOTService reurns a new service
func NewGoogleIOTService(certs *SSLCerts, logger logger.LoggerService) (Interface, error) {

	c, err := getMQTTClient(certs, MQTT.NewClientOptions())
	if err != nil {
		return nil, err
	}

	if token := c.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error().Error())
		return nil, token.Error()
	}

	return &service{
		client: c,
		certs:  certs,
		logger: logger,
		topics: topics{
			telemetry: fmt.Sprintf("/devices/%v/events", deviceID),
			config:    fmt.Sprintf("/devices/%v/logger", deviceID),
		},
	}, nil
}

func getTokenString(rsaPrivate string, projectID string) (string, error) {
	token := jwt.New(jwt.SigningMethodRS256)

	token.Claims = jwt.StandardClaims{
		Audience:  projectID,
		IssuedAt:  time.Now().Unix(),
		ExpiresAt: time.Now().Add(24 * time.Hour).Unix(),
	}

	key, err := jwt.ParseRSAPrivateKeyFromPEM([]byte(rsaPrivate))
	if err != nil {
		return "", err
	}

	tokenString, err := token.SignedString(key)
	if err != nil {
		return "", err
	}

	return tokenString, err
}

func getTLSConfig(rootsCert string) *tls.Config {
	certpool := x509.NewCertPool()
	certpool.AppendCertsFromPEM([]byte(rootsCert))

	return &tls.Config{
		RootCAs:            certpool,
		ClientAuth:         tls.NoClientCert,
		ClientCAs:          nil,
		InsecureSkipVerify: true,
		Certificates:       []tls.Certificate{},
		MinVersion:         tls.VersionTLS12,
	}
}

func getMQTTClient(certs *SSLCerts, opts *MQTT.ClientOptions) (MQTT.Client, error) {

	clientID := fmt.Sprintf("projects/%v/locations/%v/registries/%v/devices/%v",
		projectID,
		region,
		registryID,
		deviceID,
	)

	jwtString, err := getTokenString(certs.RSAPrivate, projectID)
	if err != nil {
		return nil, err
	}

	tlsConfig := getTLSConfig(certs.Roots)

	broker := fmt.Sprintf("ssl://%v:%v", host, port)
	opts.AddBroker(broker)
	opts.SetClientID(clientID)
	opts.SetTLSConfig(tlsConfig)
	opts.SetAutoReconnect(true)
	opts.SetUsername("unused")
	opts.SetPassword(jwtString)

	return MQTT.NewClient(opts), nil

}

func (s *service) PublishSensorData(ctx context.Context, d *sensors.SensorData) error {
	data, err := json.Marshal(d)
	if err != nil {
		return err
	}

	token := s.client.Publish(
		s.topics.telemetry,
		1,
		false,
		data)

	token.WaitTimeout(5 * time.Second)
	return token.Error()
}

func (s *service) SubsribeToConfigChanges(ctx context.Context) (*ConfigMessage, error) {
	receiveCount := 0
	cm := &ConfigMessage{}
	choke := make(chan [2]string)

	opts := MQTT.NewClientOptions()
	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		choke <- [2]string{msg.Topic(), string(msg.Payload())}
	})

	c, err := getMQTTClient(s.certs, opts)
	if err != nil {
		return nil, err
	}

	if token := c.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error().Error())
		return nil, token.Error()
	}

	token := c.Subscribe(
		s.topics.config,
		0,
		nil)

	token.WaitTimeout(5 * time.Second)
	err = token.Error()
	if err != nil {
		s.logger.StdErr("failed to publish the payload\n")
		return nil, err
	}

	for receiveCount < 1 {
		incoming := <-choke
		json.Unmarshal([]byte(incoming[1]), &cm)
		receiveCount++
	}

	c.Disconnect(250)
	return cm, nil
}

func (s *service) PublishDeviceState(ctx context.Context, status *DeviceStatus) error {
	c, err := getMQTTClient(s.certs, MQTT.NewClientOptions())
	if err != nil {
		s.logger.StdErr("Failed to get MQTT client, > %v", err)
		return err
	}

	s.logger.StdOut("About to connect to the server")
	if token := c.Connect(); token.Wait() && token.Error() != nil {
		s.logger.StdErr("Error stuff erorr > %v", token.Error())
		return token.Error()
	}

	data, err := json.Marshal(status)
	if err != nil {
		return err
	}

	token := c.Publish(
		s.topics.state,
		0,
		false,
		data)

	token.WaitTimeout(5 * time.Second)
	err = token.Error()
	if err != nil {
		s.logger.StdErr("failed to publish the payload\n")
		return err
	}

	c.Disconnect(250)
	return nil
}
