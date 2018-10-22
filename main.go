package main

import (
	"context"
	"log"
	"os"
	"runtime"
	"time"

	"fmt"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	httpserver "github.com/kml183/room-environment-monitor/internal/http-server"
	"github.com/kml183/room-environment-monitor/internal/sensors"
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
			e := fmt.Sprintf("Failed to get the iot config > %+s ", err.Error())
			logger.Fatalln(e)
		}

		// Services
		ss := sensors.NewSensorService(dl, dg, dt)
		gs := googleiot.NewGoogleIOTService(certs, iotConfig, logger)

		// Get IP Address
		ip, err := config.GetIPAddress()
		if err != nil {
			e := fmt.Sprintf("Failed to get the ip address > %+s ", err.Error())
			logger.Fatalln(e)
		}

		ctx := context.TODO()

		if err := gs.PublishDeviceState(ctx, &googleiot.DeviceStatus{Status: googleiot.Active}); err != nil {
			e := fmt.Errorf("Failed to publish active state for the device > %+s ", err.Error())
			logger.Fatalln(e)
		}

		s := httpserver.NewHTTPService(logger, ss, gs, ip)

		//Blocking start on the server.
		s.Start()
	}

	var t time.Duration
	t = 1

	timedHandler := func() {
		gobot.Every(t*time.Second, func() {
			fmt.Printf("TIMED PROCESS AHHHHHHHHHHHH, %+v\n", t)
			t = t + 1
		})
	}

	robot := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{dl, dt, dg},
		work,
		timedHandler,
	)
	robot.Start()
}
