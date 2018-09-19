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
	state     string
	telemetry string
}

type service struct {
	Certs           *config.SSLCerts
	GoogleIOTConfig *config.GoogleIOTConfig
	Logger          *log.Logger
	topics          topics
	clientID        string
	clientOptions   *MQTT.ClientOptions
	jwtString       string
	tlsConfig       *tls.Config
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

func NewGoogleIOTService(certs *config.SSLCerts, iotConfig *config.GoogleIOTConfig, logger *log.Logger) (Service, error) {

	jwtString, err := getTokenString(certs.RSAPrivate, iotConfig.ProjectID)
	if err != nil {
		return nil, err
	}

	tlsConfig := getTLSConfig(certs.Roots)

	return &service{
		Certs:           certs,
		GoogleIOTConfig: iotConfig,
		Logger:          logger,
		topics: topics{
			telemetry: fmt.Sprintf("/devices/%v/events", iotConfig.DeviceID),
			state:     fmt.Sprintf("/devices/%v/state", iotConfig.DeviceID),
		},
		clientID: fmt.Sprintf("projects/%v/locations/%v/registries/%v/devices/%v",
			iotConfig.ProjectID,
			iotConfig.Region,
			iotConfig.RegistryID,
			iotConfig.DeviceID,
		),
		jwtString: jwtString,
		tlsConfig: tlsConfig,
	}, nil
}

func (s *service) getMQTTClient() MQTT.Client {

	opts := MQTT.NewClientOptions()

	broker := fmt.Sprintf("ssl://%v:%v", s.GoogleIOTConfig.Bridge.Host, s.GoogleIOTConfig.Bridge.Port)
	opts.AddBroker(broker)
	opts.SetClientID(s.clientID).SetTLSConfig(s.tlsConfig)
	opts.SetUsername("unused")

	opts.SetPassword(s.jwtString)
	opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
		s.Logger.Printf("GoogleIOT - topic > %s\n", msg.Topic())
		s.Logger.Printf("GoogleIOT - payload > %s\n", msg.Payload())
	})

	return MQTT.NewClient(opts)

}

func (s *service) PublishSensorData(ctx context.Context, data string) error {

	client := s.getMQTTClient()

	if token := client.Connect(); token.Wait() && token.Error() != nil {
		log.Fatal(token.Error())
	}

	token := client.Publish(
		s.topics.telemetry,
		0,
		false,
		data)

	token.WaitTimeout(5 * time.Second)
	err := token.Error()
	if err != nil {
		s.Logger.Println("GoogleIOT - ERROR: failed to publish the payload")
		return err
	}

	client.Disconnect(250)
	return nil
}

// func (s *service) PublishDeviceState(ctx context.Context, status SensorStatus) error {

// 	str, err := json.Marshal(status)
// 	if err != nil {
// 		return err
// 	}

// 	opts, err := s.getMQTTOptions(s.clientID, tlsConfig)
// 	if err != nil {
// 		return err
// 	}

// 	client := MQTT.NewClient(opts)
// 	if token := client.Connect(); token.Wait() && token.Error() != nil {
// 		log.Fatal(token.Error())
// 	}

// 	token := client.Publish(
// 		s.topics.telemetry,
// 		0,
// 		false,
// 		data)

// 	token.WaitTimeout(5 * time.Second)
// 	err = token.Error()
// 	if err != nil {
// 		s.Logger.Println("GoogleIOT - ERROR: failed to publish the payload")
// 		return err
// 	}

// 	client.Disconnect(250)
// 	return nil
// }
