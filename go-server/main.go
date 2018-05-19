package main

import (
	"net/http"
	"os"

	r "github.com/kml183/room_environment_monitor/go_server/internal/room-environment-monitor"
)

func main() {

}

func setupHTTPServer() {
	http.HandleFunc("/room-environment-data", r.Handle)
	if err := http.ListenAndServe(":8080", nil); err != nil {
		os.Exit(2)
	}
}
