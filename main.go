package main

import (
	"fmt"
	"os"
	"runtime"

	s "github.com/kml183/room-environment-monitor/internal/server"
)

func main() {

	if val := runtime.GOMAXPROCS(8); val < -1 {
		os.Exit(4)
	}

	if err := s.NewHTTPServer(); err != nil {
		fmt.Printf("Error starting server > %s", err.Error())
		os.Exit(2)
	}
}
