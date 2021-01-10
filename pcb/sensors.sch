EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 2 2
Title "Sensors"
Date "2020-08-19"
Rev "1"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L power:GND #PWR?
U 1 1 5F9ABC13
P 3800 1700
AR Path="/5F9ABC13" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC13" Ref="#PWR0110"  Part="1" 
F 0 "#PWR0110" H 3800 1450 50  0001 C CNN
F 1 "GND" H 3805 1527 50  0000 C CNN
F 2 "" H 3800 1700 50  0001 C CNN
F 3 "" H 3800 1700 50  0001 C CNN
	1    3800 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3800 1700 3800 1650
Text GLabel 4200 3400 2    50   BiDi ~ 0
I2C_SDA
Text GLabel 4200 3300 2    50   Input ~ 0
I2C_SCK
Wire Wire Line
	3400 1300 3450 1300
Wire Wire Line
	3400 1400 3450 1400
$Comp
L power:+3.3V #PWR?
U 1 1 5F9ABC1E
P 3800 850
AR Path="/5F9ABC1E" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC1E" Ref="#PWR0111"  Part="1" 
F 0 "#PWR0111" H 3800 700 50  0001 C CNN
F 1 "+3.3V" H 3815 1023 50  0000 C CNN
F 2 "" H 3800 850 50  0001 C CNN
F 3 "" H 3800 850 50  0001 C CNN
	1    3800 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4200 1250 4250 1250
Text GLabel 4250 1250 2    50   Output ~ 0
nINT
Wire Wire Line
	3800 850  3800 1000
$Comp
L power:+3.3V #PWR?
U 1 1 5F9ABC44
P 2000 3200
AR Path="/5F9ABC44" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC44" Ref="#PWR0113"  Part="1" 
F 0 "#PWR0113" H 2000 3050 50  0001 C CNN
F 1 "+3.3V" H 2015 3373 50  0000 C CNN
F 2 "" H 2000 3200 50  0001 C CNN
F 3 "" H 2000 3200 50  0001 C CNN
	1    2000 3200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5F9ABC4B
P 1300 3200
AR Path="/5F9ABC4B" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC4B" Ref="#PWR0114"  Part="1" 
F 0 "#PWR0114" H 1300 3050 50  0001 C CNN
F 1 "+3.3V" H 1315 3373 50  0000 C CNN
F 2 "" H 1300 3200 50  0001 C CNN
F 3 "" H 1300 3200 50  0001 C CNN
	1    1300 3200
	1    0    0    -1  
$EndComp
Wire Wire Line
	1900 4000 2050 4000
Text GLabel 1150 4000 0    50   Input ~ 0
I2C_SCK
Wire Wire Line
	1150 4000 1400 4000
Text GLabel 2100 3400 2    50   BiDi ~ 0
I2C_SDA
Wire Wire Line
	1550 5600 1550 5650
Wire Wire Line
	1550 5650 1650 5650
Wire Wire Line
	1750 5650 1750 5600
Wire Wire Line
	1650 5600 1650 5650
Connection ~ 1650 5650
Wire Wire Line
	1650 5650 1750 5650
$Comp
L power:GND #PWR?
U 1 1 5F9ABC5B
P 1650 5650
AR Path="/5F9ABC5B" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC5B" Ref="#PWR0118"  Part="1" 
F 0 "#PWR0118" H 1650 5400 50  0001 C CNN
F 1 "GND" H 1655 5477 50  0000 C CNN
F 2 "" H 1650 5650 50  0001 C CNN
F 3 "" H 1650 5650 50  0001 C CNN
	1    1650 5650
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 3400 1400 3400
$Comp
L power:+1V8 #PWR?
U 1 1 5F9ABC6E
P 2100 4700
AR Path="/5F9ABC6E" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC6E" Ref="#PWR0120"  Part="1" 
F 0 "#PWR0120" H 2100 4550 50  0001 C CNN
F 1 "+1V8" H 2115 4873 50  0000 C CNN
F 2 "" H 2100 4700 50  0001 C CNN
F 3 "" H 2100 4700 50  0001 C CNN
	1    2100 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 5250 2100 5250
Wire Wire Line
	2100 4850 2100 4700
$Comp
L power:+1V8 #PWR?
U 1 1 5F9ABC79
P 1150 4700
AR Path="/5F9ABC79" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC79" Ref="#PWR0121"  Part="1" 
F 0 "#PWR0121" H 1150 4550 50  0001 C CNN
F 1 "+1V8" H 1165 4873 50  0000 C CNN
F 2 "" H 1150 4700 50  0001 C CNN
F 3 "" H 1150 4700 50  0001 C CNN
	1    1150 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1150 4850 1150 4700
Wire Wire Line
	1050 5250 1150 5250
$Comp
L power:+3.3V #PWR?
U 1 1 5F9ABC95
P 4250 4550
AR Path="/5F9ABC95" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5F9ABC95" Ref="#PWR0122"  Part="1" 
F 0 "#PWR0122" H 4250 4400 50  0001 C CNN
F 1 "+3.3V" H 4265 4723 50  0000 C CNN
F 2 "" H 4250 4550 50  0001 C CNN
F 3 "" H 4250 4550 50  0001 C CNN
	1    4250 4550
	1    0    0    -1  
$EndComp
Text GLabel 4050 5100 0    50   BiDi ~ 0
I2C_SDA
Text GLabel 4050 5250 0    50   Input ~ 0
I2C_SCK
Wire Wire Line
	4050 5250 4400 5250
Wire Wire Line
	4050 4650 4050 4550
Wire Wire Line
	4050 4550 4250 4550
Wire Wire Line
	4400 4550 4400 4650
Connection ~ 4250 4550
Wire Wire Line
	4250 4550 4400 4550
Text GLabel 1050 5250 0    50   Input ~ 0
I2C_SDA_1V8
Text GLabel 2150 5250 2    50   Input ~ 0
I2C_SCK_1V8
Text GLabel 2050 4000 2    50   Output ~ 0
I2C_SCK_1V8
Text GLabel 1150 3400 0    50   BiDi ~ 0
I2C_SDA_1V8
Wire Wire Line
	1900 3400 2100 3400
Wire Wire Line
	1400 3700 1300 3700
Wire Wire Line
	1900 3700 2000 3700
Wire Wire Line
	2100 5150 2100 5250
Connection ~ 2100 5250
Wire Wire Line
	2100 5250 2050 5250
Wire Wire Line
	1150 5150 1150 5250
Connection ~ 1150 5250
Wire Wire Line
	1150 5250 1250 5250
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5FB3311F
P 3900 3300
F 0 "J2" H 3850 3450 50  0000 C CNN
F 1 "I2C_BUS" H 3950 3050 50  0000 C CNN
F 2 "Connector_PinHeader_2.54mm:PinHeader_1x04_P2.54mm_Vertical" H 3900 3300 50  0001 C CNN
F 3 "~" H 3900 3300 50  0001 C CNN
	1    3900 3300
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FB40ACA
P 4200 3550
AR Path="/5FB40ACA" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5FB40ACA" Ref="#PWR0129"  Part="1" 
F 0 "#PWR0129" H 4200 3300 50  0001 C CNN
F 1 "GND" H 4205 3377 50  0000 C CNN
F 2 "" H 4200 3550 50  0001 C CNN
F 3 "" H 4200 3550 50  0001 C CNN
	1    4200 3550
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5FB40EF1
P 4200 3150
AR Path="/5FB40EF1" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5FB40EF1" Ref="#PWR0130"  Part="1" 
F 0 "#PWR0130" H 4200 3000 50  0001 C CNN
F 1 "+3.3V" H 4215 3323 50  0000 C CNN
F 2 "" H 4200 3150 50  0001 C CNN
F 3 "" H 4200 3150 50  0001 C CNN
	1    4200 3150
	1    0    0    -1  
$EndComp
Wire Wire Line
	4100 3200 4200 3200
Wire Wire Line
	4200 3200 4200 3150
Wire Wire Line
	4100 3300 4200 3300
Wire Wire Line
	4100 3400 4200 3400
Wire Wire Line
	4100 3500 4200 3500
Wire Wire Line
	4200 3500 4200 3550
Text GLabel 3400 1300 0    50   Input ~ 0
I2C_SCK
Text GLabel 3400 1400 0    50   BiDi ~ 0
I2C_SDA
$Comp
L dk_Transistors-FETs-MOSFETs-Arrays:BSS138DW-7-F Q2
U 1 1 5F240700
P 1600 3800
F 0 "Q2" H 1550 4287 60  0000 C CNN
F 1 "BSS138DW-7-F" H 1550 4181 60  0000 C CNN
F 2 "digikey-footprints:SOT-363" H 1800 4000 60  0001 L CNN
F 3 "https://www.diodes.com/assets/Datasheets/ds30203.pdf" V 1800 4100 60  0001 L CNN
F 4 "BSS138DW-FDICT-ND" H 1800 4200 60  0001 L CNN "Digi-Key_PN"
F 5 "BSS138DW-7-F" H 1800 4300 60  0001 L CNN "MPN"
F 6 "Discrete Semiconductor Products" H 1800 4400 60  0001 L CNN "Category"
F 7 "Transistors - FETs, MOSFETs - Arrays" H 1800 4500 60  0001 L CNN "Family"
F 8 "https://www.diodes.com/assets/Datasheets/ds30203.pdf" H 1800 4600 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/diodes-incorporated/BSS138DW-7-F/BSS138DW-FDICT-ND/808541" H 1800 4700 60  0001 L CNN "DK_Detail_Page"
F 10 "MOSFET 2N-CH 50V 0.2A SC70-6" H 1800 4800 60  0001 L CNN "Description"
F 11 "Diodes Incorporated" H 1800 4900 60  0001 L CNN "Manufacturer"
F 12 "Active" H 1800 5000 60  0001 L CNN "Status"
	1    1600 3800
	-1   0    0    1   
$EndComp
$Comp
L Device:C C3
U 1 1 5F40D43E
P 700 1600
F 0 "C3" H 815 1646 50  0000 L CNN
F 1 "100n" H 815 1555 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 738 1450 50  0001 C CNN
F 3 "~" H 700 1600 50  0001 C CNN
	1    700  1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0139
U 1 1 5F40D8C5
P 700 1800
F 0 "#PWR0139" H 700 1550 50  0001 C CNN
F 1 "GND" H 705 1627 50  0000 C CNN
F 2 "" H 700 1800 50  0001 C CNN
F 3 "" H 700 1800 50  0001 C CNN
	1    700  1800
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0140
U 1 1 5F40E142
P 700 1400
F 0 "#PWR0140" H 700 1250 50  0001 C CNN
F 1 "+3.3V" H 715 1573 50  0000 C CNN
F 2 "" H 700 1400 50  0001 C CNN
F 3 "" H 700 1400 50  0001 C CNN
	1    700  1400
	1    0    0    -1  
$EndComp
$Comp
L Device:C C11
U 1 1 5F40E452
P 4300 1850
F 0 "C11" H 4415 1896 50  0000 L CNN
F 1 "100n" H 4415 1805 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 4338 1700 50  0001 C CNN
F 3 "~" H 4300 1850 50  0001 C CNN
	1    4300 1850
	1    0    0    -1  
$EndComp
$Comp
L Device:C C12
U 1 1 5F40EB79
P 2950 5000
F 0 "C12" H 3065 5046 50  0000 L CNN
F 1 "100n" H 3065 4955 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric" H 2988 4850 50  0001 C CNN
F 3 "~" H 2950 5000 50  0001 C CNN
	1    2950 5000
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0141
U 1 1 5F40F2D5
P 4300 2050
F 0 "#PWR0141" H 4300 1800 50  0001 C CNN
F 1 "GND" H 4305 1877 50  0000 C CNN
F 2 "" H 4300 2050 50  0001 C CNN
F 3 "" H 4300 2050 50  0001 C CNN
	1    4300 2050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0142
U 1 1 5F40F86F
P 2950 5200
F 0 "#PWR0142" H 2950 4950 50  0001 C CNN
F 1 "GND" H 2955 5027 50  0000 C CNN
F 2 "" H 2950 5200 50  0001 C CNN
F 3 "" H 2950 5200 50  0001 C CNN
	1    2950 5200
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0143
U 1 1 5F40FC60
P 4300 1650
F 0 "#PWR0143" H 4300 1500 50  0001 C CNN
F 1 "+3.3V" H 4315 1823 50  0000 C CNN
F 2 "" H 4300 1650 50  0001 C CNN
F 3 "" H 4300 1650 50  0001 C CNN
	1    4300 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4300 1700 4300 1650
Wire Wire Line
	700  1450 700  1400
Wire Wire Line
	700  1750 700  1800
Wire Wire Line
	4300 2000 4300 2050
Wire Wire Line
	2950 5150 2950 5200
Wire Wire Line
	1300 3200 1300 3700
Wire Wire Line
	2000 3200 2000 3700
$Comp
L Device:R_US R10
U 1 1 5F42F459
P 1150 5000
F 0 "R10" H 1218 5046 50  0000 L CNN
F 1 "10k" H 1218 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 1190 4990 50  0001 C CNN
F 3 "~" H 1150 5000 50  0001 C CNN
	1    1150 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R11
U 1 1 5F431D99
P 2100 5000
F 0 "R11" H 2168 5046 50  0000 L CNN
F 1 "10k" H 2168 4955 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 2140 4990 50  0001 C CNN
F 3 "~" H 2100 5000 50  0001 C CNN
	1    2100 5000
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R12
U 1 1 5F43299F
P 4400 4800
F 0 "R12" H 4468 4846 50  0000 L CNN
F 1 "10k" H 4468 4755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4440 4790 50  0001 C CNN
F 3 "~" H 4400 4800 50  0001 C CNN
	1    4400 4800
	1    0    0    -1  
$EndComp
$Comp
L Device:R_US R13
U 1 1 5F437086
P 4050 4800
F 0 "R13" H 4118 4846 50  0000 L CNN
F 1 "10k" H 4118 4755 50  0000 L CNN
F 2 "Resistor_SMD:R_0603_1608Metric" V 4090 4790 50  0001 C CNN
F 3 "~" H 4050 4800 50  0001 C CNN
	1    4050 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4400 4950 4400 5250
Wire Wire Line
	4050 4950 4050 5100
Text Notes 700  6350 0    50   ~ 0
Sensor Address\nBME280 - 0x77\nTSL2591 - 0x29\nSGP30 - 0x58
$Comp
L Kevin_Lutzer:SGP30 U6
U 1 1 5F5E68DD
P 1650 5300
F 0 "U6" H 1850 5550 50  0000 L CNN
F 1 "SGP30" H 1800 5050 50  0000 L CNN
F 2 "Kevin-Lutzer-Kicad-Lbr:SGP30" H 1600 6000 50  0001 C CNN
F 3 "" H 1650 5250 50  0001 C CNN
	1    1650 5300
	1    0    0    -1  
$EndComp
Text Notes 500  600  0    100  ~ 0
Humidity/Temperature
Text Notes 500  2800 0    100  ~ 0
Air Quality
Text Notes 1150 2950 0    50   ~ 0
Lvl Shifter for 1.8V to 3.3V
Wire Notes Line
	500  2650 4750 2650
Text Notes 2900 600  0    100  ~ 0
Light
Wire Notes Line
	2900 500  2900 2650
Wire Notes Line
	3550 5900 3550 2650
Wire Notes Line
	500  5900 3550 5900
Text Notes 3550 2800 0    100  ~ 0
I2C Port
Wire Notes Line
	3550 3850 4750 3850
Wire Notes Line
	4750 5400 3550 5400
Wire Notes Line
	4750 500  4750 5400
Text Notes 3550 4150 0    100  ~ 0
I2C Bus\nPullups
$Comp
L dk_Humidity-Moisture-Sensors:BME280 U4
U 1 1 600A75C8
P 2150 1600
F 0 "U4" H 2478 1703 60  0000 L CNN
F 1 "BME280" H 2478 1597 60  0000 L CNN
F 2 "digikey-footprints:Pressure_Sensor_LGA-8_2.5x2.5mm_BME280" H 2350 1800 60  0001 L CNN
F 3 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 2350 1900 60  0001 L CNN
F 4 "828-1063-1-ND" H 2350 2000 60  0001 L CNN "Digi-Key_PN"
F 5 "BME280" H 2350 2100 60  0001 L CNN "MPN"
F 6 "Sensors, Transducers" H 2350 2200 60  0001 L CNN "Category"
F 7 "Humidity, Moisture Sensors" H 2350 2300 60  0001 L CNN "Family"
F 8 "https://ae-bst.resource.bosch.com/media/_tech/media/datasheets/BST-BME280-DS002.pdf" H 2350 2400 60  0001 L CNN "DK_Datasheet_Link"
F 9 "/product-detail/en/bosch-sensortec/BME280/828-1063-1-ND/6136314" H 2350 2500 60  0001 L CNN "DK_Detail_Page"
F 10 "SENSOR PRESSURE HUMIDITY TEMP" H 2350 2600 60  0001 L CNN "Description"
F 11 "Bosch Sensortec" H 2350 2700 60  0001 L CNN "Manufacturer"
F 12 "Active" H 2350 2800 60  0001 L CNN "Status"
	1    2150 1600
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR?
U 1 1 5FFBEA29
P 2200 2000
AR Path="/5FFBEA29" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5FFBEA29" Ref="#PWR0105"  Part="1" 
F 0 "#PWR0105" H 2200 1750 50  0001 C CNN
F 1 "GND" H 2205 1827 50  0000 C CNN
F 2 "" H 2200 2000 50  0001 C CNN
F 3 "" H 2200 2000 50  0001 C CNN
	1    2200 2000
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5FFBEED3
P 2200 1100
AR Path="/5FFBEED3" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5FFBEED3" Ref="#PWR0109"  Part="1" 
F 0 "#PWR0109" H 2200 950 50  0001 C CNN
F 1 "+3.3V" H 2215 1273 50  0000 C CNN
F 2 "" H 2200 1100 50  0001 C CNN
F 3 "" H 2200 1100 50  0001 C CNN
	1    2200 1100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2150 1900 2150 2000
Wire Wire Line
	2150 2000 2200 2000
Wire Wire Line
	2200 2000 2250 2000
Wire Wire Line
	2250 2000 2250 1900
Connection ~ 2200 2000
Wire Wire Line
	2150 1200 2150 1100
Wire Wire Line
	2150 1100 2200 1100
Wire Wire Line
	2200 1100 2250 1100
Wire Wire Line
	2250 1100 2250 1200
Connection ~ 2200 1100
Text GLabel 1450 1400 0    50   Input ~ 0
I2C_SCK
Text GLabel 1450 1500 0    50   BiDi ~ 0
I2C_SDA
Wire Wire Line
	2150 1100 1600 1100
Wire Wire Line
	1600 1100 1600 1700
Wire Wire Line
	1600 1700 1750 1700
Connection ~ 2150 1100
Wire Wire Line
	1450 1400 1750 1400
Wire Wire Line
	1450 1500 1750 1500
Wire Wire Line
	1750 1600 1700 1600
Wire Wire Line
	1700 1600 1700 2000
Wire Wire Line
	1700 2000 2150 2000
Connection ~ 2150 2000
$Comp
L Kevin_Lutzer:TSL2591 U7
U 1 1 5FFE72E4
P 3850 1350
F 0 "U7" H 3550 1650 50  0000 L CNN
F 1 "TSL2591" H 3850 1650 50  0000 L CNN
F 2 "Kevin-Lutzer-Kicad-Lbr:TSL2591" H 3300 1100 50  0001 C CNN
F 3 "https://cdn-shop.adafruit.com/datasheets/TSL25911_Datasheet_EN_v1.pdf" H 3800 1400 50  0001 C CNN
	1    3850 1350
	1    0    0    -1  
$EndComp
$Comp
L power:+1V8 #PWR?
U 1 1 5FFBD6B0
P 1700 4700
AR Path="/5FFBD6B0" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/5FFBD6B0" Ref="#PWR0119"  Part="1" 
F 0 "#PWR0119" H 1700 4550 50  0001 C CNN
F 1 "+1V8" H 1715 4873 50  0000 C CNN
F 2 "" H 1700 4700 50  0001 C CNN
F 3 "" H 1700 4700 50  0001 C CNN
	1    1700 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	1700 4700 1700 4950
Wire Wire Line
	1600 4950 1700 4950
Wire Wire Line
	1600 4950 1600 5000
Connection ~ 1700 4950
Wire Wire Line
	1700 4950 1700 5000
$Comp
L power:+1V8 #PWR?
U 1 1 600722B4
P 2950 4700
AR Path="/600722B4" Ref="#PWR?"  Part="1" 
AR Path="/5F9A85F1/600722B4" Ref="#PWR?"  Part="1" 
F 0 "#PWR?" H 2950 4550 50  0001 C CNN
F 1 "+1V8" H 2965 4873 50  0000 C CNN
F 2 "" H 2950 4700 50  0001 C CNN
F 3 "" H 2950 4700 50  0001 C CNN
	1    2950 4700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2950 4700 2950 4850
$EndSCHEMATC
