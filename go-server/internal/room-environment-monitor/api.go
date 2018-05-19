package roomenvironmentmonitor

import (
	"errors"
	"fmt"
	"net/http"
)

const (
	// HTTPPort is the port to run the server on
	HTTPPort = ":8080"
)

type server struct{}

// NewHTTPServer returns a instance of the http server
func NewHTTPServer() error {
	s := server{}

	//Setup Handlers
	http.HandleFunc("/", s.GetSnapshotHandler)

	fmt.Printf("Started HTTP handler on port %s", HTTPPort)
	if err := http.ListenAndServe(HTTPPort, nil); err != nil {
		return errors.New("Couldn't start http server")
	}

	return nil
}

// Handler is the main http handler for the room environment monitor app
func (s *server) GetSnapshotHandler(wr http.ResponseWriter, r *http.Request) {
	isStub := r.URL.Query().Get("is_stub")
	if isStub == "" {
		s.setResponse(wr, "is_stub is required", 400)
		return
	}
	s.setResponse(wr, "Hello World", 200)
	return
}

func (s *server) setResponse(wr http.ResponseWriter, message string, statusCode int64) {
	wr.Write([]byte(message))
	return
}
