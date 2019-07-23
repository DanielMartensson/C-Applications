# What is this?

This is a collection project, like my other projects. In this project repository, I use the C programming language, Atollic TrueSTUDIO and CubeMX to program a STM32 microcontroller and Eclipse CTD to create C-applications for desktop. I normaly use Java for embedded too, but only for IoT. When it comes to requriments of speed for control systems or communications, C is the only choice.

# USB IO
This is a example project how to control three PWM outputs, PA8, PA9 and PA10, and read analog values from PA0, PA1 and P4 
with a 12-bit ADC. The PWM has a period of 1 second and the PWM is configured as 8-bit.

The ADC sends back a string of the adc values "'PA0','PA1','PA4'\r\n", (without ' and ") with baudrate 115200 all the time. 
The PWM want to have a word of 8-bit, for example "AAA" or "A@&" or "A=ÿ", where the first symbol is the 8-bit ASCII character 
for PA8, the second symbol is PA9 and the last symbol is for PA10. The symbol ÿ will give 100% duty call because ÿ is ASCII 255.

![](https://github.com/DanielMartensson/C-Applications/blob/master/USB-IO/USB-IO-STM32.jpg?raw=true)

Here is an example how to send those ASCII numbers to the USB-IO. 

![](https://github.com/DanielMartensson/C-Applications/blob/master/USB-IO/USB-IO.png?raw=true)

# Bluetooth Blue Giga API - GBLib
This is a working example how to use the BLExxxx modules such as BLED112 USB Bluetooth Dongle. Fully portable for both Windows and Linux.
Written in pure C code and is terminal based. In Linux, you must be root to access this BLExxxx module from the serial port.

This example is built on current example from Silicon Labs, but I have extended it with a chat function by writing some few lines of code.

![](https://github.com/DanielMartensson/C-Applications/blob/master/Bluetooth Blue Giga API/index.jpeg?raw=true)


## How to use
Flash the BLE module with the hex file located in the "ubdcdc" folder from the "Bluetooth Blue Giga API" folder. Then you need to use the BlueGiga GUI and from the menu -> 
command -> DFU. There you will burn the hex into your BLExxxx module. 

Once your have done that, you shall open the "BLED112" folder from the "Bluetooth Blue Giga API" folder. It's a Eclipse project. Best way is to install Eclipse CDT 
and import the project: File -> Import... -> General -> Existing Projects into Workspace. 

Compile the project and then run it and write in your COM port in Windows or your dev-port in Linux.

Example:
```
			BLED112 for Windows and Linux using MinGW/GCC - Using BlueGiga API			
What do you want to do?:
1. Scan all the peripheral BLE devices MAC addresses around you: 
2. Connect a peripheral BLE device and start talking to it: 
3. Info about the connected BLED112 device in your COM port: 
4. Talk to a master BLED device from your peripheral BLED device: 
Select a number: 2
Enter COM port e.g /dev/ttyXXXX or COMx: /dev/ttyACM1
Selected COM port is: /dev/ttyACM1
Enter 6 hex MAC address e.g ´hex:hex:hex:hex:hex:hex´: 88:6b:0f:b2:77:9c
Selected MAC address is: 88:6b:0f:b2:77:9c
SUCCESS: Open serial port: /dev/ttyACM1
Connecting to peripheral...
What do you want to do?
1. Write string of length at maximum 20 bytes.
2. Read incomming message.
3. Abort.
Any number just update any message.
Enter: 1
Enter text: HELLO!
What do you want to do?
1. Write string of length at maximum 20 bytes.
2. Read incomming message.
3. Abort.
Any number just update any message.
Enter: A
Connection lost.
Reason: User on the remote device terminated the connection. Code[213]
What do you want to do?
1. Write string of length at maximum 20 bytes.
2. Read incomming message.
3. Abort.
Any number just update any message.
Enter: 
```
