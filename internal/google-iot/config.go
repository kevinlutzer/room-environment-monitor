package googleiot

import "io/ioutil"

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

type SSLCerts struct {
	Roots      string
	RsaCert    string
	RSAPrivate string
}


func GetSSLCerts(rootPath, rsaCertPath, rsaPrivatePath string) (*SSLCerts, error) {

	rs, err := ioutil.ReadFile(rootPath)
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
