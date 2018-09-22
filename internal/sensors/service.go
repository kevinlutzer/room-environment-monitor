package sensors

import (
	"context"
	"errors"
	"fmt"

	"gobot.io/x/gobot/drivers/i2c"

	"os/exec"
	"strconv"
	"strings"
	"time"

	"gobot.io/x/gobot/drivers/gpio"

	"golang.org/x/sync/errgroup"
)

const (
	CPUTempFanThresh = 35
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//ToggleFan toggles the fan state
	ToggleFan() error
}

type service struct {
	PyFile string
	td     *i2c.TSL2561Driver
	cd     *i2c.CCS811Driver
	fd     *gpio.LedDriver
}

// NewSensorService returns a new instance of the Service interface
func NewSensorService(tsl2561Driver *i2c.TSL2561Driver, ccs811Driver *i2c.CCS811Driver, fanDriver *gpio.LedDriver) Service {
	s := &service{
		PyFile: "main.py",
		td:     tsl2561Driver,
		cd:     ccs811Driver,
		fd:     fanDriver,
	}

	return s
}

// Change the current state of the fan
func (s *service) ToggleFan() error {
	return s.fd.Toggle()
}

// SetFanFromCPUTemp
func (s *service) SetFanFromCPUTemp() error {
	cpuTemp := &TempData{}
	if err := s.fetchCPUTemp(cpuTemp); err != nil {
		return err
	}

	if cpuTemp.Temp >= CPUTempFanThresh {
		return s.fd.On()
	}

	return nil
}

// FetchSensorData returns an object representing all of the sensor data
func (s *service) FetchSensorData(ctx context.Context) (*SensorData, error) {
	g, ctx := errgroup.WithContext(ctx)

	gd := &GasData{}
	g.Go(func() error {
		return s.fetchGasData(gd)
	})

	ld := &LightData{}
	g.Go(func() error {
		return s.fetchLightData(ld)
	})

	cpuTemp := &TempData{}
	g.Go(func() error {
		return s.fetchCPUTemp(cpuTemp)
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}

	sd := &SensorData{}
	sd.convertFromLightAndGasData(gd, ld, time.Now(), cpuTemp)

	return sd, nil
}

func (s *service) fetchCPUTemp(cpuTemp *TempData) error {
	cmd := exec.Command("sudo", "/opt/vc/bin/vcgencmd", "measure_temp")
	val, err := cmd.Output()
	if err != nil {
		return errors.New(fmt.Sprintf("Could not execute command with args > %v", cmd.Args))
	}

	if string(val) == "" {
		return errors.New("There was no std output")
	}

	filteredVal := strings.Replace(string(val), "temp=", "", -1)
	temp := strings.Replace(filteredVal, "'C\n", "", -1)

	floatTemp, err := strconv.ParseFloat(temp, 10)
	if err != nil {
		return err
	}

	cpuTemp.Temp = floatTemp
	return nil
}

func (s *service) fetchGasData(gd *GasData) error {
	co2, tvoc, err := s.cd.GetData()
	if err != nil {
		return err
	}

	temp, err := s.cd.GetTemperature()
	if err != nil {
		return err
	}

	gd.CO2 = co2
	gd.TVOC = tvoc
	gd.RoomTemp = temp

	return nil
}

func (s *service) fetchLightData(ld *LightData) error {
	b, ir, err := s.td.GetLuminocity()
	if err != nil {
		return err
	}
	ld.Lux = s.td.CalculateLux(b, ir)

	return nil
}
