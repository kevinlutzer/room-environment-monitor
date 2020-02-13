package sensors

import (
	"time"
)

type SensorData struct {
	Lux       uint32    `json:"lux"`
	Pressure  float32   `json:"pressure"`
	Humidity  float32   `json:"humidity"`
	RoomTemp  float32   `json:"room_temp"`
	CO2       uint16    `json:"co2"`
	TVOC      uint16    `json:"tvoc"`
	CPUTemp   float32   `json:"cpu_temp"`
	TimeStamp time.Time `json:"timestamp"`
}

func NewSensorData(lux uint32, cpuTemp float32, pressure float32, humidity float32, roomTemp float32, co2 uint16, tvoc uint16) *SensorData {
	return &SensorData{
		Lux:       lux,
		CPUTemp:   cpuTemp,
		Pressure:  pressure,
		Humidity:  humidity,
		RoomTemp:  roomTemp,
		CO2:       co2,
		TVOC:      tvoc,
		TimeStamp: time.Now(),
	}
}
