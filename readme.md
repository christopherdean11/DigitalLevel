# Digital LED Level
Custom PCB for using an accelerometer as a level with LED indicators. Scope is small to encourage getting to the finish line but has enough hardware features to come up with createive software ideas. 

## Goals
Overall: Create a PCB from scratch that can display multiple LED patterns  
- [x] Select a MCU - STM32F030F4
- [x] Design a PCB
- [x] Fab the PCB
- [x] Populate the PCB
- [x] Code and test the PCB

## Vender Documentation
### Datasheets:
* MCU - [STM32F030F4](https://www.st.com/resource/en/datasheet/stm32f030f4.pdf), [Reference Manual](https://www.st.com/resource/en/reference_manual/dm00091010-stm32f030x4x6x8xc-and-stm32f070x6xb-advanced-armbased-32bit-mcus-stmicroelectronics.pdf)
* 3.3V LDO - [MIC5504-3.3](https://www.microchip.com/wwwproducts/en/MIC5504)
* Accelerometer - [ADXL343](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL343.pdf)
* Acceleromter Breakout [Adafruit ADXL343 Breakout](https://learn.adafruit.com/adxl343-breakout-learning-guide/downloads)

### Guides / App Notes / Manuals:
* [STM32 Boot Options](https://www.st.com/content/ccc/resource/technical/document/application_note/b9/9b/16/3a/12/1e/40/0c/CD00167594.pdf/files/CD00167594.pdf/jcr:content/translations/en.CD00167594.pdf)
* [Getting Started with STM32F030xx Hardware Development](https://www.st.com/content/ccc/resource/technical/document/application_note/91/66/2d/8c/f9/b5/47/55/DM00089834.pdf/files/DM00089834.pdf/jcr:content/translations/en.DM00089834.pdf)
* [STM32F3 Discovery User Manual](https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf) (Initially used as a programmer, later replaced with standalone ST-LINK V2 USB dongle)

## Design Process
See [Hardware Design](pcb/readme.md) for hardware design process.  
Software Design readme coming soon.


## Features
#### Hardware
* 9 LEDs
* STM32F0
* Acceleromter
* 2 push-buttons
  - 1 hardwired to GPIO
  - 1 wired through a jumper to a GPIO (shared with INT1 on accelerometer)

#### Software
* LED cycle patterns with speed and direction adjustable with push-buttons




---
## Ideas for rev1
* Add a switch on the vcc input 
* Add smt LED on 3V3
* Just get the thing professionally assembled
* Would be nice to power from batteries, perhaps need a nice connector for that
* micro usb for power in connector?
* input voltage protection
* want to be able to control LED brightness
  - could try to find a suitable LED driver
  - could just add a PMOS inline with Vled for pwm-ing
  - could pwm the GPIOs but I don't think that would wok because PWM-ing the GPIO would block execution unless it was hardware timed somehow

