# AJ2019 Electronics Kit

## Using this Repo
Clone this repo to your local machine and commit all changes in a branch. Once the changes have been verified, they will be merged back to the trunk.
[GitHub Basics Tutorial](https://youtu.be/0fKg7e37bQE)
[GitHub Branch Tutorial](https://youtu.be/oFYyTZwMyAg)


## Functional Requirements
Note: Values in `quote sections` are approximate and need to be decided based on testing.
1. Simon Says Game
   - A ‘random’ sequence of flashes occurs on the 3 LEDs at a very ‘viewable’ pace. The sequence starts at `2 or 3` flashes. The user then has to press the buttons corresponding to the LEDs in that same sequence. The sequence is lengthened by 1 and the process repeats.
   - A time out will occur after waiting for user input for more than `6sec`.
   - When the user gets the sequence wrong, a game over animation will play.
   - An expansion feature is a readout of the level attained when the game ends. One possible way is to have a 1s LED, a 10s LED and 100s LED. Each unit LED could flash for the number of 1s, 10s, 100s in the final level. Realistically 100s would not be needed.
   - The corner case of ‘finishing the game’ by reaching the maximum length of the pattern should also be handled by an end of game animation.
2. Board Connection Collection (working name)
   - Each board will be programmed with a unique identifier. Either a serial number or Scout’s AJ2019 ID (probably just serial for process simplicity).
   - Two boards are connected and exchange unique identifiers. The boards each store all other identifiers to ensure that the number of connections does not include duplicates.
   - The board will then display the amount of connections using the LEDs.
     - One option is to display the number of connections by having a 1s LED, 10s LED and 100s LED. Each LED flashes 0-9 times to signify the value of that unit. Need to determine which order is easier to understand `1s->10s->100s or 100s->10s->1s`.
     - Another option is to have thee different colours of PCBs (the same as the LEDs), each LED could display by brightness the number of boards connected of that colour.
   - An additional feature could be to have a ‘base station’ at the Activity where Scouts can come up and connect their boards. The base station would interrogate the board to see how many connections it has made. The station would have a large screen displaying a leader board of the number of connections. Scouts could then enter their name (or perhaps scan their ID card) to give the name for that entry. Scanning IDs removes the possibility of Scouts entering crude/rude names.
3. Menu Functionality
   - There needs to be a simple and even instinctive way to switch between game mode and connection mode. Or eliminate the need so that connection can occur at any time regardless of gameplay. There also needs to be a way to reset the game. As multiple button presses cannot be detected, long presses on single buttons will likely be the method for the above actions. 
## Technical Specifications and Considerations:
The schematic is available from [Circuit Maker](https://workspace.circuitmaker.com/Projects/Details/Lachlan-Harper/AJ2019).
Circuit Maker (free) has been used for hardware development. MPLABX IDE has been used for firmware development. The MPLAB Code Configurator (MCC) plugin has been used for quick firmware setup. A PICKit3 has been used as the programmer. 

### Buttons
The three buttons are all connected to a single analog pin. This is done due to the minimal pin availability with an 8 pin chip. The analog value is read and from this the current button pressed can be determined. Multiple button pressed can not be detected. The button which results in the lowest voltage/ADC value is detected in the case of multiple button presses.

### LEDs
The three LEDs are all connected to PWM Module capable pins. The three PWM Modules are independent. See note in Connection about shared pins. For power saving, LEDs run at approx. `70%` duty cycle tend to look 100% brightness.

### Connection (to other boards)
The connection used is Asynchronous Serial. Only three physical connections are needed; TX, RX, GND. Duplex communication is used as there are two masters. A 1-Wire bus would require a master and slave which cannot be easily assigned/determined.
The TX pin is shared with the button ADC input. The serial module must be disabled (in firmware) in order to read valid data from the ADC.
A method of detecting a connection is yet to be determined. As the TX pin is pulled high in all idle states (ADC or Serial), the RX pin could be pulled low (to a lesser degree than the TX pulled high) and an interrupt (or poll) set to wait for a rising edge on RX. This would occur when the TX pin (high) connects to the RX and pulls RX high. A delay (perhaps `0.5sec`) could then elapse before communicating to ensure a stable connection.

### Unique Identifier
MPLABX IPE may be useful for unique identifiers in a serial format; where for each board programmed the unique identifier is incremented. MPLABX IPE (Integrated Programming Environment) as opposed to IDE (Integrated Development Environment). MPLABX IPE appears to be capable of programming a serial number to a specified location in flash memory.

### Storing Collected Unique Identifiers
A variable will store the count of unique identifiers collected. When a connection is made, the unique identifier received is checked against the unique identifiers stored in memory to ensure it is unique, before incrementing the counter. The counter will be stored in Non-Volatile memory (perhaps the High Endurance Flash).
The storage of unique identifiers presents a problem that can not be fully solved with the constraints of the hardware. With the aim of producing 2000 boards, an 11-bit (211 = 2048) address is needed. The PIC12F1572 chip has only 2K Flash and 128B High Endurance Flash (rather than EEPROM). The 2K flash must store both the program and the number of connections. A few approaches:
- Store only high 8-bits of unique identifier in (2[11-bit-8-bit] = 8) 8 tables. Using the low bits as table indexes should provide a more even distribution across the tables.
- Consider each bit in the flash space as one unique identifier (first bit is 0000, second is 0001 etc.) and set/clear the corresponding bit to the unique identifier received. This however would require 256 bytes of flash.
- Have three colours of boards (same colours as LEDs). In this case the stored values could be split in three. Three count variables in flash would also be required. The colours would likely correspond to lower, middle and upper thirds of the unique identifier range.
 - Use a circular buffer. Store the unique identifiers sequentially in flash and when flash is full, simply override the oldest (chronologically) byte of flash. A variable (pointer) keeping track of oldest byte (head of buffer) is needed.
   - If tables are used to store only single bytes, the oldest byte of the table must be overridden which may not be the oldest in total.
   - This method would also allow for a reasonable implementation where the 11-bit unique identifiers are stored in 16-bit (2 byte) allocations in flash.
Ultimately, the solution will have the simplest program and best encoding of data as the program size subtracts from the storage available. To give an idea of program size, my initial program which simply initialises peripherals and runs a very small program is 607B (30%). Though this could likely be thinned out a lot as the MCC generated code is likely very inefficient with lots of function calls. The compiler optimization could probably be set to improve program space too as speed is not a big issue.

### Low Power
The circuit runs on a 3V 2032 coin cell battery. There is no power switch. This is because the board will be set to enter low power mode after a short amount of inactivity. In low power mode the microcontroller should draw only microamps.
Low power mode can be entered from firmware and exited by several different methods.
- Pulling the reset pin low will simply reset the board.
- External interrupts (available on all pins) can be used to wake up
- Timer interrupts can be used to wake up at regular intervals
The board will go to sleep after `10sec` inactivity, then wake (somehow) when the user presses a button to ‘turn it on’ and use it.