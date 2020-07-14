EESchema Schematic File Version 4
EELAYER 30 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title ""
Date ""
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L LD39050PU33R:LD39050PU33R U2
U 1 1 5F0AB553
P 3750 1300
F 0 "U2" H 3750 1770 50  0000 C CNN
F 1 "LD39050PU33R" H 3750 1679 50  0000 C CNN
F 2 "SON95P300X300X100-7N" H 3750 1300 50  0001 L BNN
F 3 "LD39050PU33R" H 3750 1300 50  0001 L BNN
F 4 "STMicroelectronics" H 3750 1300 50  0001 L BNN "Field4"
F 5 "DFN-6 STMicroelectronics" H 3750 1300 50  0001 L BNN "Field5"
F 6 "IPC7351B" H 3750 1300 50  0001 L BNN "Field6"
F 7 "497-8716-1-ND" H 3750 1300 50  0001 L BNN "Field7"
F 8 "https://www.digikey.ph/product-detail/en/stmicroelectronics/LD39050PU33R/497-8716-1-ND/2039901?utm_source=snapeda&utm_medium=aggregator&utm_campaign=symbol" H 3750 1300 50  0001 L BNN "Field8"
	1    3750 1300
	1    0    0    -1  
$EndComp
$Comp
L Battery_Management:MCP73811T-420I-OT U1
U 1 1 5F0ABD85
P 2000 1200
F 0 "U1" H 1700 1450 50  0000 L CNN
F 1 "MCP73811T-420I-OT" H 2050 950 50  0000 L CNN
F 2 "Package_TO_SOT_SMD:SOT-23-5" H 2050 950 50  0001 L CNN
F 3 "http://ww1.microchip.com/downloads/en/DeviceDoc/22036b.pdf" H 1750 1450 50  0001 C CNN
	1    2000 1200
	1    0    0    -1  
$EndComp
$Comp
L Connector_Generic:Conn_01x02 J1
U 1 1 5F0ADBF5
P 800 1200
F 0 "J1" H 718 875 50  0000 C CNN
F 1 "Conn_01x02" H 718 966 50  0000 C CNN
F 2 "Connector_JST:JST_XH_S2B-XH-A-1_1x02_P2.50mm_Horizontal" H 800 1200 50  0001 C CNN
F 3 "~" H 800 1200 50  0001 C CNN
	1    800  1200
	-1   0    0    1   
$EndComp
$Comp
L power:+BATT #PWR0101
U 1 1 5F0AE9DC
P 1200 1050
F 0 "#PWR0101" H 1200 900 50  0001 C CNN
F 1 "+BATT" H 1215 1223 50  0000 C CNN
F 2 "" H 1200 1050 50  0001 C CNN
F 3 "" H 1200 1050 50  0001 C CNN
	1    1200 1050
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0102
U 1 1 5F0AF48B
P 1200 1250
F 0 "#PWR0102" H 1200 1000 50  0001 C CNN
F 1 "GND" H 1205 1077 50  0000 C CNN
F 2 "" H 1200 1250 50  0001 C CNN
F 3 "" H 1200 1250 50  0001 C CNN
	1    1200 1250
	1    0    0    -1  
$EndComp
Wire Wire Line
	1000 1100 1200 1100
Wire Wire Line
	1200 1100 1200 1050
Wire Wire Line
	1000 1200 1200 1200
Wire Wire Line
	1200 1200 1200 1250
$Comp
L power:+BATT #PWR0103
U 1 1 5F0B03F5
P 2450 850
F 0 "#PWR0103" H 2450 700 50  0001 C CNN
F 1 "+BATT" H 2465 1023 50  0000 C CNN
F 2 "" H 2450 850 50  0001 C CNN
F 3 "" H 2450 850 50  0001 C CNN
	1    2450 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1300 3100 1100
Wire Wire Line
	3100 1100 3150 1100
Wire Wire Line
	3100 1300 3150 1300
$Comp
L power:GND #PWR0104
U 1 1 5F0B1838
P 2000 1650
F 0 "#PWR0104" H 2000 1400 50  0001 C CNN
F 1 "GND" H 2005 1477 50  0000 C CNN
F 2 "" H 2000 1650 50  0001 C CNN
F 3 "" H 2000 1650 50  0001 C CNN
	1    2000 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2000 1650 2000 1500
Wire Wire Line
	1600 1300 1550 1300
Wire Wire Line
	1550 1300 1550 1100
Wire Wire Line
	1550 850  2000 850 
Wire Wire Line
	2000 850  2000 900 
Wire Wire Line
	1600 1100 1550 1100
Connection ~ 1550 1100
Wire Wire Line
	1550 1100 1550 850 
Wire Wire Line
	2400 1100 2450 1100
Wire Wire Line
	2450 1100 2450 850 
$Comp
L power:+5V #PWR0105
U 1 1 5F0B2688
P 2000 850
F 0 "#PWR0105" H 2000 700 50  0001 C CNN
F 1 "+5V" H 2015 1023 50  0000 C CNN
F 2 "" H 2000 850 50  0001 C CNN
F 3 "" H 2000 850 50  0001 C CNN
	1    2000 850 
	1    0    0    -1  
$EndComp
Connection ~ 2000 850 
$Comp
L Device:D_Schottky D1
U 1 1 5F0B4100
P 2800 1100
F 0 "D1" H 2800 883 50  0000 C CNN
F 1 "D_Schottky" H 2800 974 50  0000 C CNN
F 2 "Diode_SMD:D_SMA_Handsoldering" H 2800 1100 50  0001 C CNN
F 3 "~" H 2800 1100 50  0001 C CNN
	1    2800 1100
	-1   0    0    1   
$EndComp
$Comp
L power:+5V #PWR0106
U 1 1 5F0B4DD7
P 3100 850
F 0 "#PWR0106" H 3100 700 50  0001 C CNN
F 1 "+5V" H 3115 1023 50  0000 C CNN
F 2 "" H 3100 850 50  0001 C CNN
F 3 "" H 3100 850 50  0001 C CNN
	1    3100 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1100 3100 850 
Connection ~ 3100 1100
Wire Wire Line
	2450 1100 2650 1100
Connection ~ 2450 1100
Wire Wire Line
	2950 1100 3100 1100
$Comp
L power:+3.3V #PWR0107
U 1 1 5F0B902F
P 4550 850
F 0 "#PWR0107" H 4550 700 50  0001 C CNN
F 1 "+3.3V" H 4565 1023 50  0000 C CNN
F 2 "" H 4550 850 50  0001 C CNN
F 3 "" H 4550 850 50  0001 C CNN
	1    4550 850 
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1100 4550 850 
$Comp
L power:GND #PWR0108
U 1 1 5F0BA267
P 4550 1650
F 0 "#PWR0108" H 4550 1400 50  0001 C CNN
F 1 "GND" H 4555 1477 50  0000 C CNN
F 2 "" H 4550 1650 50  0001 C CNN
F 3 "" H 4550 1650 50  0001 C CNN
	1    4550 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1500 4550 1650
Wire Wire Line
	4350 1500 4550 1500
Wire Wire Line
	4350 1100 4550 1100
$Comp
L Device:C C2
U 1 1 5F0BCC65
P 4550 1300
F 0 "C2" H 4665 1346 50  0000 L CNN
F 1 "10u" H 4665 1255 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 4588 1150 50  0001 C CNN
F 3 "~" H 4550 1300 50  0001 C CNN
	1    4550 1300
	1    0    0    -1  
$EndComp
Wire Wire Line
	4550 1100 4550 1150
Connection ~ 4550 1100
Wire Wire Line
	4550 1500 4550 1450
Connection ~ 4550 1500
$Comp
L Device:C C1
U 1 1 5F0BD6CB
P 3100 1450
F 0 "C1" H 3215 1496 50  0000 L CNN
F 1 "1uf" H 3215 1405 50  0000 L CNN
F 2 "Capacitor_SMD:C_0603_1608Metric_Pad1.05x0.95mm_HandSolder" H 3138 1300 50  0001 C CNN
F 3 "~" H 3100 1450 50  0001 C CNN
	1    3100 1450
	1    0    0    -1  
$EndComp
Connection ~ 3100 1300
$Comp
L power:GND #PWR0109
U 1 1 5F0BDCA8
P 3100 1650
F 0 "#PWR0109" H 3100 1400 50  0001 C CNN
F 1 "GND" H 3105 1477 50  0000 C CNN
F 2 "" H 3100 1650 50  0001 C CNN
F 3 "" H 3100 1650 50  0001 C CNN
	1    3100 1650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3100 1650 3100 1600
$Comp
L ESP8266-12E_ESP-12E:ESP8266-12E_ESP-12E U3
U 1 1 5F0CDD87
P 3350 3750
F 0 "U3" H 3350 4617 50  0000 C CNN
F 1 "ESP8266-12E_ESP-12E" H 3350 4526 50  0000 C CNN
F 2 "XCVR_ESP8266-12E/ESP-12E" H 3350 3750 50  0001 L BNN
F 3 "AI-Thinker" H 3350 3750 50  0001 L BNN
	1    3350 3750
	1    0    0    -1  
$EndComp
$Comp
L power:GND #PWR0110
U 1 1 5F0CFC1C
P 4100 4500
F 0 "#PWR0110" H 4100 4250 50  0001 C CNN
F 1 "GND" H 4105 4327 50  0000 C CNN
F 2 "" H 4100 4500 50  0001 C CNN
F 3 "" H 4100 4500 50  0001 C CNN
	1    4100 4500
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR0111
U 1 1 5F0D0230
P 4100 3100
F 0 "#PWR0111" H 4100 2950 50  0001 C CNN
F 1 "+3.3V" H 4115 3273 50  0000 C CNN
F 2 "" H 4100 3100 50  0001 C CNN
F 3 "" H 4100 3100 50  0001 C CNN
	1    4100 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	4050 3150 4100 3150
Wire Wire Line
	4100 3150 4100 3100
Wire Wire Line
	4050 4450 4100 4450
Wire Wire Line
	4100 4450 4100 4500
Wire Wire Line
	5350 3650 5400 3650
$Comp
L power:GND #PWR0112
U 1 1 5F0D32A0
P 5400 3700
F 0 "#PWR0112" H 5400 3450 50  0001 C CNN
F 1 "GND" H 5405 3527 50  0000 C CNN
F 2 "" H 5400 3700 50  0001 C CNN
F 3 "" H 5400 3700 50  0001 C CNN
	1    5400 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	5400 3700 5400 3650
Text GLabel 5450 3450 2    50   Input ~ 0
RX
Text GLabel 5450 3350 2    50   Input ~ 0
TX
Wire Wire Line
	5350 3350 5450 3350
Wire Wire Line
	5350 3450 5450 3450
$Comp
L Connector:Conn_01x04_Male J2
U 1 1 5F0D57F9
P 5150 3450
F 0 "J2" H 5258 3731 50  0000 C CNN
F 1 "Conn_01x04_Male" H 5258 3640 50  0000 C CNN
F 2 "Connector_PinSocket_2.54mm:PinSocket_1x04_P2.54mm_Vertical" H 5150 3450 50  0001 C CNN
F 3 "~" H 5150 3450 50  0001 C CNN
	1    5150 3450
	1    0    0    -1  
$EndComp
Wire Wire Line
	2650 3350 2600 3350
Wire Wire Line
	4050 3450 4100 3450
Text GLabel 4100 3450 2    50   Input ~ 0
TX
Text GLabel 2600 3350 0    50   Input ~ 0
RX
Text GLabel 2600 3950 0    50   Input ~ 0
BOOT
Wire Wire Line
	2650 3950 2600 3950
Wire Wire Line
	2600 3250 2650 3250
Text GLabel 2600 3250 0    50   Input ~ 0
EN
Text GLabel 2600 3150 0    50   Input ~ 0
RST
Wire Wire Line
	2600 3150 2650 3150
$Comp
L Device:R_Pack04 RN?
U 1 1 5F0C1BCC
P 7900 3350
F 0 "RN?" H 8088 3396 50  0000 L CNN
F 1 "R_Pack04" H 8088 3305 50  0000 L CNN
F 2 "" V 8175 3350 50  0001 C CNN
F 3 "~" H 7900 3350 50  0001 C CNN
	1    7900 3350
	1    0    0    -1  
$EndComp
$Comp
L power:+3.3V #PWR?
U 1 1 5F0C363C
P 7850 3050
F 0 "#PWR?" H 7850 2900 50  0001 C CNN
F 1 "+3.3V" H 7865 3223 50  0000 C CNN
F 2 "" H 7850 3050 50  0001 C CNN
F 3 "" H 7850 3050 50  0001 C CNN
	1    7850 3050
	1    0    0    -1  
$EndComp
Wire Wire Line
	7700 3150 7700 3050
Wire Wire Line
	7700 3050 7800 3050
Wire Wire Line
	8000 3050 8000 3150
Connection ~ 7850 3050
Wire Wire Line
	7850 3050 7900 3050
Wire Wire Line
	7800 3150 7800 3050
Connection ~ 7800 3050
Wire Wire Line
	7800 3050 7850 3050
Wire Wire Line
	7900 3150 7900 3050
Connection ~ 7900 3050
Wire Wire Line
	7900 3050 8000 3050
$EndSCHEMATC
