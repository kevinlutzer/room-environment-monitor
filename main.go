package main

import (
	"log"
	"os"
	"runtime"

	s "github.com/kml183/room-environment-monitor/internal/server"
)

func main() {

	logger := log.New(os.Stdout, "http: ", log.LstdFlags)

	// Get OS To Setup stub
	var stub bool
	if runtime.GOOS == "darwin" {
		stub = true
	} else {
		stub = false
	}

	if val := runtime.GOMAXPROCS(8); val < -1 {
		logger.Fatalf("Could not set the max processes, value recieved > %d \n", val)
	}

	if err := s.NewHTTPServer(logger, stub); err != nil {
		logger.Fatalf("Could not start the http server > %s \n", err.Error())
	}
}
