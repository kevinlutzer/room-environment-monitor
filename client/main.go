package main

import (
	"context"
	"os"
	"runtime"

	"room-environment-monitor-client/internal/iot"
	"room-environment-monitor-client/internal/sensors"
	httpserver "room-environment-monitor-client/internal/server"

	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"

	googleiot "room-environment-monitor-client/internal/google-iot"
	"room-environment-monitor-client/internal/logger"

	cron "gopkg.in/robfig/cron.v2"
)

const (
	localDev  = "LOCAL_DEV"
	defaultIP = "localhost"
)

func main() {
	logger := logger.NewLogger()

	// Setup max proccesses to be 8
	if val := runtime.GOMAXPROCS(8); val < -1 {
		logger.StdErrFatal("Could not set the max processes, value recieved > %d \n", val)
		os.Exit(failedToSetGoProcesses)
	}
	logger.StdOut("Successfully setup the amount of go processes")

	// Services
	ss := sensors.NewSensorServiceStub()
	if os.Getenv(localDev) != "true" {
		ss = setupSensors(logger)
		logger.StdOut("Use live of sensor config")
	}

	gs := setupIOTService(logger)
	iotService := iot.NewIOTService(logger, ss, gs)

	hs := httpserver.NewHTTPService(logger, iotService)
	logger.StdOut("Successfully setup services")

	ctx := context.TODO()

	// Setup Cron
	if err := setupCRON(ctx, logger, iotService); err != nil {
		logger.StdErrFatal(err.Error())
		os.Exit(failedToSetupCRON)
	}
	logger.StdOut("Successfully setup the CRON")

	// Subscribe to Configuration Changes
	if err := iotService.SubscribeToIOTCoreConfig(ctx); err != nil {
		logger.StdErrFatal(err.Error())
		os.Exit(failedToSetupCRON)
	}

	if err := hs.Start(defaultIP); err != nil {
		logger.StdErrFatal(err.Error())
		os.Exit(failedToStartHTTPService)
	}
}

func setupSensors(logger logger.LoggerService) sensors.Interface {
	// Initialize Gobot and I2C drivers
	r := raspi.NewAdaptor()
	dl := i2c.NewTSL2561Driver(r)
	dg := i2c.NewCCS811Driver(r)
	dt := i2c.NewBME280Driver(r)
	logger.StdOut("Successfully setup the gobot drivers")

	// Start gobot stuff
	iotDevice := gobot.NewRobot("room-environment-monitor",
		[]gobot.Connection{r},
		[]gobot.Device{dl, dt, dg},
	)
	go func() {
		if err := iotDevice.Start(); err != nil {
			logger.StdErr("Failed to start iot device: %s", err.Error())
			os.Exit(failedToStartGoBotService)
		}
	}()

	return sensors.NewSensorService(dl, dg, dt)
}

func setupIOTService(logger logger.LoggerService) googleiot.Interface {

	// Create SSL Certs
	certs, err := googleiot.GetSSLCerts()
	if err != nil {
		logger.StdErrFatal("Failed to fetch the ssl cert files > %s", err.Error())
		os.Exit(failedToGetSSLCerts)
	}
	logger.StdOut("Successfully fetch the ssl cert files")

	client, err := googleiot.NewGoogleIOTService(certs, logger)
	if err != nil {
		logger.StdErrFatal("Failed to fetch the ssl cert files > %s", err.Error())
		os.Exit(failedToInitIOTService)
	}

	return client
}

func setupCRON(ctx context.Context, logger logger.LoggerService, i iot.Interface) error {
	c := cron.New()
	if _, err := c.AddFunc("1 * * * * *", func() {
		logger.StdOut("Publishing a new data snapshot")
		i.PublishSensorDataSnapshot(ctx)
	}); err != nil {
		return err
	}

	c.Start()
	return nil
}
