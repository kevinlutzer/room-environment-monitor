package main

import (
	"os"

	r "github.com/kml183/room-environment-monitor/go-server/internal/room-environment-monitor"
)

func main() {
	if err := r.NewHTTPServer(); err != nil {
		os.Exit(2)
	}
}
