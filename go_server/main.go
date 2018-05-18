package main

import (
	"net/http"
)

func main() {

}

func setupHTTPServer() {
	http.HandleFunc("/ping", ping)
	if err := http.ListenAndServe(":8080", nil); err != nil {
		panic(err)
	}
}
