package main

import (
	"os"

	s "github.com/kml183/room-environment-monitor/go-server/internal/server"
)

func main() {
	if err := s.NewHTTPServer(); err != nil {
		os.Exit(2)
	}
}
