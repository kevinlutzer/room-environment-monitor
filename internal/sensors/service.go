package sensors

import (
	"context"
	"encoding/json"
	"errors"
	"fmt"
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

type service struct{}

func NewSensorService() Service {
	return &service{}
}

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
		return s.fetchCpuTemp(cpuTemp)
	})

	err := g.Wait()
	if err != nil {
		return nil, err
	}

	sd := &SensorData{}
	sd.convertFromLightAndGasData(gd, ld, time.Now(), cpuTemp)

	return sd, nil
}

func (s *service) fetchCpuTemp(cpuTemp *TempData) error {
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
	cmd := exec.Command("python", "main.py", "--sensor=gas")
	val, err := s.execPythonCommand(cmd)
	if err != nil {
		return err
	}

	err = json.Unmarshal([]byte(val), gd)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) fetchLightData(ld *LightData) error {
	cmd := exec.Command("python", "main.py", "--sensor=light")
	val, err := s.execPythonCommand(cmd)
	if err != nil {
		return err
	}

	err = json.Unmarshal([]byte(val), ld)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) IntializeSensors() error {
	cmd := exec.Command("python", "main.py", "--sensor=initialize_light")
	_, err := s.execPythonCommand(cmd)
	if err != nil {
		return err
	}

	return nil
}
