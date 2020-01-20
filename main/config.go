package main

import (
	"fmt"
	"os"
)

type CommandLineArguments struct {
	RootPath       string `json:"root_path"`
	RsaCertPath    string `json:"rsa_cert_path"`
	RsaPrivatePath string `json:"rsa_private_path"`
	DeviceID       string `json:"device_id"`
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



