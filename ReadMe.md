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

![](https://raw.githubusercontent.com/DanielMartensson/C-Applications/master/Bluetooth%20Blue%20Giga%20API/index.jpeg?raw=true)


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

# PWM controlling a hydraulic haning load
This project is done by turning a potentiometer (10K) so an embedded system (STM32) can read the analog signals. The embedded system then send out a PWM frequency in high Hz so it will control a spole at hydraulic propotional valve. The valve will there fore release a velocity of oil to the hydraulic cylinder and then build a pressure due to the mass as load. The variable orifice is a shunt so the valve can operate the position on the mass. Else, it will be a very static system. Even if this is possible to control, it's still very nonlinear and I tried a Model Predictive Control system where the output of the state space model was velocity. But it was very hard to control.

I therefore will focusing on Fuzzy-Logic for hydraulical systems.

Please watch the video inside the folder "Potentiometer To PWM Generator" so you can see how it looks like when I turn the potentiometer and the load goes up and down.

![](https://raw.githubusercontent.com/DanielMartensson/C-Applications/master/Potentiometer%20To%20PWM%20Generator/Bench.jpeg?raw=true)

![](https://raw.githubusercontent.com/DanielMartensson/C-Applications/master/Potentiometer%20To%20PWM%20Generator/Schematic.png?raw=true)
