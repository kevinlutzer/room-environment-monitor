package sensors

import (
	"time"
)

type LightData struct {
	Lux uint32 `json:"lux"`
}

type TempData struct {
	Temp float64 `json:"temp"`
}

type GasData struct {
	CO2      uint16  `json:"co2"`
	TVOC     uint16  `json:"tvoc"`
	RoomTemp float32 `json:"room_temp"`
}

type SensorData struct {
	Lux       uint32    `json:"lux"`
	CO2       uint16    `json:"co2"`
	TVOC      uint16    `json:"tvoc"`
	RoomTemp  float32   `json:"room_temp"`
	CPUTemp   float64   `json:"cpu_temp"`
	TimeStamp time.Time `json:"timestamp"`
}

func (s *SensorData) convertFromLightAndGasData(g *GasData, l *LightData, t time.Time, cpuTemp *TempData) {
	s.Lux = l.Lux

	s.CO2 = g.CO2
	s.TVOC = g.TVOC
	s.RoomTemp = g.RoomTemp

	s.TimeStamp = t
	s.CPUTemp = cpuTemp.Temp
}

type FanState string

const (
	ToggleFan FanState = "Toggle"
	FanOn     FanState = "On"
	FanOff    FanState = "Off"
)
