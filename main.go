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
	cron "github.com/robfig/cron"
	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"
)

func main() {

	logger := config.NewLogger()

	// Setup max proccesses to be 8
	if val := runtime.GOMAXPROCS(8); val < -1 {
		logger.StdErrFatal("Could not set the max processes, value recieved > %d \n", val)
	}
	logger.StdOut("Successfully setup the amount of go processes")

	// // Initialize Gobot and I2C drivers
	r := raspi.NewAdaptor()
	dl := i2c.NewTSL2561Driver(r)
	dg := i2c.NewCCS811Driver(r)
	dt := i2c.NewBME280Driver(r)
	logger.StdOut("Successfully setup the gobot drivers")

	// Create SSL Certs
	certs, err := config.GetSSLCerts()
	if err != nil {
		logger.StdErrFatal("Failed to fetch the ssl cert files > %s", err.Error())
	}
	logger.StdOut("Successfully fetch the ssl cert files")

	// Fetch Google IOT Config
	iotConfig, err := config.GetGoogleIOTConfig()
	if err != nil {
		logger.StdErrFatal("Failed to get the iot config > %+s ", err.Error())
	}
	logger.StdOut("Successfully created google iot config")

	// Services
	ss := sensors.NewSensorService(dl, dg, dt)
	gs := googleiot.NewGoogleIOTService(certs, iotConfig, logger)
	i := iot.NewIOTService(logger, ss, gs)
	hs := httpserver.NewHTTPService(logger, ss, i)
	logger.StdOut("Successfully setup services")

	// Get IP Address
	ip, err := config.GetIPAddress()
	if err != nil {
		e := fmt.Sprintf("Failed to get the ip address > %+s ", err.Error())
		logger.StdErrFatal(e)
	}
	logger.StdOut("Successfully get ip address")

	ctx := context.TODO()

	// Publish device status on startup
	// err = i.PublishDeviceStatus(ctx)
	// if err != nil {
	// 	logger.StdErrFatal(err.Error())
	// }
	// logger.StdOut("Successfully published the device status initially")

	// Asnycronously start server. If gobot stuff hasn't been initialized, some of the server methods will not work.
	go func() {
		err = hs.Start(ip)
		if err != nil {
			logger.StdErrFatal(err.Error())
		}
	}()

	// Start gobot stuff
	iotDevice := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{dl, dt, dg},
	)

	// Setup Cron
	if err := SetupCRON(ctx, logger, i); err != nil {
		logger.StdErrFatal(err.Error())
	}
	logger.StdOut("Successfully setup the CRON")

	iotDevice.Start()
}

func SetupCRON(ctx context.Context, logger config.LoggerService, i iot.IOTServerService) error {
	c := cron.New()
	if err := c.AddFunc("0 30 * * * *", func() {
		logger.StdOut("Publishing a new data snapshot")
		i.PublishSensorDataSnapshot(ctx)
	}); err != nil {
		return err
	}

	if err := c.AddFunc("@hourly", func() {
		logger.StdOut("Publishing the device status")
		i.PublishDeviceStatus(ctx)
	}); err != nil {
		return err
	}

	if err := c.AddFunc("0 1 * * * *", func() {
		logger.StdOut("Subscribing to configuration changes")
		i.SubscribeToIOTCoreConfig(ctx)
	}); err != nil {
		return err
	}
	c.Start()
	return nil
}
