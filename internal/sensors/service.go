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
	//SetFanStatus toggles the fan state
	SetFanStatus(status FanState) error
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

// SetFanStatus change the current state of the fan
func (s *service) SetFanStatus(state FanState) error {
	var err error
	if state == FanOn {
		err = s.fd.On()
	} else if state == FanOff {
		err = s.fd.Off()
	} else {
		err = s.fd.Toggle()
	}
	return err
}

// FetchSensorData returns an object representing all of the sensor data
func (s *service) FetchSensorData(ctx context.Context) (*SensorData, error) {
	g, ctx := errgroup.WithContext(ctx)

	temp := &TempData{}
	g.Go(func() error {
		return s.fetchCPUTemp(temp)
	})

	gd := &GasData{}
	g.Go(func() error {
		return s.fetchGasData(gd, temp)
	})

	ld := &LightData{}
	g.Go(func() error {
		return s.fetchLightData(ld)
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}

	sd := &SensorData{}
	sd.convertFromLightAndGasData(gd, ld, time.Now(), temp)

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

	cpuTemp.CPUTemp = floatTemp
	return nil
}

func (s *service) fetchGasData(gd *GasData, t *TempData) error {
	co2, tvoc, err := s.cd.GetGasData()
	if err != nil {
		return err
	}

	temp, err := s.cd.GetTemperature()
	if err != nil {
		return err
	}

	gd.CO2 = co2
	gd.TVOC = tvoc
	t.RoomTemp = temp

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
