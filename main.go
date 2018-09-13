package main

import (
	"fmt"

	"log"
	"os"
	"runtime"

	"gobot.io/x/gobot/drivers/i2c"

	s "github.com/kml183/room-environment-monitor/internal/server"
)

func main() {

	logger := log.New(os.Stdout, "http: ", log.LstdFlags)

	// Get OS To Setup stub
	var stub bool
	if runtime.GOOS == "darwin" {
		stub = true
	} else {
		stub = false
	}

	if val := runtime.GOMAXPROCS(8); val < -1 {
		logger.Fatalf("Could not set the max processes, value recieved > %d \n", val)
	}

	if err := s.NewHTTPServer(logger, stub); err != nil {
		logger.Fatalf("Could not start the http server > %s \n", err.Error())
	}
}

func adafruitCCS811BreakoutBoard(a *i2c.CCS811Driver) {
	v, err := a.GetHardwareVersion()
	if err != nil {
		fmt.Printf(err.Error())
	}

	fmt.Printf("Hardare Version %#x\n", v)

	d, err := a.GetFirmwareBootVersion()
	if err != nil {
		fmt.Printf(err.Error())
	}

	fmt.Printf("Boot Version %#x\n", d)

	d, err = a.GetFirmwareAppVersion()
	if err != nil {
		fmt.Printf(err.Error())
	}

	fmt.Printf("App Version %#x\n", d)
}
