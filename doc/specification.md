# Summary
+ Power supply
  + Input voltage: 6 .. 10 V
  + Battery supply possible

+ PCB
  + Single sided
  + SMD preferred, avoid drill holes
  + 0.3 mm copper halfpitch
  + DIY manufacturable

+ MIDI
  + Input connected to MCU
  + Output connected to MCU
  + Through connected to input

+ MCU
  + Atmel atMEGA88
  + External crystal option
  + 6-pin header for SPI in system programming and peripheral communication

+ Connectivity
  + GPIO Pin headers for all unused MCU ports
  + SPI
  + I2C
  + ADC inputs

+ On-Board peripherals
  + 1 Toggle switch
  + 2 LEDs (MIDI in and MIDI out)

# In System Programming
+ 2x3 pin pin header, 2.54mm pitch
+ Pinout AVRISP mkII compatible:

| Pin   | Signal    |
|:-----:|:---------:|
| 1     | MISO      |
| 2     | VCC       |
| 3     | SCK       |
| 4     | MOSI      |
| 5     | RESET     |
| 6     | GND       |
