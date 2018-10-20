package main

import (
	"context"
	"log"
	"os"
	"runtime"

	"fmt"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	server "github.com/kml183/room-environment-monitor/internal/server"
	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"
)

func main() {
	logger := log.New(os.Stdout, "http: ", log.LstdFlags)

	if val := runtime.GOMAXPROCS(8); val < -1 {
		fmt.Printf("Could not set the max processes, value recieved > %d \n", val)
	}

	// Initialize Gobot and I2C drivers
	r := raspi.NewAdaptor()
	dl := i2c.NewTSL2561Driver(r)
	dg := i2c.NewCCS811Driver(r)
	dt := i2c.NewBME280Driver(r)

	work := func() {
		// Create SSL Certs
		certs, err := config.GetSSLCerts()
		if err != nil {
			s, _ := fmt.Printf("Failed to fetch the ssl cert files > %+s ", err.Error())
			panic(s)
		}

		// Fetch Google IOT Config
		iotConfig, err := config.GetGoogleIOTConfig()
		if err != nil {
			s, _ := fmt.Printf("Failed to get the iot config > %+s ", err.Error())
			panic(s)
		}

		// Services
		ss := sensors.NewSensorService(dl, dg, dt)
		gs := googleiot.NewGoogleIOTService(certs, iotConfig, logger)

		// Get IP Address
		ip, err := config.GetIPAddress()
		if err != nil {
			s, _ := fmt.Printf("Failed to get the ip address > %+s ", err.Error())
			panic(s)
		}

		ctx := context.TODO()

		if err := gs.PublishDeviceState(ctx, &googleiot.DeviceStatus{Status: googleiot.Active}); err != nil {
			s, _ := fmt.Printf("Failed to publish active state for the device > %+s ", err.Error())
			panic(s)
		}

		if err := server.StartHTTPServer(logger, ss, gs, ip); err != nil {
			if err := gs.PublishDeviceState(ctx, &googleiot.DeviceStatus{Status: googleiot.Unactive}); err != nil {
				s, _ := fmt.Printf("Failed to publish unactive state for the device > %+s ", err.Error())
				panic(s)
			}

			s, _ := fmt.Printf("Could not start the http server > %s \n", err.Error())
			panic(s)
		}
	}

	robot := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{dl, dt, dg},
		work,
	)

	robot.Start()
}
