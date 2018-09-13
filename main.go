package main

import (
	"log"
	"os"
	"runtime"

	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"

	s "github.com/kml183/room-environment-monitor/internal/server"
)

func main() {

	logger := log.New(os.Stdout, "http: ", log.LstdFlags)

	if val := runtime.GOMAXPROCS(8); val < -1 {
		logger.Fatalf("Could not set the max processes, value recieved > %d \n", val)
	}

	// Initialize Gobot
	r := raspi.NewAdaptor()
	d := i2c.NewTSL2561Driver(r)

	work := func() {
		if err := s.NewHTTPServer(logger, d); err != nil {
			logger.Fatalf("Could not start the http server > %s \n", err.Error())
		}
	}

	robot := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{d},
		work,
	)

	robot.Start()
}
