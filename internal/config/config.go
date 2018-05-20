package config

import (
	"io/ioutil"
)

var (
	rootsPath      = "certs/roots.pem"
	rsaCertPath    = "certs/rsa_cert.pem"
	rsaPrivatePath = "certs/rsa_private.pem"
)

type SSLCerts struct {
	Roots      string
	RsaCert    string
	RSAPrivate string
}

type Bridge struct {
	Host string
	Port string
}

type GoogleIOTConfig struct {
	DeviceID   string
	Bridge     *Bridge
	ProjectID  string
	RegistryID string
	Region     string
}

// events=room-monitor-telemetry
// flag.String("mqtt_host", "mqtt.googleapis.com", "MQTT Bridge Host"),
// flag.String("mqtt_port", "8883", "MQTT Bridge Port"),

func GetGoogleIOTConfig() *GoogleIOTConfig {
	return &GoogleIOTConfig{
		DeviceID: "raspberry-pi-room-monitor-rs256-device",
		Bridge: &Bridge{
			Host: "mqtt.googleapis.com",
			Port: "8883",
		},
		ProjectID:  "personal-website-klutzer",
		RegistryID: "klutzer-devices",
		Region:     "us-central1",
	}
}

func GetSSLCerts() (*SSLCerts, error) {

	rs, err := ioutil.ReadFile(rootsPath)
	if err != nil {
		return nil, err
	}

	rsc, err := ioutil.ReadFile(rsaCertPath)
	if err != nil {
		return nil, err
	}

	rscp, err := ioutil.ReadFile(rsaPrivatePath)
	if err != nil {
		return nil, err
	}

	return &SSLCerts{
		Roots:      string(rs),
		RsaCert:    string(rsc),
		RSAPrivate: string(rscp),
	}, nil
}
