package main

import (
	"log"
	"os"
	"runtime"

	"fmt"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	server "github.com/kml183/room-environment-monitor/internal/server"
	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/gpio"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"
)

const (
	FanPin = "15"
)

func configMessageHandler(msg *googleiot.ConfigMessage) {

}

func main() {
	logger := log.New(os.Stdout, "http: ", log.LstdFlags)

	if val := runtime.GOMAXPROCS(1); val < -1 {
		fmt.Printf("Could not set the max processes, value recieved > %d \n", val)
	}

	// Initialize Gobot and I2C drivers
	r := raspi.NewAdaptor()
	dl := i2c.NewTSL2561Driver(r)
	dg := i2c.NewCCS811Driver(r)
	fd := gpio.NewLedDriver(r, FanPin)

	work := func() {
		// Setup cpu-fan control system

		// Create SSL Certs
		certs, err := config.GetSSLCerts()
		if err != nil {
			fmt.Printf("Failed to fetch the ssl cert files > %s ", err.Error())
		}

		// Fetch Google IOT Config
		iotConfig := config.GetGoogleIOTConfig()

		// Services
		ss := sensors.NewSensorService(dl, dg, fd)
		gs := googleiot.NewGoogleIOTService(certs, iotConfig, logger)

		// ctx := context.TODO()

		// gobot.Every(5*time.Second, func() {
		// 	gs.SubsribeToConfigChanges(ctx)
		// })

		if err := server.StartHTTPServer(logger, ss, gs); err != nil {
			fmt.Printf("Could not start the http server > %s \n", err.Error())
		}
	}

	robot := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{dl, dg, fd},
		work,
	)

	robot.Start()
}
