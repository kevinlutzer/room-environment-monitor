package config

import (
	"io/ioutil"
)

var (
	rootsPath      = "/home/pi/certs/roots.pem"
	rsaCertPath    = "/home/pi/certs/rsa_cert.pem"
	rsaPrivatePath = "/home/pi/certs/rsa_private.pem"

	// APIKeyFile is the name of the file containing an string to be dynamically loaded when the server starts
	APIKeyFile = "/home/pi/api_key.txt"
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

func GetGoogleIOTConfig() *GoogleIOTConfig {
	return &GoogleIOTConfig{
		DeviceID: "room-monitor-rs256-device",
		Bridge: &Bridge{
			Host: "mqtt.googleapis.com",
			Port: "8883",
		},
		ProjectID:  "iot-klutzer",
		RegistryID: "devices-klutzer",
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
