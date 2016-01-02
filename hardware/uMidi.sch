EESchema Schematic File Version 2
LIBS:uMidi-rescue
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
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
LIBS:contrib
LIBS:valves
LIBS:regul
LIBS:atmel
LIBS:uMidi-cache
EELAYER 25 0
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
L ATXMEGA128A4U-M IC1
U 1 1 53C6C32C
P 8500 3550
F 0 "IC1" H 7750 4750 40  0000 L BNN
F 1 "ATXMEGA128A4U-M" H 8850 2300 40  0000 L BNN
F 2 "VQFN44" H 8500 3550 35  0000 C CIN
F 3 "" H 8500 3550 60  0000 C CNN
	1    8500 3550
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR01
U 1 1 53C6C394
P 8500 5050
F 0 "#PWR01" H 8500 5050 30  0001 C CNN
F 1 "GND" H 8500 4980 30  0001 C CNN
F 2 "" H 8500 5050 60  0000 C CNN
F 3 "" H 8500 5050 60  0000 C CNN
	1    8500 5050
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR02
U 1 1 53C6C3CA
P 7100 750
F 0 "#PWR02" H 7100 710 30  0001 C CNN
F 1 "+3.3V" H 7100 860 30  0000 C CNN
F 2 "" H 7100 750 60  0000 C CNN
F 3 "" H 7100 750 60  0000 C CNN
	1    7100 750 
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R9
U 1 1 53C6C462
P 7400 1900
F 0 "R9" V 7480 1900 40  0000 C CNN
F 1 "10k" V 7407 1901 40  0000 C CNN
F 2 "" V 7330 1900 30  0000 C CNN
F 3 "" H 7400 1900 30  0000 C CNN
	1    7400 1900
	1    0    0    -1  
$EndComp
$Comp
L CONN_6 P6
U 1 1 53C6CF68
P 6650 2400
F 0 "P6" V 6600 2400 60  0000 C CNN
F 1 "PDI" V 6700 2400 60  0000 C CNN
F 2 "" H 6650 2400 60  0000 C CNN
F 3 "" H 6650 2400 60  0000 C CNN
	1    6650 2400
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR03
U 1 1 53C6D292
P 7050 2700
F 0 "#PWR03" H 7050 2700 30  0001 C CNN
F 1 "GND" H 7050 2630 30  0001 C CNN
F 2 "" H 7050 2700 60  0000 C CNN
F 3 "" H 7050 2700 60  0000 C CNN
	1    7050 2700
	1    0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 4 1 53C6D4D0
P 3150 3100
F 0 "IC2" H 3300 3200 40  0000 C CNN
F 1 "74LS14" H 3350 3000 40  0000 C CNN
F 2 "" H 3150 3100 60  0000 C CNN
F 3 "" H 3150 3100 60  0000 C CNN
	4    3150 3100
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 1 1 53C6D4E4
P 2200 2300
F 0 "IC2" H 2350 2400 40  0000 C CNN
F 1 "74LS14" H 2400 2200 40  0000 C CNN
F 2 "" H 2200 2300 60  0000 C CNN
F 3 "" H 2200 2300 60  0000 C CNN
	1    2200 2300
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 2 1 53C6D4F8
P 2200 2700
F 0 "IC2" H 2350 2800 40  0000 C CNN
F 1 "74LS14" H 2400 2600 40  0000 C CNN
F 2 "" H 2200 2700 60  0000 C CNN
F 3 "" H 2200 2700 60  0000 C CNN
	2    2200 2700
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 5 1 53C6D50C
P 3150 2700
F 0 "IC2" H 3300 2800 40  0000 C CNN
F 1 "74LS14" H 3350 2600 40  0000 C CNN
F 2 "" H 3150 2700 60  0000 C CNN
F 3 "" H 3150 2700 60  0000 C CNN
	5    3150 2700
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 6 1 53C6D520
P 3150 2300
F 0 "IC2" H 3300 2400 40  0000 C CNN
F 1 "74LS14" H 3350 2200 40  0000 C CNN
F 2 "" H 3150 2300 60  0000 C CNN
F 3 "" H 3150 2300 60  0000 C CNN
	6    3150 2300
	-1   0    0    -1  
$EndComp
$Comp
L 74LS14 IC2
U 3 1 53C6D534
P 2200 3100
F 0 "IC2" H 2350 3200 40  0000 C CNN
F 1 "74LS14" H 2400 3000 40  0000 C CNN
F 2 "" H 2200 3100 60  0000 C CNN
F 3 "" H 2200 3100 60  0000 C CNN
	3    2200 3100
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR04
U 1 1 53C6D732
P 3200 3350
F 0 "#PWR04" H 3200 3350 30  0001 C CNN
F 1 "GND" H 3200 3280 30  0001 C CNN
F 2 "" H 3200 3350 60  0000 C CNN
F 3 "" H 3200 3350 60  0000 C CNN
	1    3200 3350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR05
U 1 1 53C6D766
P 3200 2100
F 0 "#PWR05" H 3200 2060 30  0001 C CNN
F 1 "+3.3V" H 3200 2210 30  0000 C CNN
F 2 "" H 3200 2100 60  0000 C CNN
F 3 "" H 3200 2100 60  0000 C CNN
	1    3200 2100
	1    0    0    -1  
$EndComp
$Comp
L USB-MINI-B CON1
U 1 1 53C6D9DB
P 4950 4600
F 0 "CON1" H 5200 4950 60  0000 C CNN
F 1 "USB-MINI-B" H 4950 4250 60  0000 C CNN
F 2 "" H 4950 4600 60  0000 C CNN
F 3 "" H 4950 4600 60  0000 C CNN
	1    4950 4600
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR06
U 1 1 53C6DAEC
P 5750 5800
F 0 "#PWR06" H 5750 5800 30  0001 C CNN
F 1 "GND" H 5750 5730 30  0001 C CNN
F 2 "" H 5750 5800 60  0000 C CNN
F 3 "" H 5750 5800 60  0000 C CNN
	1    5750 5800
	1    0    0    -1  
$EndComp
$Comp
L JUMPER3 JP1
U 1 1 53C945F1
P 3750 2700
F 0 "JP1" H 3800 2600 40  0000 L CNN
F 1 "JUMPER3" H 3750 2800 40  0000 C CNN
F 2 "" H 3750 2700 60  0000 C CNN
F 3 "" H 3750 2700 60  0000 C CNN
	1    3750 2700
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R2
U 1 1 53C9474B
P 1450 2300
F 0 "R2" V 1530 2300 40  0000 C CNN
F 1 "120" V 1457 2301 40  0000 C CNN
F 2 "" V 1380 2300 30  0000 C CNN
F 3 "" H 1450 2300 30  0000 C CNN
	1    1450 2300
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R4
U 1 1 53C94767
P 1450 2700
F 0 "R4" V 1530 2700 40  0000 C CNN
F 1 "120" V 1457 2701 40  0000 C CNN
F 2 "" V 1380 2700 30  0000 C CNN
F 3 "" H 1450 2700 30  0000 C CNN
	1    1450 2700
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R6
U 1 1 53C94772
P 1450 3100
F 0 "R6" V 1530 3100 40  0000 C CNN
F 1 "120" V 1457 3101 40  0000 C CNN
F 2 "" V 1380 3100 30  0000 C CNN
F 3 "" H 1450 3100 30  0000 C CNN
	1    1450 3100
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R1
U 1 1 53C9484F
P 1450 2100
F 0 "R1" V 1530 2100 40  0000 C CNN
F 1 "120" V 1457 2101 40  0000 C CNN
F 2 "" V 1380 2100 30  0000 C CNN
F 3 "" H 1450 2100 30  0000 C CNN
	1    1450 2100
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R3
U 1 1 53C9485A
P 1450 2500
F 0 "R3" V 1530 2500 40  0000 C CNN
F 1 "120" V 1457 2501 40  0000 C CNN
F 2 "" V 1380 2500 30  0000 C CNN
F 3 "" H 1450 2500 30  0000 C CNN
	1    1450 2500
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R5
U 1 1 53C94865
P 1450 2900
F 0 "R5" V 1530 2900 40  0000 C CNN
F 1 "120" V 1457 2901 40  0000 C CNN
F 2 "" V 1380 2900 30  0000 C CNN
F 3 "" H 1450 2900 30  0000 C CNN
	1    1450 2900
	0    1    1    0   
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR07
U 1 1 53C94936
P 1800 2100
F 0 "#PWR07" H 1800 2060 30  0001 C CNN
F 1 "+3.3V" H 1800 2210 30  0000 C CNN
F 2 "" H 1800 2100 60  0000 C CNN
F 3 "" H 1800 2100 60  0000 C CNN
	1    1800 2100
	0    1    1    0   
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR08
U 1 1 53C94941
P 1800 2500
F 0 "#PWR08" H 1800 2460 30  0001 C CNN
F 1 "+3.3V" H 1800 2610 30  0000 C CNN
F 2 "" H 1800 2500 60  0000 C CNN
F 3 "" H 1800 2500 60  0000 C CNN
	1    1800 2500
	0    1    1    0   
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR09
U 1 1 53C9494C
P 1800 2900
F 0 "#PWR09" H 1800 2860 30  0001 C CNN
F 1 "+3.3V" H 1800 3010 30  0000 C CNN
F 2 "" H 1800 2900 60  0000 C CNN
F 3 "" H 1800 2900 60  0000 C CNN
	1    1800 2900
	0    1    1    0   
$EndComp
$Comp
L CONN_2 P4
U 1 1 53C94B1F
P 800 3000
F 0 "P4" V 750 3000 40  0000 C CNN
F 1 "MIDI Out" V 850 3000 40  0000 C CNN
F 2 "" H 800 3000 60  0000 C CNN
F 3 "" H 800 3000 60  0000 C CNN
	1    800  3000
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P2
U 1 1 53C94B31
P 800 2200
F 0 "P2" V 750 2200 40  0000 C CNN
F 1 "MIDI Thru" V 850 2200 40  0000 C CNN
F 2 "" H 800 2200 60  0000 C CNN
F 3 "" H 800 2200 60  0000 C CNN
	1    800  2200
	-1   0    0    -1  
$EndComp
$Comp
L CONN_2 P3
U 1 1 53C94B3C
P 800 2600
F 0 "P3" V 750 2600 40  0000 C CNN
F 1 "MIDI T/O" V 850 2600 40  0000 C CNN
F 2 "" H 800 2600 60  0000 C CNN
F 3 "" H 800 2600 60  0000 C CNN
	1    800  2600
	-1   0    0    -1  
$EndComp
$Comp
L 6N135-RESCUE-uMidi IC4
U 1 1 53C94FAA
P 2350 1300
F 0 "IC4" H 2140 1530 40  0000 C CNN
F 1 "6N138" H 2510 1060 40  0000 C CNN
F 2 "SMD-8" H 2150 1070 30  0000 C CIN
F 3 "" H 2350 1300 60  0000 C CNN
	1    2350 1300
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R7
U 1 1 53C94FCB
P 1500 1050
F 0 "R7" V 1580 1050 40  0000 C CNN
F 1 "220" V 1507 1051 40  0000 C CNN
F 2 "" V 1430 1050 30  0000 C CNN
F 3 "" H 1500 1050 30  0000 C CNN
	1    1500 1050
	0    1    1    0   
$EndComp
$Comp
L DIODE D3
U 1 1 53C950D4
P 1800 1300
F 0 "D3" H 1800 1400 40  0000 C CNN
F 1 "DIODE" H 1800 1200 40  0000 C CNN
F 2 "" H 1800 1300 60  0000 C CNN
F 3 "" H 1800 1300 60  0000 C CNN
	1    1800 1300
	0    -1   -1   0   
$EndComp
$Comp
L CONN_2 P1
U 1 1 53C94EC8
P 800 1300
F 0 "P1" V 750 1300 40  0000 C CNN
F 1 "MIDI In" V 850 1300 40  0000 C CNN
F 2 "" H 800 1300 60  0000 C CNN
F 3 "" H 800 1300 60  0000 C CNN
	1    800  1300
	-1   0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR010
U 1 1 53C95524
P 2750 1100
F 0 "#PWR010" H 2750 1060 30  0001 C CNN
F 1 "+3.3V" H 2750 1210 30  0000 C CNN
F 2 "" H 2750 1100 60  0000 C CNN
F 3 "" H 2750 1100 60  0000 C CNN
	1    2750 1100
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R8
U 1 1 53C9552F
P 3250 1050
F 0 "R8" V 3330 1050 40  0000 C CNN
F 1 "270" V 3257 1051 40  0000 C CNN
F 2 "" V 3180 1050 30  0000 C CNN
F 3 "" H 3250 1050 30  0000 C CNN
	1    3250 1050
	-1   0    0    1   
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR011
U 1 1 53C956B4
P 3250 750
F 0 "#PWR011" H 3250 710 30  0001 C CNN
F 1 "+3.3V" H 3250 860 30  0000 C CNN
F 2 "" H 3250 750 60  0000 C CNN
F 3 "" H 3250 750 60  0000 C CNN
	1    3250 750 
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR012
U 1 1 53C9577E
P 2750 1850
F 0 "#PWR012" H 2750 1850 30  0001 C CNN
F 1 "GND" H 2750 1780 30  0001 C CNN
F 2 "" H 2750 1850 60  0000 C CNN
F 3 "" H 2750 1850 60  0000 C CNN
	1    2750 1850
	1    0    0    -1  
$EndComp
Text GLabel 3850 1600 2    60   Output ~ 0
RXD
Text GLabel 7500 3600 0    60   Input ~ 0
RXD
Text GLabel 3850 3100 2    60   Input ~ 0
TXD
Text GLabel 7500 3750 0    60   Output ~ 0
TXD
$Comp
L C-RESCUE-uMidi C7
U 1 1 53CA9538
P 7850 1900
F 0 "C7" H 7850 2000 40  0000 L CNN
F 1 "100n" H 7856 1815 40  0000 L CNN
F 2 "" H 7888 1750 30  0000 C CNN
F 3 "" H 7850 1900 60  0000 C CNN
	1    7850 1900
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C8
U 1 1 53CA95D0
P 8050 1900
F 0 "C8" H 8050 2000 40  0000 L CNN
F 1 "100n" H 8056 1815 40  0000 L CNN
F 2 "" H 8088 1750 30  0000 C CNN
F 3 "" H 8050 1900 60  0000 C CNN
	1    8050 1900
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C9
U 1 1 53CA95DB
P 8250 1900
F 0 "C9" H 8250 2000 40  0000 L CNN
F 1 "100n" H 8256 1815 40  0000 L CNN
F 2 "" H 8288 1750 30  0000 C CNN
F 3 "" H 8250 1900 60  0000 C CNN
	1    8250 1900
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C12
U 1 1 53CA95EC
P 7650 1900
F 0 "C12" H 7650 2000 40  0000 L CNN
F 1 "10u" H 7656 1815 40  0000 L CNN
F 2 "" H 7688 1750 30  0000 C CNN
F 3 "" H 7650 1900 60  0000 C CNN
	1    7650 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR013
U 1 1 53CA9815
P 7850 2150
F 0 "#PWR013" H 7850 2150 30  0001 C CNN
F 1 "GND" H 7850 2080 30  0001 C CNN
F 2 "" H 7850 2150 60  0000 C CNN
F 3 "" H 7850 2150 60  0000 C CNN
	1    7850 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR014
U 1 1 53CA9834
P 8050 2150
F 0 "#PWR014" H 8050 2150 30  0001 C CNN
F 1 "GND" H 8050 2080 30  0001 C CNN
F 2 "" H 8050 2150 60  0000 C CNN
F 3 "" H 8050 2150 60  0000 C CNN
	1    8050 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR015
U 1 1 53CA983F
P 8250 2150
F 0 "#PWR015" H 8250 2150 30  0001 C CNN
F 1 "GND" H 8250 2080 30  0001 C CNN
F 2 "" H 8250 2150 60  0000 C CNN
F 3 "" H 8250 2150 60  0000 C CNN
	1    8250 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR016
U 1 1 53CA984A
P 7650 2150
F 0 "#PWR016" H 7650 2150 30  0001 C CNN
F 1 "GND" H 7650 2080 30  0001 C CNN
F 2 "" H 7650 2150 60  0000 C CNN
F 3 "" H 7650 2150 60  0000 C CNN
	1    7650 2150
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L1
U 1 1 53CA9BEA
P 8400 1450
F 0 "L1" V 8350 1450 40  0000 C CNN
F 1 "Bead" V 8500 1450 40  0000 C CNN
F 2 "" H 8400 1450 60  0000 C CNN
F 3 "" H 8400 1450 60  0000 C CNN
	1    8400 1450
	0    -1   -1   0   
$EndComp
$Comp
L C-RESCUE-uMidi C11
U 1 1 53CA9C01
P 9150 1900
F 0 "C11" H 9150 2000 40  0000 L CNN
F 1 "10u" H 9156 1815 40  0000 L CNN
F 2 "" H 9188 1750 30  0000 C CNN
F 3 "" H 9150 1900 60  0000 C CNN
	1    9150 1900
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C10
U 1 1 53CA9C0C
P 8950 1900
F 0 "C10" H 8950 2000 40  0000 L CNN
F 1 "100n" H 8956 1815 40  0000 L CNN
F 2 "" H 8988 1750 30  0000 C CNN
F 3 "" H 8950 1900 60  0000 C CNN
	1    8950 1900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR017
U 1 1 53CA9EF0
P 8950 2150
F 0 "#PWR017" H 8950 2150 30  0001 C CNN
F 1 "GND" H 8950 2080 30  0001 C CNN
F 2 "" H 8950 2150 60  0000 C CNN
F 3 "" H 8950 2150 60  0000 C CNN
	1    8950 2150
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR018
U 1 1 53CA9EFB
P 9150 2150
F 0 "#PWR018" H 9150 2150 30  0001 C CNN
F 1 "GND" H 9150 2080 30  0001 C CNN
F 2 "" H 9150 2150 60  0000 C CNN
F 3 "" H 9150 2150 60  0000 C CNN
	1    9150 2150
	1    0    0    -1  
$EndComp
$Comp
L INDUCTOR L2
U 1 1 53CA9FF7
P 7100 1100
F 0 "L2" V 7050 1100 40  0000 C CNN
F 1 "10u" V 7200 1100 40  0000 C CNN
F 2 "" H 7100 1100 60  0000 C CNN
F 3 "" H 7100 1100 60  0000 C CNN
	1    7100 1100
	1    0    0    -1  
$EndComp
$Comp
L LT1129CST-3.3 IC3
U 1 1 53CAB726
P 2400 6800
F 0 "IC3" H 2150 7000 40  0000 C CNN
F 1 "LT1129CST-3.3" H 2550 7000 40  0000 C CNN
F 2 "SOT-223" H 2400 6900 35  0000 C CIN
F 3 "" H 2400 6800 60  0000 C CNN
	1    2400 6800
	1    0    0    -1  
$EndComp
Text GLabel 6450 4350 1    60   Output ~ 0
VBUS
Text GLabel 1750 5950 1    60   Input ~ 0
VBUS
$Comp
L DIODE D2
U 1 1 53CAB822
P 1750 6450
F 0 "D2" H 1750 6550 40  0000 C CNN
F 1 "DIODE" H 1750 6350 40  0000 C CNN
F 2 "" H 1750 6450 60  0000 C CNN
F 3 "" H 1750 6450 60  0000 C CNN
	1    1750 6450
	0    1    1    0   
$EndComp
$Comp
L GND #PWR019
U 1 1 53CAB9F8
P 2400 7350
F 0 "#PWR019" H 2400 7350 30  0001 C CNN
F 1 "GND" H 2400 7280 30  0001 C CNN
F 2 "" H 2400 7350 60  0000 C CNN
F 3 "" H 2400 7350 60  0000 C CNN
	1    2400 7350
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C4
U 1 1 53CABAC9
P 2950 7050
F 0 "C4" H 2950 7150 40  0000 L CNN
F 1 "10u" H 2956 6965 40  0000 L CNN
F 2 "" H 2988 6900 30  0000 C CNN
F 3 "" H 2950 7050 60  0000 C CNN
	1    2950 7050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR020
U 1 1 53CABAD4
P 2950 7350
F 0 "#PWR020" H 2950 7350 30  0001 C CNN
F 1 "GND" H 2950 7280 30  0001 C CNN
F 2 "" H 2950 7350 60  0000 C CNN
F 3 "" H 2950 7350 60  0000 C CNN
	1    2950 7350
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR021
U 1 1 53CABBFC
P 2950 6650
F 0 "#PWR021" H 2950 6610 30  0001 C CNN
F 1 "+3.3V" H 2950 6760 30  0000 C CNN
F 2 "" H 2950 6650 60  0000 C CNN
F 3 "" H 2950 6650 60  0000 C CNN
	1    2950 6650
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C1
U 1 1 53CAA38D
P 1850 7050
F 0 "C1" H 1850 7150 40  0000 L CNN
F 1 "10u" H 1856 6965 40  0000 L CNN
F 2 "" H 1888 6900 30  0000 C CNN
F 3 "" H 1850 7050 60  0000 C CNN
	1    1850 7050
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR022
U 1 1 53CAA437
P 1850 7350
F 0 "#PWR022" H 1850 7350 30  0001 C CNN
F 1 "GND" H 1850 7280 30  0001 C CNN
F 2 "" H 1850 7350 60  0000 C CNN
F 3 "" H 1850 7350 60  0000 C CNN
	1    1850 7350
	1    0    0    -1  
$EndComp
$Comp
L DIODE D1
U 1 1 53CAA4CA
P 1450 6750
F 0 "D1" H 1450 6850 40  0000 C CNN
F 1 "DIODE" H 1450 6650 40  0000 C CNN
F 2 "" H 1450 6750 60  0000 C CNN
F 3 "" H 1450 6750 60  0000 C CNN
	1    1450 6750
	1    0    0    -1  
$EndComp
$Comp
L CONN_2 P5
U 1 1 53CAA6B4
P 800 6850
F 0 "P5" V 750 6850 40  0000 C CNN
F 1 "5..12 V" V 850 6850 40  0000 C CNN
F 2 "" H 800 6850 60  0000 C CNN
F 3 "" H 800 6850 60  0000 C CNN
	1    800  6850
	-1   0    0    -1  
$EndComp
$Comp
L GND #PWR023
U 1 1 53CAA74E
P 1250 7150
F 0 "#PWR023" H 1250 7150 30  0001 C CNN
F 1 "GND" H 1250 7080 30  0001 C CNN
F 2 "" H 1250 7150 60  0000 C CNN
F 3 "" H 1250 7150 60  0000 C CNN
	1    1250 7150
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C2
U 1 1 53CAAC0C
P 3950 2250
F 0 "C2" H 3950 2350 40  0000 L CNN
F 1 "100n" H 3956 2165 40  0000 L CNN
F 2 "" H 3988 2100 30  0000 C CNN
F 3 "" H 3950 2250 60  0000 C CNN
	1    3950 2250
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C3
U 1 1 53CAAC17
P 4250 2250
F 0 "C3" H 4250 2350 40  0000 L CNN
F 1 "100n" H 4256 2165 40  0000 L CNN
F 2 "" H 4288 2100 30  0000 C CNN
F 3 "" H 4250 2250 60  0000 C CNN
	1    4250 2250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR024
U 1 1 53CAAE9F
P 4100 2650
F 0 "#PWR024" H 4100 2650 30  0001 C CNN
F 1 "GND" H 4100 2580 30  0001 C CNN
F 2 "" H 4100 2650 60  0000 C CNN
F 3 "" H 4100 2650 60  0000 C CNN
	1    4100 2650
	1    0    0    -1  
$EndComp
$Comp
L +3.3V-RESCUE-uMidi #PWR025
U 1 1 53CAAF3E
P 4100 1900
F 0 "#PWR025" H 4100 1860 30  0001 C CNN
F 1 "+3.3V" H 4100 2010 30  0000 C CNN
F 2 "" H 4100 1900 60  0000 C CNN
F 3 "" H 4100 1900 60  0000 C CNN
	1    4100 1900
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P7
U 1 1 53CAB25D
P 10850 2900
F 0 "P7" V 10800 2900 60  0000 C CNN
F 1 "GPIO1" V 10900 2900 60  0000 C CNN
F 2 "" H 10850 2900 60  0000 C CNN
F 3 "" H 10850 2900 60  0000 C CNN
	1    10850 2900
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR026
U 1 1 53CAB87E
P 10450 3400
F 0 "#PWR026" H 10450 3400 30  0001 C CNN
F 1 "GND" H 10450 3330 30  0001 C CNN
F 2 "" H 10450 3400 60  0000 C CNN
F 3 "" H 10450 3400 60  0000 C CNN
	1    10450 3400
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P8
U 1 1 53CAB9DB
P 10850 4100
F 0 "P8" V 10800 4100 60  0000 C CNN
F 1 "GPIO2" V 10900 4100 60  0000 C CNN
F 2 "" H 10850 4100 60  0000 C CNN
F 3 "" H 10850 4100 60  0000 C CNN
	1    10850 4100
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR027
U 1 1 53CAC1E9
P 10450 4600
F 0 "#PWR027" H 10450 4600 30  0001 C CNN
F 1 "GND" H 10450 4530 30  0001 C CNN
F 2 "" H 10450 4600 60  0000 C CNN
F 3 "" H 10450 4600 60  0000 C CNN
	1    10450 4600
	1    0    0    -1  
$EndComp
$Comp
L CONN_10 P9
U 1 1 53CAC3EE
P 10850 5300
F 0 "P9" V 10800 5300 60  0000 C CNN
F 1 "GPIO3" V 10900 5300 60  0000 C CNN
F 2 "" H 10850 5300 60  0000 C CNN
F 3 "" H 10850 5300 60  0000 C CNN
	1    10850 5300
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR028
U 1 1 53CAD159
P 10450 5800
F 0 "#PWR028" H 10450 5800 30  0001 C CNN
F 1 "GND" H 10450 5730 30  0001 C CNN
F 2 "" H 10450 5800 60  0000 C CNN
F 3 "" H 10450 5800 60  0000 C CNN
	1    10450 5800
	1    0    0    -1  
$EndComp
$Comp
L TVS D4
U 1 1 53CD4F63
P 6400 5400
F 0 "D4" H 6400 5550 60  0000 C CNN
F 1 "TVS" H 6400 5250 60  0000 C CNN
F 2 "" H 6400 5400 60  0000 C CNN
F 3 "" H 6400 5400 60  0000 C CNN
	1    6400 5400
	0    -1   -1   0   
$EndComp
$Comp
L TVS D5
U 1 1 53CD4F93
P 6650 5150
F 0 "D5" H 6650 5300 60  0000 C CNN
F 1 "TVS" H 6650 5000 60  0000 C CNN
F 2 "" H 6650 5150 60  0000 C CNN
F 3 "" H 6650 5150 60  0000 C CNN
	1    6650 5150
	0    -1   -1   0   
$EndComp
$Comp
L TVS D6
U 1 1 53CD4F9E
P 6900 4900
F 0 "D6" H 6900 5050 60  0000 C CNN
F 1 "TVS" H 6900 4750 60  0000 C CNN
F 2 "" H 6900 4900 60  0000 C CNN
F 3 "" H 6900 4900 60  0000 C CNN
	1    6900 4900
	0    -1   -1   0   
$EndComp
$Comp
L GND #PWR029
U 1 1 53CD5276
P 6400 5750
F 0 "#PWR029" H 6400 5750 30  0001 C CNN
F 1 "GND" H 6400 5680 30  0001 C CNN
F 2 "" H 6400 5750 60  0000 C CNN
F 3 "" H 6400 5750 60  0000 C CNN
	1    6400 5750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR030
U 1 1 53CD5281
P 6650 5500
F 0 "#PWR030" H 6650 5500 30  0001 C CNN
F 1 "GND" H 6650 5430 30  0001 C CNN
F 2 "" H 6650 5500 60  0000 C CNN
F 3 "" H 6650 5500 60  0000 C CNN
	1    6650 5500
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR031
U 1 1 53CD528C
P 6900 5250
F 0 "#PWR031" H 6900 5250 30  0001 C CNN
F 1 "GND" H 6900 5180 30  0001 C CNN
F 2 "" H 6900 5250 60  0000 C CNN
F 3 "" H 6900 5250 60  0000 C CNN
	1    6900 5250
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C5
U 1 1 53CD5DBF
P 5750 4600
F 0 "C5" H 5750 4700 40  0000 L CNN
F 1 "100n" H 5756 4515 40  0000 L CNN
F 2 "" H 5788 4450 30  0000 C CNN
F 3 "" H 5750 4600 60  0000 C CNN
	1    5750 4600
	1    0    0    -1  
$EndComp
NoConn ~ 5450 4700
$Comp
L PWR_FLAG #FLG032
U 1 1 53CD6B1F
P 1200 6650
F 0 "#FLG032" H 1200 6745 30  0001 C CNN
F 1 "PWR_FLAG" H 1200 6830 30  0000 C CNN
F 2 "" H 1200 6650 60  0000 C CNN
F 3 "" H 1200 6650 60  0000 C CNN
	1    1200 6650
	1    0    0    -1  
$EndComp
$Comp
L PWR_FLAG #FLG033
U 1 1 53CD6B31
P 1350 7000
F 0 "#FLG033" H 1350 7095 30  0001 C CNN
F 1 "PWR_FLAG" H 1350 7180 30  0000 C CNN
F 2 "" H 1350 7000 60  0000 C CNN
F 3 "" H 1350 7000 60  0000 C CNN
	1    1350 7000
	0    1    1    0   
$EndComp
NoConn ~ 7000 2350
NoConn ~ 7000 2450
$Comp
L PWR_FLAG #FLG034
U 1 1 53CD7FB0
P 1700 6800
F 0 "#FLG034" H 1700 6895 30  0001 C CNN
F 1 "PWR_FLAG" H 1700 6980 30  0000 C CNN
F 2 "" H 1700 6800 60  0000 C CNN
F 3 "" H 1700 6800 60  0000 C CNN
	1    1700 6800
	-1   0    0    1   
$EndComp
$Comp
L PWR_FLAG #FLG035
U 1 1 53CD8684
P 1650 6100
F 0 "#FLG035" H 1650 6195 30  0001 C CNN
F 1 "PWR_FLAG" H 1650 6280 30  0000 C CNN
F 2 "" H 1650 6100 60  0000 C CNN
F 3 "" H 1650 6100 60  0000 C CNN
	1    1650 6100
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG036
U 1 1 54699643
P 7000 1450
F 0 "#FLG036" H 7000 1545 30  0001 C CNN
F 1 "PWR_FLAG" H 7000 1630 30  0000 C CNN
F 2 "" H 7000 1450 60  0000 C CNN
F 3 "" H 7000 1450 60  0000 C CNN
	1    7000 1450
	0    -1   -1   0   
$EndComp
$Comp
L PWR_FLAG #FLG037
U 1 1 54699C27
P 8800 1400
F 0 "#FLG037" H 8800 1495 30  0001 C CNN
F 1 "PWR_FLAG" H 8800 1580 30  0000 C CNN
F 2 "" H 8800 1400 60  0000 C CNN
F 3 "" H 8800 1400 60  0000 C CNN
	1    8800 1400
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C6
U 1 1 546CD5B3
P 5000 5350
F 0 "C6" V 4850 5300 40  0000 L CNN
F 1 "4.7n" V 5150 5300 40  0000 L CNN
F 2 "" H 5038 5200 30  0000 C CNN
F 3 "" H 5000 5350 60  0000 C CNN
	1    5000 5350
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R10
U 1 1 546CD6F8
P 5000 5650
F 0 "R10" V 5080 5650 40  0000 C CNN
F 1 "1M" V 4900 5650 40  0000 C CNN
F 2 "" V 4930 5650 30  0000 C CNN
F 3 "" H 5000 5650 30  0000 C CNN
	1    5000 5650
	0    -1   -1   0   
$EndComp
$Comp
L INDUCTOR L3
U 1 1 546E3812
P 3350 6750
F 0 "L3" V 3300 6750 40  0000 C CNN
F 1 "10u" V 3450 6750 40  0000 C CNN
F 2 "" H 3350 6750 60  0000 C CNN
F 3 "" H 3350 6750 60  0000 C CNN
	1    3350 6750
	0    -1   -1   0   
$EndComp
$Comp
L C-RESCUE-uMidi C13
U 1 1 546E3C38
P 3700 7000
F 0 "C13" H 3700 7100 40  0000 L CNN
F 1 "10u" H 3706 6915 40  0000 L CNN
F 2 "" H 3738 6850 30  0000 C CNN
F 3 "" H 3700 7000 60  0000 C CNN
	1    3700 7000
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C16
U 1 1 546E3CB6
P 4400 7000
F 0 "C16" H 4400 7100 40  0000 L CNN
F 1 "100n" H 4406 6915 40  0000 L CNN
F 2 "" H 4438 6850 30  0000 C CNN
F 3 "" H 4400 7000 60  0000 C CNN
	1    4400 7000
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C15
U 1 1 546E3D1F
P 4200 7000
F 0 "C15" H 4200 7100 40  0000 L CNN
F 1 "100n" H 4206 6915 40  0000 L CNN
F 2 "" H 4238 6850 30  0000 C CNN
F 3 "" H 4200 7000 60  0000 C CNN
	1    4200 7000
	1    0    0    -1  
$EndComp
$Comp
L C-RESCUE-uMidi C14
U 1 1 546E3D7F
P 4000 7000
F 0 "C14" H 4000 7100 40  0000 L CNN
F 1 "100n" H 4006 6915 40  0000 L CNN
F 2 "" H 4038 6850 30  0000 C CNN
F 3 "" H 4000 7000 60  0000 C CNN
	1    4000 7000
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR038
U 1 1 546E41EF
P 3700 7250
F 0 "#PWR038" H 3700 7250 30  0001 C CNN
F 1 "GND" H 3700 7180 30  0001 C CNN
F 2 "" H 3700 7250 60  0000 C CNN
F 3 "" H 3700 7250 60  0000 C CNN
	1    3700 7250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR039
U 1 1 546E4A76
P 4000 7250
F 0 "#PWR039" H 4000 7250 30  0001 C CNN
F 1 "GND" H 4000 7180 30  0001 C CNN
F 2 "" H 4000 7250 60  0000 C CNN
F 3 "" H 4000 7250 60  0000 C CNN
	1    4000 7250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR040
U 1 1 546E4AB8
P 4200 7250
F 0 "#PWR040" H 4200 7250 30  0001 C CNN
F 1 "GND" H 4200 7180 30  0001 C CNN
F 2 "" H 4200 7250 60  0000 C CNN
F 3 "" H 4200 7250 60  0000 C CNN
	1    4200 7250
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR041
U 1 1 546E4AFA
P 4400 7250
F 0 "#PWR041" H 4400 7250 30  0001 C CNN
F 1 "GND" H 4400 7180 30  0001 C CNN
F 2 "" H 4400 7250 60  0000 C CNN
F 3 "" H 4400 7250 60  0000 C CNN
	1    4400 7250
	1    0    0    -1  
$EndComp
Text GLabel 4500 6750 2    60   Output ~ 0
VGPIO
Text GLabel 10400 3650 0    60   Input ~ 0
VGPIO
Text GLabel 10400 2450 0    60   Input ~ 0
VGPIO
Text GLabel 10400 4850 0    60   Input ~ 0
VGPIO
$Comp
L R-RESCUE-uMidi R11
U 1 1 546F0CF2
P 6850 3300
F 0 "R11" V 6930 3300 40  0000 C CNN
F 1 "120" V 6857 3301 40  0000 C CNN
F 2 "" V 6780 3300 30  0000 C CNN
F 3 "" H 6850 3300 30  0000 C CNN
	1    6850 3300
	0    1    1    0   
$EndComp
$Comp
L R-RESCUE-uMidi R12
U 1 1 546F0E74
P 6850 3600
F 0 "R12" V 6930 3600 40  0000 C CNN
F 1 "120" V 6857 3601 40  0000 C CNN
F 2 "" V 6780 3600 30  0000 C CNN
F 3 "" H 6850 3600 30  0000 C CNN
	1    6850 3600
	0    1    1    0   
$EndComp
$Comp
L LED-RESCUE-uMidi D7
U 1 1 546F2E09
P 6350 3300
F 0 "D7" H 6350 3400 50  0000 C CNN
F 1 "LED" H 6350 3200 50  0000 C CNN
F 2 "" H 6350 3300 60  0000 C CNN
F 3 "" H 6350 3300 60  0000 C CNN
	1    6350 3300
	-1   0    0    1   
$EndComp
$Comp
L LED-RESCUE-uMidi D8
U 1 1 546F2EDE
P 6350 3600
F 0 "D8" H 6350 3700 50  0000 C CNN
F 1 "LED" H 6350 3500 50  0000 C CNN
F 2 "" H 6350 3600 60  0000 C CNN
F 3 "" H 6350 3600 60  0000 C CNN
	1    6350 3600
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR042
U 1 1 546F3606
P 6100 3650
F 0 "#PWR042" H 6100 3650 30  0001 C CNN
F 1 "GND" H 6100 3580 30  0001 C CNN
F 2 "" H 6100 3650 60  0000 C CNN
F 3 "" H 6100 3650 60  0000 C CNN
	1    6100 3650
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R13
U 1 1 54705F59
P 2950 1550
F 0 "R13" V 3030 1550 40  0000 C CNN
F 1 "4.7k" V 2957 1551 40  0000 C CNN
F 2 "" V 2880 1550 30  0000 C CNN
F 3 "" H 2950 1550 30  0000 C CNN
	1    2950 1550
	-1   0    0    1   
$EndComp
$Comp
L GND #PWR043
U 1 1 5470748F
P 2950 1850
F 0 "#PWR043" H 2950 1850 30  0001 C CNN
F 1 "GND" H 2950 1780 30  0001 C CNN
F 2 "" H 2950 1850 60  0000 C CNN
F 3 "" H 2950 1850 60  0000 C CNN
	1    2950 1850
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R14
U 1 1 5484813F
P 9650 1900
F 0 "R14" V 9730 1900 40  0000 C CNN
F 1 "10k" V 9657 1901 40  0000 C CNN
F 2 "" V 9580 1900 30  0000 C CNN
F 3 "" H 9650 1900 30  0000 C CNN
	1    9650 1900
	1    0    0    -1  
$EndComp
$Comp
L R-RESCUE-uMidi R15
U 1 1 548484DC
P 9850 1900
F 0 "R15" V 9930 1900 40  0000 C CNN
F 1 "10k" V 9857 1901 40  0000 C CNN
F 2 "" V 9780 1900 30  0000 C CNN
F 3 "" H 9850 1900 30  0000 C CNN
	1    9850 1900
	1    0    0    -1  
$EndComp
Text GLabel 9650 2200 3    60   Input ~ 0
SCL
Text GLabel 10150 4100 1    60   Input ~ 0
SDA
Text GLabel 10000 4200 1    60   Input ~ 0
SCL
Text GLabel 9850 2200 3    60   Input ~ 0
SDA
Text GLabel 6950 1750 0    60   Input ~ 0
PWR_MCU
Text GLabel 9750 1450 1    60   Input ~ 0
PWR_MCU
Wire Wire Line
	8500 4950 8500 5050
Wire Wire Line
	8250 4950 8650 4950
Wire Wire Line
	8650 4950 8650 4850
Wire Wire Line
	8350 4950 8350 4850
Connection ~ 8500 4950
Wire Wire Line
	8450 4850 8450 4950
Connection ~ 8450 4950
Wire Wire Line
	8550 4850 8550 4950
Connection ~ 8550 4950
Wire Wire Line
	8500 2150 8500 2250
Wire Wire Line
	8400 2150 8600 2150
Connection ~ 8500 2150
Wire Wire Line
	8600 2150 8600 2250
Wire Wire Line
	7000 2150 7300 2150
Wire Wire Line
	7300 2150 7300 2650
Wire Wire Line
	7300 2650 7600 2650
Wire Wire Line
	7100 2250 7000 2250
Wire Wire Line
	7000 2550 7600 2550
Wire Wire Line
	2650 2300 2700 2300
Wire Wire Line
	2700 2700 2650 2700
Wire Wire Line
	2650 3100 2700 3100
Wire Wire Line
	6200 4550 7600 4550
Wire Wire Line
	5600 4450 7600 4450
Wire Wire Line
	3750 2950 3750 3100
Connection ~ 3750 3100
Wire Wire Line
	3750 1350 3750 2450
Wire Wire Line
	1700 2300 1750 2300
Wire Wire Line
	1700 2700 1750 2700
Wire Wire Line
	1700 3100 1750 3100
Wire Wire Line
	1150 2100 1200 2100
Wire Wire Line
	1150 2300 1200 2300
Wire Wire Line
	1150 3100 1200 3100
Wire Wire Line
	1150 2900 1200 2900
Wire Wire Line
	1150 2700 1200 2700
Wire Wire Line
	1150 2500 1200 2500
Wire Wire Line
	1800 2900 1700 2900
Wire Wire Line
	1800 2500 1700 2500
Wire Wire Line
	1800 2100 1700 2100
Wire Wire Line
	1200 1550 1950 1550
Wire Wire Line
	1950 1550 1950 1400
Wire Wire Line
	1950 1400 2000 1400
Wire Wire Line
	1800 1550 1800 1500
Wire Wire Line
	1200 1550 1200 1400
Wire Wire Line
	1200 1400 1150 1400
Connection ~ 1800 1550
Wire Wire Line
	1750 1050 1950 1050
Wire Wire Line
	1950 1050 1950 1200
Wire Wire Line
	1950 1200 2000 1200
Wire Wire Line
	1800 1050 1800 1100
Connection ~ 1800 1050
Wire Wire Line
	1150 1200 1200 1200
Wire Wire Line
	1200 1200 1200 1050
Wire Wire Line
	1200 1050 1250 1050
Wire Wire Line
	2700 1150 2750 1150
Wire Wire Line
	2750 1150 2750 1100
Wire Wire Line
	3750 1600 3850 1600
Wire Wire Line
	3250 1350 3250 1300
Wire Wire Line
	3250 800  3250 750 
Wire Wire Line
	2750 1850 2750 1450
Wire Wire Line
	2750 1450 2700 1450
Connection ~ 3250 1350
Connection ~ 3750 1600
Connection ~ 3750 2300
Wire Wire Line
	8250 1650 8250 1700
Connection ~ 8250 1650
Wire Wire Line
	8050 1650 8050 1700
Connection ~ 8050 1650
Wire Wire Line
	7850 1700 7850 1650
Wire Wire Line
	7650 2150 7650 2100
Wire Wire Line
	8800 1400 8800 2250
Wire Wire Line
	9150 1650 9150 1700
Wire Wire Line
	8950 1650 8950 1700
Connection ~ 8950 1650
Wire Wire Line
	8250 2150 8250 2100
Wire Wire Line
	8050 2150 8050 2100
Wire Wire Line
	7850 2150 7850 2100
Wire Wire Line
	8950 2150 8950 2100
Wire Wire Line
	9150 2150 9150 2100
Wire Wire Line
	1750 5950 1750 6250
Wire Wire Line
	1750 6750 1750 6650
Wire Wire Line
	1650 6750 1950 6750
Wire Wire Line
	2950 7350 2950 7250
Wire Wire Line
	2950 6650 2950 6850
Connection ~ 2950 6750
Wire Wire Line
	1850 6750 1850 6850
Connection ~ 1850 6750
Wire Wire Line
	1850 7350 1850 7250
Connection ~ 1750 6750
Wire Wire Line
	1150 6750 1250 6750
Wire Wire Line
	1250 6950 1250 7150
Wire Wire Line
	1250 6950 1150 6950
Wire Wire Line
	3950 2450 3950 2550
Wire Wire Line
	3950 2050 3950 1950
Wire Wire Line
	3950 1950 4250 1950
Wire Wire Line
	4250 1950 4250 2050
Wire Wire Line
	3950 2550 4250 2550
Wire Wire Line
	4250 2550 4250 2450
Wire Wire Line
	4100 2650 4100 2550
Connection ~ 4100 2550
Wire Wire Line
	4100 1900 4100 1950
Connection ~ 4100 1950
Wire Wire Line
	9400 2550 10500 2550
Wire Wire Line
	10500 2650 9400 2650
Wire Wire Line
	9400 2750 10500 2750
Wire Wire Line
	10500 2850 9400 2850
Wire Wire Line
	9400 2950 10500 2950
Wire Wire Line
	10500 3050 9400 3050
Wire Wire Line
	9400 3150 10500 3150
Wire Wire Line
	10500 3250 9400 3250
Wire Wire Line
	10500 3750 9950 3750
Wire Wire Line
	9950 3750 9950 3400
Wire Wire Line
	9950 3400 9400 3400
Wire Wire Line
	9400 3500 9900 3500
Wire Wire Line
	9900 3500 9900 3850
Wire Wire Line
	9900 3850 10500 3850
Wire Wire Line
	9400 3600 9850 3600
Wire Wire Line
	9850 3600 9850 3950
Wire Wire Line
	9850 3950 10500 3950
Wire Wire Line
	9400 3700 9800 3700
Wire Wire Line
	9800 3700 9800 4050
Wire Wire Line
	9800 4050 10500 4050
Wire Wire Line
	9400 3850 9750 3850
Wire Wire Line
	9750 3850 9750 4150
Wire Wire Line
	9750 4150 10500 4150
Wire Wire Line
	9400 3950 9700 3950
Wire Wire Line
	9700 3950 9700 4250
Wire Wire Line
	9700 4250 10500 4250
Wire Wire Line
	9400 4050 9650 4050
Wire Wire Line
	9650 4050 9650 4350
Wire Wire Line
	9650 4350 10500 4350
Wire Wire Line
	9400 4150 9600 4150
Wire Wire Line
	9600 4150 9600 4450
Wire Wire Line
	9600 4450 10500 4450
Wire Wire Line
	9400 4250 9550 4250
Wire Wire Line
	9550 4250 9550 4950
Wire Wire Line
	9550 4950 10500 4950
Wire Wire Line
	9400 4350 9500 4350
Wire Wire Line
	9500 4350 9500 5050
Wire Wire Line
	9500 5050 10500 5050
Wire Wire Line
	9400 4450 9450 4450
Wire Wire Line
	9450 4450 9450 5150
Wire Wire Line
	9450 5150 10500 5150
Wire Wire Line
	9400 4550 9400 5250
Wire Wire Line
	9400 5250 10500 5250
Wire Wire Line
	6200 4600 6200 4550
Wire Wire Line
	5450 4600 6200 4600
Wire Wire Line
	5450 4800 5750 4800
Wire Wire Line
	6450 4400 6450 4350
Wire Wire Line
	7600 4150 7500 4150
Wire Wire Line
	7500 4150 7500 5650
Wire Wire Line
	7500 5650 10500 5650
Wire Wire Line
	7600 4050 7400 4050
Wire Wire Line
	7400 4050 7400 5550
Wire Wire Line
	7400 5550 10500 5550
Wire Wire Line
	7600 3950 7300 3950
Wire Wire Line
	7300 3950 7300 5450
Wire Wire Line
	7300 5450 10500 5450
Wire Wire Line
	10500 5350 7200 5350
Wire Wire Line
	7200 5350 7200 3850
Wire Wire Line
	7200 3850 7600 3850
Wire Wire Line
	7500 3600 7600 3600
Wire Wire Line
	7500 3750 7550 3750
Wire Wire Line
	7550 3750 7550 3700
Wire Wire Line
	7550 3700 7600 3700
Wire Wire Line
	7100 1400 7100 2250
Wire Wire Line
	7000 2650 7050 2650
Wire Wire Line
	7050 2650 7050 2700
Connection ~ 6450 4400
Connection ~ 6650 4450
Connection ~ 6400 4550
Wire Wire Line
	6900 5250 6900 5200
Wire Wire Line
	6650 5500 6650 5450
Wire Wire Line
	6400 5750 6400 5700
Wire Wire Line
	6400 5100 6400 4550
Wire Wire Line
	6650 4450 6650 4850
Wire Wire Line
	6900 4400 6900 4600
Wire Wire Line
	5750 4800 5750 5800
Connection ~ 5750 4800
Wire Wire Line
	5450 4400 6900 4400
Connection ~ 5750 4400
Wire Wire Line
	1250 7000 1350 7000
Connection ~ 1250 7000
Wire Wire Line
	1200 6750 1200 6650
Connection ~ 1200 6750
Wire Wire Line
	2850 6750 3050 6750
Connection ~ 1700 6750
Wire Wire Line
	1700 6800 1700 6750
Wire Wire Line
	1650 6100 1750 6100
Connection ~ 1750 6100
Wire Wire Line
	2400 7100 2400 7350
Wire Wire Line
	8250 4950 8250 4850
Connection ~ 8350 4950
Wire Wire Line
	8800 1650 9150 1650
Connection ~ 8800 1650
Wire Wire Line
	8400 1650 8400 2250
Connection ~ 8400 2150
Wire Wire Line
	8700 1450 8800 1450
Wire Wire Line
	7650 1650 8400 1650
Wire Wire Line
	7400 2150 7400 2550
Connection ~ 7400 2550
Wire Wire Line
	7400 1450 7400 1650
Wire Wire Line
	7650 1450 7650 1700
Wire Wire Line
	7000 1450 8100 1450
Connection ~ 7650 1650
Connection ~ 7850 1650
Connection ~ 7650 1450
Connection ~ 7400 1450
Connection ~ 7100 1450
Wire Wire Line
	7100 800  7100 750 
Connection ~ 8800 1450
Wire Wire Line
	3200 3350 3200 3200
Wire Wire Line
	3200 2100 3200 2200
Wire Wire Line
	4250 4700 4250 5650
Wire Wire Line
	4250 5350 4800 5350
Wire Wire Line
	5200 5350 5750 5350
Connection ~ 5750 5350
Wire Wire Line
	5250 5650 5750 5650
Connection ~ 5750 5650
Wire Wire Line
	4250 5650 4750 5650
Connection ~ 4250 5350
Wire Wire Line
	3700 7250 3700 7200
Wire Wire Line
	3700 6800 3700 6750
Wire Wire Line
	3650 6750 4500 6750
Wire Wire Line
	4400 6750 4400 6800
Connection ~ 3700 6750
Wire Wire Line
	4200 6800 4200 6750
Connection ~ 4200 6750
Wire Wire Line
	4000 6800 4000 6750
Connection ~ 4000 6750
Wire Wire Line
	4400 7250 4400 7200
Wire Wire Line
	4200 7200 4200 7250
Wire Wire Line
	4000 7250 4000 7200
Connection ~ 4400 6750
Wire Wire Line
	10450 4600 10450 4550
Wire Wire Line
	10450 4550 10500 4550
Wire Wire Line
	10450 3400 10450 3350
Wire Wire Line
	10450 3350 10500 3350
Wire Wire Line
	10450 5800 10450 5750
Wire Wire Line
	10450 5750 10500 5750
Wire Wire Line
	10400 4850 10500 4850
Wire Wire Line
	10400 3650 10500 3650
Wire Wire Line
	10400 2450 10500 2450
Wire Wire Line
	7150 3400 7600 3400
Wire Wire Line
	7600 3500 7150 3500
Wire Wire Line
	6600 3300 6550 3300
Wire Wire Line
	6550 3600 6600 3600
Wire Wire Line
	6100 3300 6100 3650
Wire Wire Line
	7150 3400 7150 3300
Wire Wire Line
	7150 3300 7100 3300
Wire Wire Line
	7100 3600 7150 3600
Wire Wire Line
	7150 3600 7150 3500
Wire Wire Line
	6150 3600 6100 3600
Connection ~ 6100 3600
Wire Wire Line
	6100 3300 6150 3300
Wire Wire Line
	2950 1850 2950 1800
Wire Wire Line
	2700 1250 2950 1250
Wire Wire Line
	2950 1250 2950 1300
Wire Wire Line
	9650 2200 9650 2150
Wire Wire Line
	9850 2200 9850 2150
Wire Wire Line
	9650 1600 9850 1600
Wire Wire Line
	9650 1600 9650 1650
Wire Wire Line
	9850 1600 9850 1650
Connection ~ 9750 1600
Wire Wire Line
	10150 4100 10150 4150
Connection ~ 10150 4150
Wire Wire Line
	10000 4200 10000 4250
Connection ~ 10000 4250
Wire Wire Line
	6950 1750 7100 1750
Connection ~ 7100 1750
Wire Wire Line
	9750 1600 9750 1450
NoConn ~ 4450 4400
NoConn ~ 4450 4500
NoConn ~ 4450 4800
Wire Wire Line
	3600 3100 3850 3100
Wire Wire Line
	3600 2700 3650 2700
Wire Wire Line
	3600 2300 3750 2300
Wire Wire Line
	2700 1350 3750 1350
$Comp
L JUMPER JP2
U 1 1 548CD67D
P 5300 2850
F 0 "JP2" H 5300 3000 60  0000 C CNN
F 1 "JUMPER" H 5300 2770 40  0000 C CNN
F 2 "" H 5300 2850 60  0000 C CNN
F 3 "" H 5300 2850 60  0000 C CNN
	1    5300 2850
	1    0    0    -1  
$EndComp
$Comp
L JUMPER JP3
U 1 1 548CD801
P 5300 3150
F 0 "JP3" H 5300 3300 60  0000 C CNN
F 1 "JUMPER" H 5300 3070 40  0000 C CNN
F 2 "" H 5300 3150 60  0000 C CNN
F 3 "" H 5300 3150 60  0000 C CNN
	1    5300 3150
	1    0    0    -1  
$EndComp
$Comp
L JUMPER JP4
U 1 1 548CD86D
P 5300 3450
F 0 "JP4" H 5300 3600 60  0000 C CNN
F 1 "JUMPER" H 5300 3370 40  0000 C CNN
F 2 "" H 5300 3450 60  0000 C CNN
F 3 "" H 5300 3450 60  0000 C CNN
	1    5300 3450
	1    0    0    -1  
$EndComp
$Comp
L JUMPER JP5
U 1 1 548CD8CA
P 5300 3750
F 0 "JP5" H 5300 3900 60  0000 C CNN
F 1 "JUMPER" H 5300 3670 40  0000 C CNN
F 2 "" H 5300 3750 60  0000 C CNN
F 3 "" H 5300 3750 60  0000 C CNN
	1    5300 3750
	1    0    0    -1  
$EndComp
$Comp
L GND #PWR044
U 1 1 548CE1BB
P 4950 3850
F 0 "#PWR044" H 4950 3850 30  0001 C CNN
F 1 "GND" H 4950 3780 30  0001 C CNN
F 2 "" H 4950 3850 60  0000 C CNN
F 3 "" H 4950 3850 60  0000 C CNN
	1    4950 3850
	1    0    0    -1  
$EndComp
Wire Wire Line
	4950 2850 4950 3850
Wire Wire Line
	4950 2850 5000 2850
Wire Wire Line
	5000 3150 4950 3150
Connection ~ 4950 3150
Wire Wire Line
	5000 3450 4950 3450
Connection ~ 4950 3450
Wire Wire Line
	5000 3750 4950 3750
Connection ~ 4950 3750
Wire Wire Line
	7600 3150 7400 3150
Wire Wire Line
	7400 3150 7400 2850
Wire Wire Line
	7400 2850 5600 2850
Wire Wire Line
	7600 3250 7300 3250
Wire Wire Line
	7300 3250 7300 2950
Wire Wire Line
	7300 2950 5700 2950
Wire Wire Line
	5700 2950 5700 3150
Wire Wire Line
	5700 3150 5600 3150
Wire Wire Line
	7600 4250 7100 4250
Wire Wire Line
	7100 4250 7100 3800
Wire Wire Line
	5600 3750 5700 3750
Wire Wire Line
	5700 3750 5700 3900
Wire Wire Line
	5700 3900 7000 3900
Wire Wire Line
	7000 3900 7000 4350
Wire Wire Line
	7000 4350 7600 4350
Wire Wire Line
	7100 3800 5800 3800
Wire Wire Line
	5800 3800 5800 3450
Wire Wire Line
	5800 3450 5600 3450
Wire Wire Line
	4250 4700 4450 4700
Wire Wire Line
	5600 4450 5600 4500
Wire Wire Line
	5600 4500 5450 4500
$EndSCHEMATC
