package googleiot

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"encoding/json"
	"flag"
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
	//PublishSensorData fetches sensors data
	PublishSensorData(ctx context.Context, data *sensors.SensorData) error
	//PublishDeviceState fetches sensors data
	PublishDeviceState(ctx context.Context, status *SensorStatus) error
	//SubsribeToConfigChanges subscribe to any config changes
	SubsribeToConfigChanges(ctx context.Context) (*ConfigMessage, error)
}

type topics struct {
	state     string
	telemetry string
	config    string
}

type service struct {
	Logger    *log.Logger
	topics    topics
	certs     *config.SSLCerts
	iotConfig *config.GoogleIOTConfig
	current   []byte
	num       *int
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

func getMQTTClient(certs *config.SSLCerts, iotConfig *config.GoogleIOTConfig, logger *log.Logger, opts *MQTT.ClientOptions) (MQTT.Client, error) {

	clientID := fmt.Sprintf("projects/%v/locations/%v/registries/%v/devices/%v",
		iotConfig.ProjectID,
		iotConfig.Region,
		iotConfig.RegistryID,
		iotConfig.DeviceID,
	)

	jwtString, err := getTokenString(certs.RSAPrivate, iotConfig.ProjectID)
	if err != nil {
		return nil, err
	}

	tlsConfig := getTLSConfig(certs.Roots)

	broker := fmt.Sprintf("ssl://%v:%v", iotConfig.Bridge.Host, iotConfig.Bridge.Port)
	opts.AddBroker(broker)
	opts.SetClientID(clientID).SetTLSConfig(tlsConfig)
	opts.SetUsername("unused")

	opts.SetPassword(jwtString)

	return MQTT.NewClient(opts), nil

}

// NewGoogleIOTService reurns a new service
func NewGoogleIOTService(certs *config.SSLCerts, iotConfig *config.GoogleIOTConfig, logger *log.Logger) Service {

	return &service{
		certs:     certs,
		iotConfig: iotConfig,
		Logger:    logger,
		topics: topics{
			telemetry: fmt.Sprintf("/devices/%v/events", iotConfig.DeviceID),
			state:     fmt.Sprintf("/devices/%v/state", iotConfig.DeviceID),
			config:    fmt.Sprintf("/devices/%v/config", iotConfig.DeviceID),
		},
		num: flag.Int("num", 1, "The number of messages to publish or subscribe (default 1)"),
	}
}

func (s *service) PublishSensorData(ctx context.Context, d *sensors.SensorData) error {

	opts := MQTT.NewClientOptions()

	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		s.Logger.Printf("GoogleIOT - topic > %s\n", msg.Topic())
		s.Logger.Printf("GoogleIOT - payload > %s\n", msg.Payload())
	})

	c, err := getMQTTClient(s.certs, s.iotConfig, s.Logger, opts)
	if err != nil {
		return err
	}

	if token := c.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error().Error())
		return token.Error()
	}

	data, err := json.Marshal(d)
	if err != nil {
		return err
	}

	token := c.Publish(
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

	c.Disconnect(250)
	return nil
}

func (s *service) HandleMQTTConfigMessage(c MQTT.Client, m MQTT.Message) {
	s.Logger.Println("GoogleIOT - Handler config message")
	s.Logger.Println(string(m.Payload()))
	s.current = m.Payload()
}

func (s *service) SubsribeToConfigChanges(ctx context.Context) (*ConfigMessage, error) {

	s.Logger.Println("GoogleIOT - Subscribe to config message")

	receiveCount := 0
	cm := &ConfigMessage{}
	choke := make(chan [2]string)

	opts := MQTT.NewClientOptions()
	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		choke <- [2]string{msg.Topic(), string(msg.Payload())}
	})

	c, err := getMQTTClient(s.certs, s.iotConfig, s.Logger, opts)
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
		s.Logger.Println("GoogleIOT - ERROR: failed to publish the payload")
		return nil, err
	}

	for receiveCount < *s.num {
		incoming := <-choke
		s.Logger.Printf("GoogleIOT - recieved message %s", incoming[1])
		json.Unmarshal([]byte(incoming[1]), &cm)
		receiveCount++
	}

	c.Disconnect(250)
	return cm, nil
}

func (s *service) PublishDeviceState(ctx context.Context, status *SensorStatus) error {

	c, err := getMQTTClient(s.certs, s.iotConfig, s.Logger, MQTT.NewClientOptions())
	if err != nil {
		return err
	}

	if token := c.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error().Error())
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
		s.Logger.Println("GoogleIOT - ERROR: failed to publish the payload")
		return err
	}

	c.Disconnect(250)
	return nil
}
