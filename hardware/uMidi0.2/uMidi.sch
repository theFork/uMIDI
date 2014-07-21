EESchema Schematic File Version 2
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:uMidi-cache
EELAYER 24 0
EELAYER END
$Descr A4 11693 8268
encoding utf-8
Sheet 1 1
Title "uMIDI 0.2"
Date "18 Jul 2014"
Rev "A"
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L ATXMEGA128A4U-M IC2
U 1 1 53C6C32C
P 7200 3750
F 0 "IC2" H 6450 4950 40  0000 L BNN
F 1 "ATXMEGA128A4U-M" H 7550 2500 40  0000 L BNN
F 2 "VQFN44" H 7200 3750 35  0000 C CIN
F 3 "" H 7200 3750 60  0000 C CNN
	1    7200 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR26
U 1 1 53C6C394
P 7200 5250
F 0 "#PWR26" H 7200 5250 30  0001 C CNN
F 1 "GND" H 7200 5180 30  0001 C CNN
F 2 "" H 7200 5250 60  0000 C CNN
F 3 "" H 7200 5250 60  0000 C CNN
	1    7200 5250
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR36
U 1 1 53C6C3CA
P 9250 1800
F 0 "#PWR36" H 9250 1760 30  0001 C CNN
F 1 "+3.3V" H 9250 1910 30  0000 C CNN
F 2 "" H 9250 1800 60  0000 C CNN
F 3 "" H 9250 1800 60  0000 C CNN
	1    9250 1800
	1    0    0    -1  
$EndComp
$Comp
L R R9
U 1 1 53C6C462
P 6150 2100
F 0 "R9" V 6230 2100 40  0000 C CNN
F 1 "10k" V 6157 2101 40  0000 C CNN
F 2 "" V 6080 2100 30  0000 C CNN
F 3 "" H 6150 2100 30  0000 C CNN
	1    6150 2100
	1    0    0    -1  
$EndComp
$Comp
L CONN_6 P6
U 1 1 53C6CF68
P 5350 2600
F 0 "P6" V 5300 2600 60  0000 C CNN
F 1 "PDI" V 5400 2600 60  0000 C CNN
F 2 "" H 5350 2600 60  0000 C CNN
F 3 "" H 5350 2600 60  0000 C CNN
	1    5350 2600
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR22
U 1 1 53C6D292
P 5750 2900
F 0 "#PWR22" H 5750 2900 30  0001 C CNN
F 1 "GND" H 5750 2830 30  0001 C CNN
F 2 "" H 5750 2900 60  0000 C CNN
F 3 "" H 5750 2900 60  0000 C CNN
	1    5750 2900
	1    0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 4 1 53C6D4D0
P 3500 2200
F 0 "U1" H 3650 2300 40  0000 C CNN
F 1 "74LS14" H 3700 2100 40  0000 C CNN
F 2 "" H 3500 2200 60  0000 C CNN
F 3 "" H 3500 2200 60  0000 C CNN
	4    3500 2200
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 1 1 53C6D4E4
P 2550 2200
F 0 "U1" H 2700 2300 40  0000 C CNN
F 1 "74LS14" H 2750 2100 40  0000 C CNN
F 2 "" H 2550 2200 60  0000 C CNN
F 3 "" H 2550 2200 60  0000 C CNN
	1    2550 2200
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 2 1 53C6D4F8
P 2550 2600
F 0 "U1" H 2700 2700 40  0000 C CNN
F 1 "74LS14" H 2750 2500 40  0000 C CNN
F 2 "" H 2550 2600 60  0000 C CNN
F 3 "" H 2550 2600 60  0000 C CNN
	2    2550 2600
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 5 1 53C6D50C
P 3500 2600
F 0 "U1" H 3650 2700 40  0000 C CNN
F 1 "74LS14" H 3700 2500 40  0000 C CNN
F 2 "" H 3500 2600 60  0000 C CNN
F 3 "" H 3500 2600 60  0000 C CNN
	5    3500 2600
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 6 1 53C6D520
P 3500 3000
F 0 "U1" H 3650 3100 40  0000 C CNN
F 1 "74LS14" H 3700 2900 40  0000 C CNN
F 2 "" H 3500 3000 60  0000 C CNN
F 3 "" H 3500 3000 60  0000 C CNN
	6    3500 3000
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 U1
U 3 1 53C6D534
P 2550 3000
F 0 "U1" H 2700 3100 40  0000 C CNN
F 1 "74LS14" H 2750 2900 40  0000 C CNN
F 2 "" H 2550 3000 60  0000 C CNN
F 3 "" H 2550 3000 60  0000 C CNN
	3    2550 3000
	-1   0    0    -1  
$EndComp
Wire Wire Line
	7200 5150 7200 5250
Wire Wire Line
	7050 5150 7350 5150
Wire Wire Line
	7350 5150 7350 5050
Wire Wire Line
	7050 5150 7050 5050
Connection ~ 7200 5150
Wire Wire Line
	7150 5050 7150 5150
Connection ~ 7150 5150
Wire Wire Line
	7250 5050 7250 5150
Connection ~ 7250 5150
Wire Wire Line
	7200 2350 7200 2450
Wire Wire Line
	7100 2350 7300 2350
Connection ~ 7200 2350
Wire Wire Line
	7300 2350 7300 2450
Wire Wire Line
	6150 2750 6150 2350
Wire Wire Line
	5700 2350 6000 2350
Wire Wire Line
	6000 2350 6000 2850
Wire Wire Line
	6000 2850 6300 2850
Wire Wire Line
	5800 2450 5700 2450
Wire Wire Line
	5700 2750 6300 2750
Connection ~ 6150 2750
Wire Wire Line
	3000 2200 3050 2200
Wire Wire Line
	3050 2600 3000 2600
Wire Wire Line
	3000 3000 3050 3000
$Comp
L GND #PWR14
U 1 1 53C6D732
P 3550 3250
F 0 "#PWR14" H 3550 3250 30  0001 C CNN
F 1 "GND" H 3550 3180 30  0001 C CNN
F 2 "" H 3550 3250 60  0000 C CNN
F 3 "" H 3550 3250 60  0000 C CNN
	1    3550 3250
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 3250 3550 3100
$Comp
L +3.3V #PWR13
U 1 1 53C6D766
P 3550 2000
F 0 "#PWR13" H 3550 1960 30  0001 C CNN
F 1 "+3.3V" H 3550 2110 30  0000 C CNN
F 2 "" H 3550 2000 60  0000 C CNN
F 3 "" H 3550 2000 60  0000 C CNN
	1    3550 2000
	1    0    0    -1  
$EndComp
Wire Wire Line
	3550 2100 3550 2000
Wire Wire Line
	3950 3000 4300 3000
Wire Wire Line
	4900 4750 6300 4750
Wire Wire Line
	4200 4650 6300 4650
$Comp
L USB-MINI-B CON1
U 1 1 53C6D9DB
P 3650 4800
F 0 "CON1" H 3400 5250 60  0000 C CNN
F 1 "USB-MINI-B" H 3600 4300 60  0000 C CNN
F 2 "" H 3650 4800 60  0000 C CNN
F 3 "" H 3650 4800 60  0000 C CNN
	1    3650 4800
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3100 4500 3000 4500
Wire Wire Line
	3000 4500 3000 5100
Wire Wire Line
	3000 5100 3100 5100
Wire Wire Line
	3100 4950 3000 4950
Connection ~ 3000 4950
Wire Wire Line
	3100 4650 3000 4650
Connection ~ 3000 4650
$Comp
L GND #PWR16
U 1 1 53C6DAEC
P 4450 5200
F 0 "#PWR16" H 4450 5200 30  0001 C CNN
F 1 "GND" H 4450 5130 30  0001 C CNN
F 2 "" H 4450 5200 60  0000 C CNN
F 3 "" H 4450 5200 60  0000 C CNN
	1    4450 5200
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3 JP1
U 1 1 53C945F1
P 4200 2600
F 0 "JP1" H 4250 2500 40  0000 L CNN
F 1 "JUMPER3" H 4200 2700 40  0000 C CNN
F 2 "" H 4200 2600 60  0000 C CNN
F 3 "" H 4200 2600 60  0000 C CNN
	1    4200 2600
	0    1    1    0   
$EndComp
Wire Wire Line
	3950 2600 4100 2600
Wire Wire Line
	4200 2850 4200 3000
Connection ~ 4200 3000
Wire Wire Line
	4200 1500 4200 2350
Wire Wire Line
	4200 2200 3950 2200
$Comp
L R R2
U 1 1 53C9474B
P 1800 2200
F 0 "R2" V 1880 2200 40  0000 C CNN
F 1 "220" V 1807 2201 40  0000 C CNN
F 2 "" V 1730 2200 30  0000 C CNN
F 3 "" H 1800 2200 30  0000 C CNN
	1    1800 2200
	0    1    1    0   
$EndComp
$Comp
L R R4
U 1 1 53C94767
P 1800 2600
F 0 "R4" V 1880 2600 40  0000 C CNN
F 1 "220" V 1807 2601 40  0000 C CNN
F 2 "" V 1730 2600 30  0000 C CNN
F 3 "" H 1800 2600 30  0000 C CNN
	1    1800 2600
	0    1    1    0   
$EndComp
$Comp
L R R6
U 1 1 53C94772
P 1800 3000
F 0 "R6" V 1880 3000 40  0000 C CNN
F 1 "220" V 1807 3001 40  0000 C CNN
F 2 "" V 1730 3000 30  0000 C CNN
F 3 "" H 1800 3000 30  0000 C CNN
	1    1800 3000
	0    1    1    0   
$EndComp
Wire Wire Line
	2050 2200 2100 2200
Wire Wire Line
	2050 2600 2100 2600
Wire Wire Line
	2050 3000 2100 3000
$Comp
L R R1
U 1 1 53C9484F
P 1800 2000
F 0 "R1" V 1880 2000 40  0000 C CNN
F 1 "220" V 1807 2001 40  0000 C CNN
F 2 "" V 1730 2000 30  0000 C CNN
F 3 "" H 1800 2000 30  0000 C CNN
	1    1800 2000
	0    1    1    0   
$EndComp
$Comp
L R R3
U 1 1 53C9485A
P 1800 2400
F 0 "R3" V 1880 2400 40  0000 C CNN
F 1 "220" V 1807 2401 40  0000 C CNN
F 2 "" V 1730 2400 30  0000 C CNN
F 3 "" H 1800 2400 30  0000 C CNN
	1    1800 2400
	0    1    1    0   
$EndComp
$Comp
L R R5
U 1 1 53C94865
P 1800 2800
F 0 "R5" V 1880 2800 40  0000 C CNN
F 1 "220" V 1807 2801 40  0000 C CNN
F 2 "" V 1730 2800 30  0000 C CNN
F 3 "" H 1800 2800 30  0000 C CNN
	1    1800 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	1500 2000 1550 2000
Wire Wire Line
	1500 2200 1550 2200
Wire Wire Line
	1500 3000 1550 3000
Wire Wire Line
	1500 2800 1550 2800
Wire Wire Line
	1500 2600 1550 2600
Wire Wire Line
	1500 2400 1550 2400
$Comp
L +3.3V #PWR2
U 1 1 53C94936
P 2150 2000
F 0 "#PWR2" H 2150 1960 30  0001 C CNN
F 1 "+3.3V" H 2150 2110 30  0000 C CNN
F 2 "" H 2150 2000 60  0000 C CNN
F 3 "" H 2150 2000 60  0000 C CNN
	1    2150 2000
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR3
U 1 1 53C94941
P 2150 2400
F 0 "#PWR3" H 2150 2360 30  0001 C CNN
F 1 "+3.3V" H 2150 2510 30  0000 C CNN
F 2 "" H 2150 2400 60  0000 C CNN
F 3 "" H 2150 2400 60  0000 C CNN
	1    2150 2400
	0    1    1    0   
$EndComp
$Comp
L +3.3V #PWR4
U 1 1 53C9494C
P 2150 2800
F 0 "#PWR4" H 2150 2760 30  0001 C CNN
F 1 "+3.3V" H 2150 2910 30  0000 C CNN
F 2 "" H 2150 2800 60  0000 C CNN
F 3 "" H 2150 2800 60  0000 C CNN
	1    2150 2800
	0    1    1    0   
$EndComp
Wire Wire Line
	2150 2800 2050 2800
Wire Wire Line
	2150 2400 2050 2400
Wire Wire Line
	2150 2000 2050 2000
$Comp
L CONN_2 P4
U 1 1 53C94B1F
P 1150 2900
F 0 "P4" V 1100 2900 40  0000 C CNN
F 1 "MIDI Out" V 1200 2900 40  0000 C CNN
F 2 "" H 1150 2900 60  0000 C CNN
F 3 "" H 1150 2900 60  0000 C CNN
	1    1150 2900
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 53C94B31
P 1150 2100
F 0 "P2" V 1100 2100 40  0000 C CNN
F 1 "MIDI Thru" V 1200 2100 40  0000 C CNN
F 2 "" H 1150 2100 60  0000 C CNN
F 3 "" H 1150 2100 60  0000 C CNN
	1    1150 2100
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 53C94B3C
P 1150 2500
F 0 "P3" V 1100 2500 40  0000 C CNN
F 1 "MIDI T/O" V 1200 2500 40  0000 C CNN
F 2 "" H 1150 2500 60  0000 C CNN
F 3 "" H 1150 2500 60  0000 C CNN
	1    1150 2500
	-1   0    0    -1  
$EndComp
$Comp
L 6N135 IC1
U 1 1 53C94FAA
P 2850 1450
F 0 "IC1" H 2640 1680 40  0000 C CNN
F 1 "6N138" H 3010 1210 40  0000 C CNN
F 2 "SMD-8" H 2650 1220 30  0000 C CIN
F 3 "" H 2850 1450 60  0000 C CNN
	1    2850 1450
	1    0    0    -1  
$EndComp
$Comp
L R R7
U 1 1 53C94FCB
P 1850 1200
F 0 "R7" V 1930 1200 40  0000 C CNN
F 1 "220" V 1857 1201 40  0000 C CNN
F 2 "" V 1780 1200 30  0000 C CNN
F 3 "" H 1850 1200 30  0000 C CNN
	1    1850 1200
	0    1    1    0   
$EndComp
$Comp
L DIODE D3
U 1 1 53C950D4
P 2250 1450
F 0 "D3" H 2250 1550 40  0000 C CNN
F 1 "DIODE" H 2250 1350 40  0000 C CNN
F 2 "" H 2250 1450 60  0000 C CNN
F 3 "" H 2250 1450 60  0000 C CNN
	1    2250 1450
	0    -1   -1   0   
$EndComp
Wire Wire Line
	1550 1700 2450 1700
Wire Wire Line
	2450 1700 2450 1550
Wire Wire Line
	2450 1550 2500 1550
Wire Wire Line
	2250 1700 2250 1650
Wire Wire Line
	1550 1700 1550 1550
Wire Wire Line
	1550 1550 1500 1550
Connection ~ 2250 1700
Wire Wire Line
	2100 1200 2450 1200
Wire Wire Line
	2450 1200 2450 1350
Wire Wire Line
	2450 1350 2500 1350
Wire Wire Line
	2250 1200 2250 1250
Connection ~ 2250 1200
$Comp
L CONN_2 P1
U 1 1 53C94EC8
P 1150 1450
F 0 "P1" V 1100 1450 40  0000 C CNN
F 1 "MIDI In" V 1200 1450 40  0000 C CNN
F 2 "" H 1150 1450 60  0000 C CNN
F 3 "" H 1150 1450 60  0000 C CNN
	1    1150 1450
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1500 1350 1550 1350
Wire Wire Line
	1550 1350 1550 1200
Wire Wire Line
	1550 1200 1600 1200
$Comp
L +3.3V #PWR9
U 1 1 53C95524
P 3300 1150
F 0 "#PWR9" H 3300 1110 30  0001 C CNN
F 1 "+3.3V" H 3300 1260 30  0000 C CNN
F 2 "" H 3300 1150 60  0000 C CNN
F 3 "" H 3300 1150 60  0000 C CNN
	1    3300 1150
	1    0    0    -1  
$EndComp
$Comp
L R R8
U 1 1 53C9552F
P 3600 1200
F 0 "R8" V 3680 1200 40  0000 C CNN
F 1 "270" V 3607 1201 40  0000 C CNN
F 2 "" V 3530 1200 30  0000 C CNN
F 3 "" H 3600 1200 30  0000 C CNN
	1    3600 1200
	-1   0    0    1   
$EndComp
Wire Wire Line
	3200 1300 3300 1300
Wire Wire Line
	3300 1300 3300 1150
$Comp
L +3.3V #PWR15
U 1 1 53C956B4
P 3600 900
F 0 "#PWR15" H 3600 860 30  0001 C CNN
F 1 "+3.3V" H 3600 1010 30  0000 C CNN
F 2 "" H 3600 900 60  0000 C CNN
F 3 "" H 3600 900 60  0000 C CNN
	1    3600 900 
	1    0    0    -1  
$EndComp
Wire Wire Line
	3200 1500 4300 1500
Wire Wire Line
	3600 1500 3600 1450
Wire Wire Line
	3600 950  3600 900 
$Comp
L GND #PWR10
U 1 1 53C9577E
P 3300 1700
F 0 "#PWR10" H 3300 1700 30  0001 C CNN
F 1 "GND" H 3300 1630 30  0001 C CNN
F 2 "" H 3300 1700 60  0000 C CNN
F 3 "" H 3300 1700 60  0000 C CNN
	1    3300 1700
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 1700 3300 1600
Wire Wire Line
	3300 1600 3200 1600
Connection ~ 3600 1500
Connection ~ 4200 1500
Connection ~ 4200 2200
Text GLabel 4300 1500 2    60   Output ~ 0
RXD
Text GLabel 6200 3800 0    60   Input ~ 0
RXD
Text GLabel 4300 3000 2    60   Input ~ 0
TXD
Text GLabel 6200 3950 0    60   Output ~ 0
TXD
$Comp
L C C7
U 1 1 53CA9538
P 6450 2100
F 0 "C7" H 6450 2200 40  0000 L CNN
F 1 "100n" H 6456 2015 40  0000 L CNN
F 2 "" H 6488 1950 30  0000 C CNN
F 3 "" H 6450 2100 60  0000 C CNN
	1    6450 2100
	1    0    0    -1  
$EndComp
$Comp
L C C8
U 1 1 53CA95D0
P 6650 2100
F 0 "C8" H 6650 2200 40  0000 L CNN
F 1 "100n" H 6656 2015 40  0000 L CNN
F 2 "" H 6688 1950 30  0000 C CNN
F 3 "" H 6650 2100 60  0000 C CNN
	1    6650 2100
	1    0    0    -1  
$EndComp
$Comp
L C C9
U 1 1 53CA95DB
P 6850 2100
F 0 "C9" H 6850 2200 40  0000 L CNN
F 1 "100n" H 6856 2015 40  0000 L CNN
F 2 "" H 6888 1950 30  0000 C CNN
F 3 "" H 6850 2100 60  0000 C CNN
	1    6850 2100
	1    0    0    -1  
$EndComp
$Comp
L C C12
U 1 1 53CA95EC
P 8550 2100
F 0 "C12" H 8550 2200 40  0000 L CNN
F 1 "10u" H 8556 2015 40  0000 L CNN
F 2 "" H 8588 1950 30  0000 C CNN
F 3 "" H 8550 2100 60  0000 C CNN
	1    8550 2100
	1    0    0    -1  
$EndComp
Wire Wire Line
	6850 1850 6850 1900
Connection ~ 6850 1850
Wire Wire Line
	6650 1850 6650 1900
Connection ~ 6650 1850
Wire Wire Line
	6450 1900 6450 1850
$Comp
L GND #PWR23
U 1 1 53CA9815
P 6450 2350
F 0 "#PWR23" H 6450 2350 30  0001 C CNN
F 1 "GND" H 6450 2280 30  0001 C CNN
F 2 "" H 6450 2350 60  0000 C CNN
F 3 "" H 6450 2350 60  0000 C CNN
	1    6450 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR24
U 1 1 53CA9834
P 6650 2350
F 0 "#PWR24" H 6650 2350 30  0001 C CNN
F 1 "GND" H 6650 2280 30  0001 C CNN
F 2 "" H 6650 2350 60  0000 C CNN
F 3 "" H 6650 2350 60  0000 C CNN
	1    6650 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR25
U 1 1 53CA983F
P 6850 2350
F 0 "#PWR25" H 6850 2350 30  0001 C CNN
F 1 "GND" H 6850 2280 30  0001 C CNN
F 2 "" H 6850 2350 60  0000 C CNN
F 3 "" H 6850 2350 60  0000 C CNN
	1    6850 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR29
U 1 1 53CA984A
P 8550 2350
F 0 "#PWR29" H 8550 2350 30  0001 C CNN
F 1 "GND" H 8550 2280 30  0001 C CNN
F 2 "" H 8550 2350 60  0000 C CNN
F 3 "" H 8550 2350 60  0000 C CNN
	1    8550 2350
	1    0    0    -1  
$EndComp
Wire Wire Line
	8550 2350 8550 2300
$Comp
L INDUCTOR L1
U 1 1 53CA9BEA
P 8200 1850
F 0 "L1" V 8150 1850 40  0000 C CNN
F 1 "Bead" V 8300 1850 40  0000 C CNN
F 2 "" H 8200 1850 60  0000 C CNN
F 3 "" H 8200 1850 60  0000 C CNN
	1    8200 1850
	0    1    1    0   
$EndComp
$Comp
L C C11
U 1 1 53CA9C01
P 7850 2100
F 0 "C11" H 7850 2200 40  0000 L CNN
F 1 "10u" H 7856 2015 40  0000 L CNN
F 2 "" H 7888 1950 30  0000 C CNN
F 3 "" H 7850 2100 60  0000 C CNN
	1    7850 2100
	1    0    0    -1  
$EndComp
$Comp
L C C10
U 1 1 53CA9C0C
P 7650 2100
F 0 "C10" H 7650 2200 40  0000 L CNN
F 1 "100n" H 7656 2015 40  0000 L CNN
F 2 "" H 7688 1950 30  0000 C CNN
F 3 "" H 7650 2100 60  0000 C CNN
	1    7650 2100
	1    0    0    -1  
$EndComp
Connection ~ 7100 2350
Wire Wire Line
	7500 1850 7500 2450
Wire Wire Line
	7500 1850 7900 1850
Wire Wire Line
	7850 1900 7850 1850
Connection ~ 7850 1850
Wire Wire Line
	7650 1900 7650 1850
Connection ~ 7650 1850
$Comp
L GND #PWR27
U 1 1 53CA9EF0
P 7650 2350
F 0 "#PWR27" H 7650 2350 30  0001 C CNN
F 1 "GND" H 7650 2280 30  0001 C CNN
F 2 "" H 7650 2350 60  0000 C CNN
F 3 "" H 7650 2350 60  0000 C CNN
	1    7650 2350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR28
U 1 1 53CA9EFB
P 7850 2350
F 0 "#PWR28" H 7850 2350 30  0001 C CNN
F 1 "GND" H 7850 2280 30  0001 C CNN
F 2 "" H 7850 2350 60  0000 C CNN
F 3 "" H 7850 2350 60  0000 C CNN
	1    7850 2350
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L2
U 1 1 53CA9FF7
P 8900 1850
F 0 "L2" V 8850 1850 40  0000 C CNN
F 1 "10u" V 9000 1850 40  0000 C CNN
F 2 "" H 8900 1850 60  0000 C CNN
F 3 "" H 8900 1850 60  0000 C CNN
	1    8900 1850
	0    1    1    0   
$EndComp
Wire Wire Line
	6450 1850 7100 1850
Wire Wire Line
	7100 1700 7100 2450
Wire Wire Line
	6850 2350 6850 2300
Wire Wire Line
	6650 2350 6650 2300
Wire Wire Line
	6450 2350 6450 2300
Connection ~ 7100 1850
Wire Wire Line
	7650 2350 7650 2300
Wire Wire Line
	7850 2350 7850 2300
Wire Wire Line
	9200 1850 9250 1850
Wire Wire Line
	9250 1850 9250 1800
Wire Wire Line
	8500 1850 8600 1850
Wire Wire Line
	8550 1700 8550 1900
Connection ~ 8550 1850
Wire Wire Line
	5800 1700 8550 1700
Wire Wire Line
	6150 1700 6150 1850
Connection ~ 6150 1700
Connection ~ 7100 1700
$Comp
L LT1129CST-3.3 U2
U 1 1 53CAB726
P 2750 6100
F 0 "U2" H 2500 6300 40  0000 C CNN
F 1 "LT1129CST-3.3" H 2900 6300 40  0000 C CNN
F 2 "SOT-223" H 2750 6200 35  0000 C CIN
F 3 "" H 2750 6100 60  0000 C CNN
	1    2750 6100
	1    0    0    -1  
$EndComp
Text GLabel 5150 4450 1    60   Output ~ 0
VBUS
Text GLabel 2100 5450 1    60   Input ~ 0
VBUS
$Comp
L DIODE D2
U 1 1 53CAB822
P 2100 5750
F 0 "D2" H 2100 5850 40  0000 C CNN
F 1 "DIODE" H 2100 5650 40  0000 C CNN
F 2 "" H 2100 5750 60  0000 C CNN
F 3 "" H 2100 5750 60  0000 C CNN
	1    2100 5750
	0    1    1    0   
$EndComp
Wire Wire Line
	2100 5550 2100 5450
Wire Wire Line
	2100 5950 2100 6050
Wire Wire Line
	2000 6050 2300 6050
$Comp
L GND #PWR8
U 1 1 53CAB9F8
P 2750 6650
F 0 "#PWR8" H 2750 6650 30  0001 C CNN
F 1 "GND" H 2750 6580 30  0001 C CNN
F 2 "" H 2750 6650 60  0000 C CNN
F 3 "" H 2750 6650 60  0000 C CNN
	1    2750 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2750 6650 2750 6400
$Comp
L C C4
U 1 1 53CABAC9
P 3300 6350
F 0 "C4" H 3300 6450 40  0000 L CNN
F 1 "10u" H 3306 6265 40  0000 L CNN
F 2 "" H 3338 6200 30  0000 C CNN
F 3 "" H 3300 6350 60  0000 C CNN
	1    3300 6350
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR12
U 1 1 53CABAD4
P 3300 6650
F 0 "#PWR12" H 3300 6650 30  0001 C CNN
F 1 "GND" H 3300 6580 30  0001 C CNN
F 2 "" H 3300 6650 60  0000 C CNN
F 3 "" H 3300 6650 60  0000 C CNN
	1    3300 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	3300 6650 3300 6550
Wire Wire Line
	3300 5950 3300 6150
Wire Wire Line
	3300 6050 3200 6050
$Comp
L +3.3V #PWR11
U 1 1 53CABBFC
P 3300 5950
F 0 "#PWR11" H 3300 5910 30  0001 C CNN
F 1 "+3.3V" H 3300 6060 30  0000 C CNN
F 2 "" H 3300 5950 60  0000 C CNN
F 3 "" H 3300 5950 60  0000 C CNN
	1    3300 5950
	1    0    0    -1  
$EndComp
Connection ~ 3300 6050
$Comp
L C C1
U 1 1 53CAA38D
P 2200 6350
F 0 "C1" H 2200 6450 40  0000 L CNN
F 1 "10u" H 2206 6265 40  0000 L CNN
F 2 "" H 2238 6200 30  0000 C CNN
F 3 "" H 2200 6350 60  0000 C CNN
	1    2200 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 6050 2200 6150
Connection ~ 2200 6050
$Comp
L GND #PWR5
U 1 1 53CAA437
P 2200 6650
F 0 "#PWR5" H 2200 6650 30  0001 C CNN
F 1 "GND" H 2200 6580 30  0001 C CNN
F 2 "" H 2200 6650 60  0000 C CNN
F 3 "" H 2200 6650 60  0000 C CNN
	1    2200 6650
	1    0    0    -1  
$EndComp
Wire Wire Line
	2200 6650 2200 6550
$Comp
L DIODE D1
U 1 1 53CAA4CA
P 1800 6050
F 0 "D1" H 1800 6150 40  0000 C CNN
F 1 "DIODE" H 1800 5950 40  0000 C CNN
F 2 "" H 1800 6050 60  0000 C CNN
F 3 "" H 1800 6050 60  0000 C CNN
	1    1800 6050
	1    0    0    -1  
$EndComp
Connection ~ 2100 6050
$Comp
L CONN_2 P5
U 1 1 53CAA6B4
P 1150 6150
F 0 "P5" V 1100 6150 40  0000 C CNN
F 1 "5..12 V" V 1200 6150 40  0000 C CNN
F 2 "" H 1150 6150 60  0000 C CNN
F 3 "" H 1150 6150 60  0000 C CNN
	1    1150 6150
	-1   0    0    -1  
$EndComp
Wire Wire Line
	1500 6050 1600 6050
$Comp
L GND #PWR1
U 1 1 53CAA74E
P 1600 6350
F 0 "#PWR1" H 1600 6350 30  0001 C CNN
F 1 "GND" H 1600 6280 30  0001 C CNN
F 2 "" H 1600 6350 60  0000 C CNN
F 3 "" H 1600 6350 60  0000 C CNN
	1    1600 6350
	1    0    0    -1  
$EndComp
Wire Wire Line
	1600 6350 1600 6250
Wire Wire Line
	1600 6250 1500 6250
$Comp
L C C2
U 1 1 53CAAC0C
P 2300 3700
F 0 "C2" H 2300 3800 40  0000 L CNN
F 1 "100n" H 2306 3615 40  0000 L CNN
F 2 "" H 2338 3550 30  0000 C CNN
F 3 "" H 2300 3700 60  0000 C CNN
	1    2300 3700
	1    0    0    -1  
$EndComp
$Comp
L C C3
U 1 1 53CAAC17
P 2600 3700
F 0 "C3" H 2600 3800 40  0000 L CNN
F 1 "100n" H 2606 3615 40  0000 L CNN
F 2 "" H 2638 3550 30  0000 C CNN
F 3 "" H 2600 3700 60  0000 C CNN
	1    2600 3700
	1    0    0    -1  
$EndComp
Wire Wire Line
	2300 3900 2300 4000
Wire Wire Line
	2300 3500 2300 3400
Wire Wire Line
	2300 3400 2600 3400
Wire Wire Line
	2600 3400 2600 3500
Wire Wire Line
	2300 4000 2600 4000
Wire Wire Line
	2600 4000 2600 3900
$Comp
L GND #PWR7
U 1 1 53CAAE9F
P 2450 4100
F 0 "#PWR7" H 2450 4100 30  0001 C CNN
F 1 "GND" H 2450 4030 30  0001 C CNN
F 2 "" H 2450 4100 60  0000 C CNN
F 3 "" H 2450 4100 60  0000 C CNN
	1    2450 4100
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 4100 2450 4000
Connection ~ 2450 4000
$Comp
L +3.3V #PWR6
U 1 1 53CAAF3E
P 2450 3350
F 0 "#PWR6" H 2450 3310 30  0001 C CNN
F 1 "+3.3V" H 2450 3460 30  0000 C CNN
F 2 "" H 2450 3350 60  0000 C CNN
F 3 "" H 2450 3350 60  0000 C CNN
	1    2450 3350
	1    0    0    -1  
$EndComp
Wire Wire Line
	2450 3350 2450 3400
Connection ~ 2450 3400
$Comp
L CONN_10 P7
U 1 1 53CAB25D
P 9150 3100
F 0 "P7" V 9100 3100 60  0000 C CNN
F 1 "GPIO1" V 9200 3100 60  0000 C CNN
F 2 "" H 9150 3100 60  0000 C CNN
F 3 "" H 9150 3100 60  0000 C CNN
	1    9150 3100
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 2750 8800 2750
Wire Wire Line
	8800 2850 8100 2850
Wire Wire Line
	8100 2950 8800 2950
Wire Wire Line
	8800 3050 8100 3050
Wire Wire Line
	8100 3150 8800 3150
Wire Wire Line
	8800 3250 8100 3250
Wire Wire Line
	8100 3350 8800 3350
Wire Wire Line
	8800 3450 8100 3450
$Comp
L GND #PWR31
U 1 1 53CAB87E
P 8750 3600
F 0 "#PWR31" H 8750 3600 30  0001 C CNN
F 1 "GND" H 8750 3530 30  0001 C CNN
F 2 "" H 8750 3600 60  0000 C CNN
F 3 "" H 8750 3600 60  0000 C CNN
	1    8750 3600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 3600 8750 3550
Wire Wire Line
	8750 3550 8800 3550
$Comp
L +3.3V #PWR30
U 1 1 53CAB929
P 8750 2600
F 0 "#PWR30" H 8750 2560 30  0001 C CNN
F 1 "+3.3V" H 8750 2710 30  0000 C CNN
F 2 "" H 8750 2600 60  0000 C CNN
F 3 "" H 8750 2600 60  0000 C CNN
	1    8750 2600
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 2600 8750 2650
Wire Wire Line
	8750 2650 8800 2650
$Comp
L CONN_10 P8
U 1 1 53CAB9DB
P 9150 4300
F 0 "P8" V 9100 4300 60  0000 C CNN
F 1 "GPIO2" V 9200 4300 60  0000 C CNN
F 2 "" H 9150 4300 60  0000 C CNN
F 3 "" H 9150 4300 60  0000 C CNN
	1    9150 4300
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR32
U 1 1 53CAB9E6
P 8750 3800
F 0 "#PWR32" H 8750 3760 30  0001 C CNN
F 1 "+3.3V" H 8750 3910 30  0000 C CNN
F 2 "" H 8750 3800 60  0000 C CNN
F 3 "" H 8750 3800 60  0000 C CNN
	1    8750 3800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8800 3850 8750 3850
Wire Wire Line
	8750 3850 8750 3800
Wire Wire Line
	8800 3950 8650 3950
Wire Wire Line
	8650 3950 8650 3600
Wire Wire Line
	8650 3600 8100 3600
Wire Wire Line
	8100 3700 8600 3700
Wire Wire Line
	8600 3700 8600 4050
Wire Wire Line
	8600 4050 8800 4050
Wire Wire Line
	8100 3800 8550 3800
Wire Wire Line
	8550 3800 8550 4150
Wire Wire Line
	8550 4150 8800 4150
Wire Wire Line
	8100 3900 8500 3900
Wire Wire Line
	8500 3900 8500 4250
Wire Wire Line
	8500 4250 8800 4250
Wire Wire Line
	8100 4050 8450 4050
Wire Wire Line
	8450 4050 8450 4350
Wire Wire Line
	8450 4350 8800 4350
Wire Wire Line
	8100 4150 8400 4150
Wire Wire Line
	8400 4150 8400 4450
Wire Wire Line
	8400 4450 8800 4450
Wire Wire Line
	8100 4250 8350 4250
Wire Wire Line
	8350 4250 8350 4550
Wire Wire Line
	8350 4550 8800 4550
Wire Wire Line
	8100 4350 8300 4350
Wire Wire Line
	8300 4350 8300 4650
Wire Wire Line
	8300 4650 8800 4650
$Comp
L GND #PWR33
U 1 1 53CAC1E9
P 8750 4800
F 0 "#PWR33" H 8750 4800 30  0001 C CNN
F 1 "GND" H 8750 4730 30  0001 C CNN
F 2 "" H 8750 4800 60  0000 C CNN
F 3 "" H 8750 4800 60  0000 C CNN
	1    8750 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 4800 8750 4750
Wire Wire Line
	8750 4750 8800 4750
$Comp
L CONN_10 P9
U 1 1 53CAC3EE
P 9150 5500
F 0 "P9" V 9100 5500 60  0000 C CNN
F 1 "GPIO2" V 9200 5500 60  0000 C CNN
F 2 "" H 9150 5500 60  0000 C CNN
F 3 "" H 9150 5500 60  0000 C CNN
	1    9150 5500
	1    0    0    -1  
$EndComp
Wire Wire Line
	8100 4450 8250 4450
Wire Wire Line
	8250 4450 8250 5150
Wire Wire Line
	8250 5150 8800 5150
Wire Wire Line
	8100 4550 8200 4550
Wire Wire Line
	8200 4550 8200 5250
Wire Wire Line
	8200 5250 8800 5250
Wire Wire Line
	8100 4650 8150 4650
Wire Wire Line
	8150 4650 8150 5350
Wire Wire Line
	8150 5350 8800 5350
Wire Wire Line
	8100 4750 8100 5450
Wire Wire Line
	8100 5450 8800 5450
Wire Wire Line
	4900 4800 4900 4750
Wire Wire Line
	4200 4800 4900 4800
Wire Wire Line
	4200 5100 4450 5100
Wire Wire Line
	4200 4500 5600 4500
Wire Wire Line
	5150 4500 5150 4450
Wire Wire Line
	6300 4350 6200 4350
Wire Wire Line
	6200 4350 6200 5850
Wire Wire Line
	6200 5850 8800 5850
Wire Wire Line
	6300 4250 6100 4250
Wire Wire Line
	6100 4250 6100 5750
Wire Wire Line
	6100 5750 8800 5750
Wire Wire Line
	6300 4150 6000 4150
Wire Wire Line
	6000 4150 6000 5650
Wire Wire Line
	6000 5650 8800 5650
Wire Wire Line
	8800 5550 5900 5550
Wire Wire Line
	5900 5550 5900 4050
Wire Wire Line
	5900 4050 6300 4050
$Comp
L GND #PWR35
U 1 1 53CAD159
P 8750 6000
F 0 "#PWR35" H 8750 6000 30  0001 C CNN
F 1 "GND" H 8750 5930 30  0001 C CNN
F 2 "" H 8750 6000 60  0000 C CNN
F 3 "" H 8750 6000 60  0000 C CNN
	1    8750 6000
	1    0    0    -1  
$EndComp
$Comp
L +3.3V #PWR34
U 1 1 53CAD164
P 8750 5000
F 0 "#PWR34" H 8750 4960 30  0001 C CNN
F 1 "+3.3V" H 8750 5110 30  0000 C CNN
F 2 "" H 8750 5000 60  0000 C CNN
F 3 "" H 8750 5000 60  0000 C CNN
	1    8750 5000
	1    0    0    -1  
$EndComp
Wire Wire Line
	8750 5000 8750 5050
Wire Wire Line
	8750 5050 8800 5050
Wire Wire Line
	8750 6000 8750 5950
Wire Wire Line
	8750 5950 8800 5950
$Comp
L QUARTZCMS4 X1
U 1 1 53CAD6C6
P 6050 3400
F 0 "X1" H 6050 3550 60  0000 C CNN
F 1 "32MHz" H 6050 3250 60  0000 C CNN
F 2 "" H 6050 3400 60  0000 C CNN
F 3 "" H 6050 3400 60  0000 C CNN
	1    6050 3400
	0    1    1    0   
$EndComp
Wire Wire Line
	6200 3800 6300 3800
Wire Wire Line
	6200 3950 6250 3950
Wire Wire Line
	6250 3950 6250 3900
Wire Wire Line
	6250 3900 6300 3900
Wire Wire Line
	6000 3700 6200 3700
Wire Wire Line
	6200 3700 6200 3500
Wire Wire Line
	6200 3500 6300 3500
Wire Wire Line
	6300 3500 6300 3450
Wire Wire Line
	6300 3350 6300 3300
Wire Wire Line
	6300 3300 6200 3300
Wire Wire Line
	6200 3300 6200 3100
Wire Wire Line
	6200 3100 6000 3100
$Comp
L C C6
U 1 1 53CADAB7
P 5800 3700
F 0 "C6" H 5800 3800 40  0000 L CNN
F 1 "15p" H 5806 3615 40  0000 L CNN
F 2 "" H 5838 3550 30  0000 C CNN
F 3 "" H 5800 3700 60  0000 C CNN
	1    5800 3700
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5800 2450 5800 1700
$Comp
L C C5
U 1 1 53CADE7D
P 5800 3100
F 0 "C5" H 5800 3200 40  0000 L CNN
F 1 "15p" H 5806 3015 40  0000 L CNN
F 2 "" H 5838 2950 30  0000 C CNN
F 3 "" H 5800 3100 60  0000 C CNN
	1    5800 3100
	0    -1   -1   0   
$EndComp
Wire Wire Line
	5700 2850 5750 2850
Wire Wire Line
	5750 2850 5750 2900
Connection ~ 6050 3100
Connection ~ 6050 3700
$Comp
L GND #PWR20
U 1 1 53CAE196
P 5550 3700
F 0 "#PWR20" H 5550 3700 30  0001 C CNN
F 1 "GND" H 5550 3630 30  0001 C CNN
F 2 "" H 5550 3700 60  0000 C CNN
F 3 "" H 5550 3700 60  0000 C CNN
	1    5550 3700
	0    1    1    0   
$EndComp
$Comp
L GND #PWR19
U 1 1 53CAE1A6
P 5550 3100
F 0 "#PWR19" H 5550 3100 30  0001 C CNN
F 1 "GND" H 5550 3030 30  0001 C CNN
F 2 "" H 5550 3100 60  0000 C CNN
F 3 "" H 5550 3100 60  0000 C CNN
	1    5550 3100
	0    1    1    0   
$EndComp
Wire Wire Line
	5550 3100 5600 3100
Wire Wire Line
	5550 3700 5600 3700
$Comp
L TVS D4
U 1 1 53CD4F63
P 5100 5600
F 0 "D4" H 5100 5750 60  0000 C CNN
F 1 "TVS" H 5100 5450 60  0000 C CNN
F 2 "" H 5100 5600 60  0000 C CNN
F 3 "" H 5100 5600 60  0000 C CNN
	1    5100 5600
	0    -1   -1   0   
$EndComp
$Comp
L TVS D5
U 1 1 53CD4F93
P 5350 5350
F 0 "D5" H 5350 5500 60  0000 C CNN
F 1 "TVS" H 5350 5200 60  0000 C CNN
F 2 "" H 5350 5350 60  0000 C CNN
F 3 "" H 5350 5350 60  0000 C CNN
	1    5350 5350
	0    -1   -1   0   
$EndComp
$Comp
L TVS D6
U 1 1 53CD4F9E
P 5600 5100
F 0 "D6" H 5600 5250 60  0000 C CNN
F 1 "TVS" H 5600 4950 60  0000 C CNN
F 2 "" H 5600 5100 60  0000 C CNN
F 3 "" H 5600 5100 60  0000 C CNN
	1    5600 5100
	0    -1   -1   0   
$EndComp
Connection ~ 5150 4500
Connection ~ 5350 4650
Connection ~ 5100 4750
$Comp
L GND #PWR17
U 1 1 53CD5276
P 5100 5950
F 0 "#PWR17" H 5100 5950 30  0001 C CNN
F 1 "GND" H 5100 5880 30  0001 C CNN
F 2 "" H 5100 5950 60  0000 C CNN
F 3 "" H 5100 5950 60  0000 C CNN
	1    5100 5950
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR18
U 1 1 53CD5281
P 5350 5700
F 0 "#PWR18" H 5350 5700 30  0001 C CNN
F 1 "GND" H 5350 5630 30  0001 C CNN
F 2 "" H 5350 5700 60  0000 C CNN
F 3 "" H 5350 5700 60  0000 C CNN
	1    5350 5700
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR21
U 1 1 53CD528C
P 5600 5450
F 0 "#PWR21" H 5600 5450 30  0001 C CNN
F 1 "GND" H 5600 5380 30  0001 C CNN
F 2 "" H 5600 5450 60  0000 C CNN
F 3 "" H 5600 5450 60  0000 C CNN
	1    5600 5450
	1    0    0    -1  
$EndComp
Wire Wire Line
	5600 5450 5600 5400
Wire Wire Line
	5350 5700 5350 5650
Wire Wire Line
	5100 5950 5100 5900
Wire Wire Line
	5100 5300 5100 4750
Wire Wire Line
	5350 4650 5350 5050
Wire Wire Line
	5600 4500 5600 4800
$Comp
L C C?
U 1 1 53CD5DBF
P 4450 4800
F 0 "C?" H 4450 4900 40  0000 L CNN
F 1 "100n" H 4456 4715 40  0000 L CNN
F 2 "" H 4488 4650 30  0000 C CNN
F 3 "" H 4450 4800 60  0000 C CNN
	1    4450 4800
	1    0    0    -1  
$EndComp
Wire Wire Line
	4450 5000 4450 5200
Connection ~ 4450 5100
Wire Wire Line
	4450 4600 4450 4500
Connection ~ 4450 4500
$EndSCHEMATC
