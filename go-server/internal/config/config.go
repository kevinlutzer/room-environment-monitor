package config

import "io/ioutil"

var (
	rootsPath  = "./certs/roots.pem"
	rsaCert    = "./certs/rsa_cert.pem"
	rsaPrivate = "./certs/rsa_private.pem"
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

	rc, err := ioutil.ReadFile(rsaCert)
	if err != nil {
		return nil, err
	}

	rp, err := ioutil.ReadFile(rsaPrivate)
	if err != nil {
		return nil, err
	}

	return &SSLCerts{
		Roots:      string(rs),
		RsaCert:    string(rc),
		RSAPrivate: string(rp),
	}, nil
}
