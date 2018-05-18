package main

import (
	"net/http"

	"./internal/room_environment_monitor/api"
)

func main() {

}

func setupHTTPServer() {
	http.HandleFunc("/room-environment-data", api.Handle)
	if err := http.ListenAndServe(":8080", nil); err != nil {
		panic(err)
	}
}
