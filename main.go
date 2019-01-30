package main

import (
	"context"
	"runtime"

	"github.com/kml183/room-environment-monitor/internal/iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"

	"fmt"

	"github.com/kml183/room-environment-monitor/internal/config"
	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	httpserver "github.com/kml183/room-environment-monitor/internal/http-server"
	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"
)

func main() {

	logger := config.NewLogger()

	if val := runtime.GOMAXPROCS(8); val < -1 {
		fmt.Printf("Could not set the max processes, value recieved > %d \n", val)
	}

	// Initialize Gobot and I2C drivers
	r := raspi.NewAdaptor()
	dl := i2c.NewTSL2561Driver(r)
	// dg := i2c.NewCCS811Driver(r)
	dt := i2c.NewBME280Driver(r)

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
		logger.StdErr.Fatalln(e)
	}

	// Services
	// ss := sensors.NewSensorService(dl, dg, dt)
	ss := sensors.NewSensorService(dl, dt)
	gs := googleiot.NewGoogleIOTService(certs, iotConfig, logger)
	i := iot.NewIOTService(logger, ss, gs)
	hs := httpserver.NewHTTPService(logger, ss, i)

	// Get IP Address
	ip, err := config.GetIPAddress()
	if err != nil {
		e := fmt.Sprintf("Failed to get the ip address > %+s ", err.Error())
		logger.StdErr.Fatalln(e)
	}

	ctx := context.TODO()

	// Publish device status on startup
	err = i.PublishDeviceStatus(ctx)
	if err != nil {
		logger.StdErr.Fatalln(err.Error())
	}

	// Initialize the iot timed functions
	go i.IntializeIOTFunctions(ctx)

	// Asnycronously start server. If gobot stuff hasn't been initialized, some of the server methods will not work.
	go func() {
		err = hs.Start(ip)
		if err != nil {
			logger.StdErr.Fatalln(err.Error())
		}
	}()

	// Start gobot stuff
	iotDevice := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		// []gobot.Device{dl, dt, dg},
		[]gobot.Device{dl, dt},
	)

	iotDevice.Start()
}
