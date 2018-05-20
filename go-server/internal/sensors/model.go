package sensors

import (
	"time"
)

type LightData struct {
	Lux int32 `json:"lux"`
}

type TempData struct {
	Temp string `json:"temp"`
}

type GasData struct {
	CO2         float32 `json:"co2"`
	TVOC        int32   `json:"tvoc"`
	Temperature string  `json:"temp"`
}

type SensorData struct {
	Lux         int32     `json:"lux"`
	CO2         float32   `json:"co2"`
	TVOC        int32     `json:"tvoc"`
	Temperature string    `json:"temp"`
	CPUTemp     string    `json:"cpu_temp"`
	TimeStamp   time.Time `json:"timestamp"`
}

func (s *SensorData) convertFromLightAndGasData(g *GasData, l *LightData, t time.Time, cpuTemp string) {
	s.Lux = l.Lux

	s.CO2 = g.CO2
	s.TVOC = g.TVOC
	s.Temperature = g.Temperature

	s.TimeStamp = t
	s.CPUTemp = cpuTemp
}
