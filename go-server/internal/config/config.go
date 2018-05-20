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
