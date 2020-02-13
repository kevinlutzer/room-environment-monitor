package googleiot

import "io/ioutil"

const (
	certPath   = "../certs"
	deviceID   = "prototyping-device"
	host       = "mqtt.googleapis.com"
	port       = "8883"
	projectID  = "iot-klutzer"
	registryID = "devices-klutzer"
	region     = "us-central1"
)

// SSLCerts stores the strings for the certs needed for SSL/TLS
type SSLCerts struct {
	Roots      string
	RSAPrivate string
}

// GetSSLCerts reads the privte public key certs used for SSL/TLS connections
// to google iot core
func GetSSLCerts() (*SSLCerts, error) {
	rs, err := ioutil.ReadFile(certPath + "/roots.pem")
	if err != nil {
		return nil, err
	}

	rscp, err := ioutil.ReadFile(certPath + "/rsa_private.pem")
	if err != nil {
		return nil, err
	}

	return &SSLCerts{
		Roots:      string(rs),
		RSAPrivate: string(rscp),
	}, nil
}
