package sensors

import (
	"context"
	"errors"
	"fmt"

	"gobot.io/x/gobot/drivers/i2c"

	"os/exec"
	"strconv"
	"strings"

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
	tsl2561Driver *i2c.TSL2561Driver
	ccs811Driver  *i2c.CCS811Driver
	bme280Driver  *i2c.BME280Driver
	fanDriver     *gpio.LedDriver
}

// NewSensorService returns a new instance of the Service interface
func NewSensorService(tsl2561Driver *i2c.TSL2561Driver, ccs811Driver *i2c.CCS811Driver, bme280Driver *i2c.BME280Driver, fanDriver *gpio.LedDriver) Service {
	s := &service{
		tsl2561Driver: tsl2561Driver,
		ccs811Driver:  ccs811Driver,
		bme280Driver:  bme280Driver,
		fanDriver:     fanDriver,
	}

	return s
}

// SetFanStatus change the current state of the fan
func (s *service) SetFanStatus(state FanState) error {
	var err error
	if state == FanOn {
		err = s.fanDriver.On()
	} else if state == FanOff {
		err = s.fanDriver.Off()
	} else {
		err = s.fanDriver.Toggle()
	}
	return err
}

// FetchSensorData returns an object representing all of the sensor data
func (s *service) FetchSensorData(ctx context.Context) (*SensorData, error) {
	g, ctx := errgroup.WithContext(ctx)

	var lux uint32
	var pressure, humidity, roomTemp, cpuTemp float32
	var co2, tvoc uint16

	g.Go(func() error {
		return s.fetchCPUTemp(&cpuTemp)
	})

	g.Go(func() error {
		return s.fetchGasData(&co2, &tvoc)
	})

	g.Go(func() error {
		return s.fetchLightData(&lux)
	})

	g.Go(func() error {
		return s.fetchHumidty(&pressure, &humidity, &roomTemp)
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}

	return NewSensorData(lux, cpuTemp, pressure, humidity, roomTemp, co2, tvoc), nil
}

func (s *service) fetchHumidty(pressure, humidity, roomTemp *float32) error {
	var err error
	*pressure, err = s.bme280Driver.Pressure()
	if err != nil {
		return err
	}

	*humidity, err = s.bme280Driver.Humidity()
	if err != nil {
		return err
	}

	*roomTemp, err = s.bme280Driver.Temperature()
	if err != nil {
		return err
	}

	return nil
}

func (s *service) fetchCPUTemp(roomTemp *float32) error {
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

	*roomTemp = float32(floatTemp)
	return nil
}

func (s *service) fetchGasData(co2 *uint16, tvoc *uint16) error {
	var err error
	*co2, *tvoc, err = s.ccs811Driver.GetGasData()
	if err != nil {
		return err
	}

	return nil
}

func (s *service) fetchLightData(ld *uint32) error {
	b, ir, err := s.tsl2561Driver.GetLuminocity()
	if err != nil {
		return err
	}
	*ld = s.tsl2561Driver.CalculateLux(b, ir)

	return nil
}
