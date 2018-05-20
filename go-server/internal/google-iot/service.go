package googleiot

import (
	"context"
	"crypto/tls"
	"crypto/x509"
	"fmt"

	"github.com/kml183/room-environment-monitor/go-server/internal/config"

	"github.com/kml183/room-environment-monitor/go-server/internal/sensors"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	PublishSensorData(ctx context.Context) error
}

type service struct {
	SensorsService sensors.Service
	Certs          *config.SSLCerts
}

func NewGoogleIOTService(ss sensors.Service, certs *config.SSLCerts) Service {
	return &service{
		SensorsService: ss,
		Certs:          certs,
	}
}

func (s *service) PublishSensorData(ctx context.Context) error {

	certpool := x509.NewCertPool()
	certpool.AppendCertsFromPEM([]byte(s.Certs.Roots))

	fmt.Println("Creating TLS Config")

	config := &tls.Config{
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

	// fmt.Println("Creating MQTT Client Options")
	opts := MQTT.NewClientOptions()

	// broker := fmt.Sprintf("ssl://%v:%v", *bridge.host, *bridge.port)
	// log.Printf("[main] Broker '%v'", broker)

	// opts.AddBroker(broker)
	// opts.SetClientID(clientID).SetTLSConfig(config)

	// opts.SetUsername("unused")

	token := jwt.New(jwt.SigningMethodRS256)
	fmt.Printf("%s, %s, %s, %s", certpool, clientID, opts, token)
	// token.Claims = jwt.StandardClaims{
	// 	Audience:  *projectID,
	// 	IssuedAt:  time.Now().Unix(),
	// 	ExpiresAt: time.Now().Add(24 * time.Hour).Unix(),
	// }

	// log.Println("[main] Load Private Key")
	// keyBytes, err := ioutil.ReadFile(*privateKey)
	// if err != nil {
	// 	log.Fatal(err)
	// }

	// log.Println("[main] Parse Private Key")
	// key, err := jwt.ParseRSAPrivateKeyFromPEM(keyBytes)
	// if err != nil {
	// 	log.Fatal(err)
	// }

	// log.Println("[main] Sign String")
	// tokenString, err := token.SignedString(key)
	// if err != nil {
	// 	log.Fatal(err)
	// }

	// opts.SetPassword(tokenString)

	// // Incoming
	// opts.SetDefaultPublishHandler(func(client MQTT.Client, msg MQTT.Message) {
	// 	fmt.Printf("[handler] Topic: %v\n", msg.Topic())
	// 	fmt.Printf("[handler] Payload: %v\n", msg.Payload())
	// })

	// log.Println("[main] MQTT Client Connecting")
	// client := MQTT.NewClient(opts)
	// if token := client.Connect(); token.Wait() && token.Error() != nil {
	// 	log.Fatal(token.Error())
	// }

	// topic := struct {
	// 	config    string
	// 	telemetry string
	// }{
	// 	config:    fmt.Sprintf("/devices/%v/config", *deviceID),
	// 	telemetry: fmt.Sprintf("/devices/%v/events", *deviceID),
	// }

	// log.Println("[main] Creating Subscription")
	// client.Subscribe(topic.config, 0, nil)

	// log.Println("[main] Publishing Messages")
	// for i := 0; i < 10; i++ {
	// 	log.Printf("[main] Publishing Message #%d", i)
	// 	token := client.Publish(
	// 		topic.telemetry,
	// 		0,
	// 		false,
	// 		fmt.Sprintf("Message: %d", i))
	// 	token.WaitTimeout(5 * time.Second)
	// }

	// log.Println("[main] MQTT Client Disconnecting")
	// client.Disconnect(250)

	// log.Println("[main] Done")
}
