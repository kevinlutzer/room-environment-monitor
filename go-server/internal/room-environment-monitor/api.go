package roomenvironmentmonitor

import (
	"net/http"
)

// Handler is the main http handler for the room environment monitor app
func Handler(r *http.Request, wr http.ResponseWriter) {
	isStub := r.URL.Query().Get("is_stub")
	if isStub == "" {
		setResponse(wr, "is_stub is required", 400)
	}
	setResponse(wr, "Hello World", 200)
	return
}

func setResponse(wr http.ResponseWriter, message string, statusCode int64) {
	wr.Write([]byte(message))
	return
}
