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

	"golang.org/x/sync/errgroup"
)

//Service represents the structure of the service layer
type Service interface {
	//FetchSensorData fetches sensors data
	FetchSensorData(ctx context.Context) (*SensorData, error)
	//IntializeSensors initializes the sensors
	IntializeSensors() error
}

type service struct {
	PyFile string
	td     *i2c.TSL2561Driver
	cd     *i2c.CCS811Driver
}

// NewSensorService returns a new instance of the Service interface
func NewSensorService(tsl2561Driver *i2c.TSL2561Driver, ccs811Driver *i2c.CCS811Driver) Service {
	s := &service{
		PyFile: "main.py",
		td:     tsl2561Driver,
		cd:     ccs811Driver,
	}

	return s
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
	val, err := s.execPythonCommand(cmd)
	if err != nil {
		panic("Couldn't execute the command")
	}
	filteredVal := strings.Replace(val, "temp=", "", -1)
	temp := strings.Replace(filteredVal, "'C\n", "", -1)

	floatTemp, err := strconv.ParseFloat(temp, 10)
	if err != nil {
		return err
	}

	cpuTemp.Temp = floatTemp
	return nil
}

func (s *service) execPythonCommand(cmd *exec.Cmd) (string, error) {
	val, err := cmd.Output()
	if err != nil {
		return "", errors.New(fmt.Sprintf("Could not execute command with args > %v", cmd.Args))
	}

	if string(val) == "" {
		return "", errors.New("There was no std output")
	}

	return string(val), nil
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
	gd.Temperature = temp

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

func (s *service) IntializeSensors() error {
	cmd := exec.Command("python", s.PyFile, "--sensor=initialize_light")
	_, err := s.execPythonCommand(cmd)
	if err != nil {
		return err
	}

	return nil
}
