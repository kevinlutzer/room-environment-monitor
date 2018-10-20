package config

import (
	"errors"
	"fmt"
	"io/ioutil"
	"os"
	"os/exec"
	"strings"
)

var (
	rootsPath      = "/home/pi/certs/roots.pem"
	rsaCertPath    = "/home/pi/certs/rsa_cert.pem"
	rsaPrivatePath = "/home/pi/certs/rsa_private.pem"
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

func GetIPAddress() (string, error) {
	cmd := exec.Command("hostname", "-I")
	val, err := cmd.Output()
	if err != nil {
		return "", errors.New(fmt.Sprintf("Could not execute command with args > %v", cmd.Args))
	}

	if string(val) == "" {
		return "", errors.New("Failed to get hostname")
	}

	filteredVal := strings.Replace(string(val), " \n", "", -1)
	return filteredVal, nil
}

func GetGoogleIOTConfig() (*GoogleIOTConfig, error) {

	h, err := os.Hostname()
	if err != nil {
		return nil, err
	}

	return &GoogleIOTConfig{
		DeviceID: h,
		Bridge: &Bridge{
			Host: "mqtt.googleapis.com",
			Port: "8883",
		},
		ProjectID:  "iot-klutzer",
		RegistryID: "devices-klutzer",
		Region:     "us-central1",
	}, nil
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
