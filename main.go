package main

import (
	"fmt"
	"os"

	s "github.com/kml183/room-environment-monitor/internal/server"
)

func main() {
	if err := s.NewHTTPServer(); err != nil {
		fmt.Printf("Error starting server > %s", err.Error())
		os.Exit(2)
	}
}
