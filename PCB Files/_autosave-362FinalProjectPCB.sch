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
L MCU_ST_STM32F0:STM32F091RCTx U?
U 1 1 61AC2B66
P 5600 3750
F 0 "U?" H 5550 1861 50  0000 C CNN
F 1 "STM32F091RCTx" H 5550 1770 50  0000 C CNN
F 2 "Package_QFP:LQFP-64_10x10mm_P0.5mm" H 5000 2050 50  0001 R CNN
F 3 "http://www.st.com/st-web-ui/static/active/en/resource/technical/document/datasheet/DM00115237.pdf" H 5600 3750 50  0001 C CNN
	1    5600 3750
	1    0    0    -1  
$EndComp
$Comp
L pspice:0 #GND?
U 1 1 61AC7155
P 5500 6000
F 0 "#GND?" H 5500 5900 50  0001 C CNN
F 1 "0" H 5500 6089 50  0000 C CNN
F 2 "" H 5500 6000 50  0001 C CNN
F 3 "~" H 5500 6000 50  0001 C CNN
	1    5500 6000
	1    0    0    -1  
$EndComp
Wire Wire Line
	5300 5550 5300 5650
Wire Wire Line
	5300 5650 5400 5650
Wire Wire Line
	5500 5650 5500 6000
Wire Wire Line
	5700 5550 5700 5650
Wire Wire Line
	5700 5650 5600 5650
Connection ~ 5500 5650
Wire Wire Line
	5400 5550 5400 5650
Wire Wire Line
	5400 5650 5500 5650
Connection ~ 5400 5650
Wire Wire Line
	5500 5550 5500 5650
Wire Wire Line
	5600 5550 5600 5650
Connection ~ 5600 5650
Wire Wire Line
	5600 5650 5500 5650
Wire Wire Line
	5800 1950 5800 1750
Wire Wire Line
	5800 1750 5600 1750
Wire Wire Line
	5300 1750 5300 1950
Wire Wire Line
	5400 1950 5400 1750
Connection ~ 5400 1750
Wire Wire Line
	5400 1750 5300 1750
Wire Wire Line
	5500 1950 5500 1750
Connection ~ 5500 1750
Wire Wire Line
	5500 1750 5400 1750
Wire Wire Line
	5600 1950 5600 1750
Connection ~ 5600 1750
Wire Wire Line
	5600 1750 5500 1750
$Comp
L power:+3V0 #PWR?
U 1 1 61AD0EC2
P 5300 1750
F 0 "#PWR?" H 5300 1600 50  0001 C CNN
F 1 "+3V0" H 5315 1923 50  0000 C CNN
F 2 "" H 5300 1750 50  0001 C CNN
F 3 "" H 5300 1750 50  0001 C CNN
	1    5300 1750
	1    0    0    -1  
$EndComp
Connection ~ 5300 1750
Text Label 5700 1650 0    50   ~ 0
3VA
Wire Wire Line
	5700 1950 5700 1650
$Comp
L Connector_Generic:Conn_02x05_Odd_Even J?
U 1 1 61AD2FBE
P 1400 1300
F 0 "J?" H 1450 1717 50  0000 C CNN
F 1 "Conn_02x05_Odd_Even" H 1450 1626 50  0000 C CNN
F 2 "" H 1400 1300 50  0001 C CNN
F 3 "~" H 1400 1300 50  0001 C CNN
	1    1400 1300
	1    0    0    -1  
$EndComp
$EndSCHEMATC
