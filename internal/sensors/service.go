package sensors

import (
	"bufio"
	"context"
	"encoding/json"
	"errors"
	"fmt"
	"io"
	"log"
	"os/exec"
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
	sd.convertFromLightAndGasData(gd, ld, time.Now(), cpuTemp.Temp)

	return sd, nil
}

func (s *service) fetchCpuTemp(cpuTemp *TempData) error {
	cmd := exec.Command("python", "main.py")
	val, err := s.execCommand(cmd)
	if err != nil {
		return err
	}

	fmt.Println(val)

	err = json.Unmarshal([]byte(val), cpuTemp)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) execCommand(cmd *exec.Cmd) (string, error) {
	stdout, err := cmd.StdoutPipe()
	if err != nil {
		return "", errors.New("Can't setup stdout pipe")
	}

	// stderr, err := cmd.StderrPipe()
	// if err != nil {
	// 	return "", errors.New("Can't setup stderr pipe")
	// }
	err = cmd.Start()
	if err != nil {
		return "", errors.New("Can't start the command")
	}

	// if s.getCommandOutput(stderr) != "" {
	// 	return "", errors.New("The command failed")
	// }

	err = cmd.Wait()
	if err != nil {
		fmt.Printf("%s \n", err.Error())
		return "", errors.New("The command failed to execute")
	}

	return s.getCommandOutput(stdout), nil
}

func (s *service) getCommandOutput(r io.Reader) string {
	scanner := bufio.NewScanner(r)
	// Read First Line
	for scanner.Scan() {
		log.Fatalf(scanner.Text())
	}

	return scanner.Text()
}

func (s *service) fetchGasData(gd *GasData) error {
	m := "{\"temp\": \"32.6\", \"tvoc\": 2000, \"co2\": 23.5}"

	err := json.Unmarshal([]byte(m), gd)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) fetchLightData(ld *LightData) error {
	t := "{\"lux\": 3000}"

	err := json.Unmarshal([]byte(t), ld)
	if err != nil {
		return err
	}
	return nil
}

func (s *service) IntializeSensors() error {
	return errors.New("Not yet implemented")
}
