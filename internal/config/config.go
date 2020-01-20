package config

import (
	"fmt"
	"io/ioutil"
	"os"
)

type SSLCerts struct {
	Roots      string
	RsaCert    string
	RSAPrivate string
}

type CommandLineArguments struct {
	RootPath       string `json:"root_path"`
	RsaCertPath    string `json:"rsa_cert_path"`
	RsaPrivatePath string `json:"rsa_private_path"`
	DeviceID       string `json:"device_id"`
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

func GetCommandLineArgs() (*CommandLineArguments, error) {
	if len(os.Args) == 1 || len(os.Args) < 5 {
		return nil, fmt.Errorf("Please pass in the ip, device_id, rsa_cert path, rsa_key path, root path in that order")
	}
	return &CommandLineArguments{
		DeviceID:       os.Args[1],
		RsaCertPath:    os.Args[2],
		RsaPrivatePath: os.Args[3],
		RootPath:       os.Args[4],
	}, nil
}

func GetGoogleIOTConfig(deviceID string) (*GoogleIOTConfig, error) {

	return &GoogleIOTConfig{
		DeviceID: deviceID,
		Bridge: &Bridge{
			Host: "mqtt.googleapis.com",
			Port: "8883",
		},
		ProjectID:  "iot-klutzer",
		RegistryID: "devices-klutzer",
		Region:     "us-central1",
	}, nil
}

func GetSSLCerts(cliConfig *CommandLineArguments) (*SSLCerts, error) {

	rs, err := ioutil.ReadFile(cliConfig.RootPath)
	if err != nil {
		return nil, err
	}

	rsc, err := ioutil.ReadFile(cliConfig.RsaCertPath)
	if err != nil {
		return nil, err
	}

	rscp, err := ioutil.ReadFile(cliConfig.RsaPrivatePath)
	if err != nil {
		return nil, err
	}

	return &SSLCerts{
		Roots:      string(rs),
		RsaCert:    string(rsc),
		RSAPrivate: string(rscp),
	}, nil
}
