package sensors

import (
	"time"
)

type LightData struct {
	Lux int32 `json:"lux"`
}

type GasData struct {
	CO2         float32 `json:"co2"`
	TVOC        int32   `json:"tvoc"`
	Temperature string  `json:"temp"`
}

type SensorData struct {
	Lux         int32
	CO2         float32
	TVOC        int32
	Temperature string
	CPUTemp     string
	TimeStamp   time.Time
}

func (s *SensorData) convertFromLightAndGasData(g *GasData, l *LightData, t time.Time, cpuTemp string) {
	s.Lux = l.Lux

	s.CO2 = g.CO2
	s.TVOC = g.TVOC
	s.Temperature = g.Temperature

	s.TimeStamp = t
	s.CPUTemp = cpuTemp
}
