# Hardware Design Process
## MCU Requirements
### Select an MCU
Initially thought about picking an STM32G0 series for low-cost, but after looking into it more, the STM32F0 series is not much more when buying low quantity, seems to be more popular, has extensive documentation, and the STM32F and STM32L series parts are already library components in Kicad.  
Another considerations was a package that lends itself to hand soldering.  With all of these in mind, I selected the [STM32F030F4](https://www.st.com/en/microcontrollers-microprocessors/stm32f030f4.html) in 20-pin TSSOP at approx. $1.23/unit.



###  I/O
This requires some time in STM Cube IDE to verify pinout and pin options to get a feel for what the board can look like.  We can easily get 8 GPIO outputs for LEDs with this MCU.  The [STM32F030F4](https://www.st.com/en/microcontrollers-microprocessors/stm32f030f4.html) comes in a 20pin TSSOP (this project will be hand soldered), with 12 GPIO available on PORTA + PB1 for 13 GPIO pins.
###  Programming
Another big item is how to program the MCU.  It looks like the STM32F030F4 has SWD (I beleive all ARM MCUs have this) support, so I should be able to connect with ST-LINK using SWD pins.  The F3 discovery board has an option to disconnect the actual dev board section from the ST-LINK section and use ST-LINK to program another board.  This will be a good starting point for programming. So the board will need to have the SWD pins broken out to headers.  

Tie BOOT0 to GND for normal operation and programming with SWD. See [this forum page](https://community.st.com/s/question/0D50X00009XkYDuSAN/hi-what-is-boot0-in-stm32).  Could tie it to a resitor to GND to leave door open to overdrive it if needed for some reason.  

The pinout for the SWD connector on the F3 Discovery board is available in [this document](https://www.st.com/content/ccc/resource/technical/document/user_manual/8a/56/97/63/8d/56/41/73/DM00063382.pdf/files/DM00063382.pdf/jcr:content/translations/en.DM00063382.pdf), page 16
Pins required: 
* VDD
* SWCLK
* GND
* SWDIO
* NRST - **NOTE:Solder bridge SB7 must be open to use NRST pin to control another device**

*Note: Eventually gave up with the F3 Discovery and orered a standalone ST-LINK V2 dongle, seems to just be easier*

## PCB Requirements:
- As small as reasonably possible to save cost
- series of LEDs in a row at one end
- SWD programming/debugging pins broken out
- At least 1 push button to cycle through LED options
- Acceleromter to do I2C comms and for LED patterns  

##  PCB Design Process:
* [x] Open CubeIDE to start a project with the STM32F030F4 and set up the pin configurations.
* [x] Add the MCU to a new design in Kicad.
* [x] Refer to the CubeIDE for the desired pin configuration.  Break out the SWD pins to headers and strap the BOOT pin.
* [x] Add some LEDs and a push button and wire it up.
* [x] Research and find an acceptable accelerometer.  Low cost I2C capable and hand-solerable.
* [x] Research and select a 3.3V regulator to power from 5V.

### Power
Plan is to power the board with 5V DC from a wall wart and regulate it to 3.3V.  Since this isn't powered by a battery, power efficiency isn't really a concern on this project (save that futuer versions), so an LDO will be fine, gets the job done and is dead simple to add to a PCB.  
#### Selecting 3.3V Regulator:
Just filter on digikey/mouser for 3.3V LDO. Found [MIC5504-3.3YM5](https://www.microchip.com/wwwproducts/en/MIC5504) for 3.3V at 300mA and 2% initial accuracy. VIN up to 5.5V. 
#### LED Power Consumption
9 LEDs at 15mA = 135mA  
Total allowed GPIO sourcing current for STM32F030F4 is 80mA (see page 44 in datasheet)  
*Redesigned board so LEDs are powered by the VDD rail and turned on off by GPIOs with an NMOS.*

### Acceleromoter
Needed a breakout board because all acceleromters have packages that do not lend themselves to hand-soldering.  The ADXL343 breakout from Adafruit is reasonbly priced and uses a quality acceleromter. 
Some notes: 
* Has a level shifter on board, may need to remove/jump over those but do not expect a problem
* See pg 16 of [ADXL3434 datasheet](https://www.analog.com/media/en/technical-documentation/data-sheets/ADXL343.pdf) for I2C addresses and address for Read/Write

| SDO/ALT_ADDRESS Pin | 7 bit address | Read Address | Write Address |  
| --- | --- | ---- | --- |  
| GND | 0x53| 0xA6 | 0xA7 |

I2C address are 7-bit, and the least-signficant bit (LSB) is used to determine if the transaction is a read or write.

## PCB Layout
Begin layout of the design and revise as needed.
### Layout Process
1. Get everything in roughly the desired position. See how it looks, revise layout and schematic as needed.
   - I want all the LEDs in a nice row on the edge, so I changed the schematic to put LED current limit resistors between LED and NMOS and moved the current sense solder bridges for LED1 and LED5 for the same reason.  
1. Move on to routing power and ground after initial revisions are complete and rough layout is acceptable.
   - 2 Layer board
   - Ground plane as bottom layer, only use for routing when necessary to jump under existing traces
   - Run power along the edge of the board for LEDs and SW1, SW2, and wrap around to power acceleromter
   - This is not a high-speed or high-precision board, routing will be as good as it can be with 2 layers
## PCB Bring-Up Plan
Populate minimally requried components to get a program onto the board and show a sign of life.  
* Header pins, U1, U2, Boot resistor, decoupling caps, and LED2
* Run a program to blink LED2 to prove communication

Once that is working populate remainder of board and run a progressive blink routine to validate each LED.

## Experience Notes
* Almost forgot to add decoupling caps, researched and added those to the design as per the "Getting Started with STM32F030xx Hardware Development" link above.  
* Just noticed there is a "hand-solder" optional footprint for SMT components in KiCAD, and since I plan to hand solder, I will have to change all the footprints to those or I will probably wish I did.
* Board ordered on Feb 28 via oshpark
* Parts ordered on Feb 29 via mouser  


---

## Board Bring-up

*Eventually resolved these issues below and can talk to the board. Still not clear what the fix truly was, but I bought a standalone ST-LINK V2 USB dongle, updated STM CubeIDE to the latest, and completely discarded the Cube code project and started a Cube project, and all seems to be well.  The program downloads to the micro and the debugger works. Original debugging notes kept here.*


#### Original Debug Notes
* Trying to program using F3Discovery board as an ST-LINK and getting 

> Error message from debugger back end:  
> Load failed  
> Failed to execute MI command:  
> load /Users/christopherdean/Dev/stm32/LED_Level/code/Debug/Digital_Level.elf   
>  
> Error message from debugger back end:  
> Load failed  
> Load failed  

* Checked current on 3V3 rail (not using 3.3V regulator) and see it at ~1.3mA for a second or two, and then jump to ~1.8mA or 2mA
* [This schematic](https://easyeda.com/r3bers/STM32F103C8T6-Test-Board) shows pullup/down resistors on SWCLKS/SWDIO 
  - other posts online say the uc should have internal pullup/downs that are sufficient..

* Command to run the debugger from command line:
navigate to  
```
/Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.stlink-gdb-server.macos64_1.3.0.202002181050/tools/bin
```
and run
```
./ST-LINK_gdbserver -d -v -cp /Applications/STM32CubeIDE.app/Contents/Eclipse/plugins/com.st.stm32cube.ide.mcu.externaltools.cubeprogrammer.macos64_1.3.0.202002181050/tools/bin
```

When I run it to program my board:
```
STMicroelectronics ST-LINK GDB server. Version 5.5.0
Copyright (c) 2019, STMicroelectronics. All rights reserved.

Starting server with the following options:
        Persistent Mode            : Disabled
        Logging Level              : 31
        Listen Port Number         : 61234
        Status Refresh Delay       : 15s
        Verbose Mode               : Enabled
        SWD Debug                  : Enabled

Target connection mode: Default
Target connection failed. Try connecting under reset
Target connection failed
Target unknown error 19

Error in initializing ST-LINK device.
Reason: Unknown. Please check power and cabling to target.
```

When I install the jumpers on the F3DISCOVERY board to use the ST-LINK to program the discovery board (and not just as an ST-LINK for external things), I see this:
```
STMicroelectronics ST-LINK GDB server. Version 5.5.0
Copyright (c) 2019, STMicroelectronics. All rights reserved.

Starting server with the following options:
        Persistent Mode            : Disabled
        Logging Level              : 31
        Listen Port Number         : 61234
        Status Refresh Delay       : 15s
        Verbose Mode               : Enabled
        SWD Debug                  : Enabled

Target connection mode: Default
Hardware watchpoint supported by the target 
COM frequency = 4000 kHz
ST-LINK Firmware version : V2J36M26
Device ID: 0x422
PC: 0x80007f8
ST-LINK device status: HALT_MODE
ST-LINK detects target voltage = 2.91 V
ST-LINK device status: HALT_MODE
ST-LINK device initialization OK
Waiting for debugger connection...
Waiting for connection on port 61234...
```
while it wait for a `gdb` process to connect to it

Maybe my microcontrollers are toasted?
Maybe next step is to solder one to a breakout board and try to program it on a breadboard?


Borrowed an STM32 USB programmer from a friend, got a new error about part is LOCKUP and debugger timed out.  Thought maybe finally clicking on the "Update CubeIDE" button might help. Get 1.4.2 installed and now the debugger won't even startup, just opens a window with 
```
ST-LINK Server is required to start the debug session. Download from st.com
```
Ok. Download. Install. No change. Restart the IDE. No change. :( 
This forum post suggests downgrading from 1.4.2 to 1.4.0 to get it to work. Ugh.
https://community.st.com/s/question/0D53W00000GWQKn/how-can-stlink-server-be-installed-for-use-by-stm32cubeide-

Tried to manually start the gdb session from the terminal with help here https://jacobmossberg.se/posts/2017/01/17/use-gdb-on-arm-assembly-program.html
it appeared to work and even saw `ST-LINK device status: RUN_MODE` but my board was still not working, perhaps a fundamental HW issue I just don't see. 

NRST pin is tied to the NRST pin on the SWD jumper but otherwise is floating.  It is supposed to be connected to a 40k ohm (typ) pullup internally.  There is a recommendation for a 0.1uF external capacitor to prevent parasitic resets, which is missing from the PCB design.  

#### Other Debugging Ideas
Maybe try the i2c lines as usart data lines for programming?
http://eleceng.dit.ie/frank/arm/STM32F030ISP/index.html

Maybe try after driving boot pin high:
https://electronics.stackexchange.com/questions/410386/stm32f0-boot0-pin-and-swd-programming 



