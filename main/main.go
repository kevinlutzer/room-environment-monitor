package main

import (
	"context"
	"encoding/json"
	"os"
	"runtime"

	"github.com/kml183/room-environment-monitor/internal/iot"
	"github.com/kml183/room-environment-monitor/internal/sensors"
	httpserver "github.com/kml183/room-environment-monitor/internal/server"
	"gobot.io/x/gobot"
	"gobot.io/x/gobot/drivers/i2c"
	"gobot.io/x/gobot/platforms/raspi"

	googleiot "github.com/kml183/room-environment-monitor/internal/google-iot"
	"github.com/kml183/room-environment-monitor/internal/logger"
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

	// Publish device status on startup
	if err := iotService.PublishDeviceStatus(ctx); err != nil {
		logger.StdErrFatal(err.Error())
		os.Exit(failedToPublishDeviceStatus)
	}
	logger.StdOut("Successfully published the device status initially")

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
	// Get Command Line Args
	args, err := GetCommandLineArgs()
	if err != nil {
		logger.StdErrFatal(err.Error())
		os.Exit(failedToGetCommandLineArguments)
	}
	logger.StdOut("Successfully get the command line argument")

	m, _ := json.Marshal(args)
	logger.StdOut("The command line args are: %s", string(m))

	// Create SSL Certs
	certs, err := googleiot.GetSSLCerts()
	if err != nil {
		logger.StdErrFatal("Failed to fetch the ssl cert files > %s", err.Error())
		os.Exit(failedToGetSSLCerts)
	}
	logger.StdOut("Successfully fetch the ssl cert files")

	return googleiot.NewGoogleIOTService(certs, logger)
}

func setupCRON(ctx context.Context, logger logger.LoggerService, i iot.Interface) error {
	c := cron.New()
	if _, err := c.AddFunc("* 5 * * * *", func() {
		logger.StdOut("Publishing a new data snapshot")
		i.PublishSensorDataSnapshot(ctx)
	}); err != nil {
		return err
	}

	if _, err := c.AddFunc("30 * * * * *", func() {
		logger.StdOut("Subscribing to configuration changes")
		i.SubscribeToIOTCoreConfig(ctx)
	}); err != nil {
		return err
	}

	if _, err := c.AddFunc("@hourly", func() {
		logger.StdOut("Publishing the device status")
		i.PublishDeviceStatus(ctx)
	}); err != nil {
		return err
	}

	c.Start()
	return nil
}
