/*
 ============================================================================
 Name        : BLED112.c
 Author      : Daniel Mårtensson
 Version     : 1.0
 Copyright   : Daniel Mårtensson
 Description : Communicating UART via the BLED112 to other BLE-devices
 ============================================================================
 */

/*
 * Standard libraries for Windows API
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

/*
 * libraries from GlueGiga API
 */
#include "cmd_def.h"
#include "uart.h"
#include "actions.h"
#define UART_TIMEOUT 1000

/*
 * Fields
 */
bd_addr connect_addr;

/*
 * Enumerations for actions - Add one if you want to use different action.
 */
enum actions {
	action_none,
	action_scan,
	action_connect_peripheral,
	action_info,
	action_connect_master
};
enum actions action = action_none;

/*
 * Declare functions for COM port
 */
void askForComPort(char* COM);
void output(uint8 len1, uint8* data1, uint16 len2, uint8* data2);
void hexToUint8(const char* macAddress);
void askForMacAdress(char* MAC);

/*
 * Main function
 */
int main() {

	/*
	 * Introduction
	 */
	char START[1];
	char COM[80];
	char MAC[80];
	printf("\t\t\tBLED112 for Windows and Linux using MinGW/GCC - Using BlueGiga API\t\t\t\n");
	printf("What do you want to do?:\n");
	printf("1. Scan all the peripheral BLE devices MAC addresses around you: \n");
	printf("2. Connect a peripheral BLE device and start talking to it: \n");
	printf("3. Info about the connected BLED112 device in your COM port: \n");
	printf("4. Talk to a master BLED device from your peripheral BLED device: \n");
	printf("Select a number: ");
	scanf("%s", START);

	/*
	 * Get the COM port or the MAC-address
	 */
	if (strcmp(START, "1") == 0) {
		askForComPort(COM);
		action = action_scan; // Scan all the BLE devices MAC addresses
	} else if (strcmp(START, "2") == 0) {
		askForComPort(COM);
		askForMacAdress(MAC);
		hexToUint8(MAC);
		action = action_connect_peripheral; // Connect to a BLE device
	} else if (strcmp(START, "3") == 0) {
		askForComPort(COM);
		action = action_info; // Get the info about the BLED112 connected to your COM port
	} else if (strcmp(START, "4") == 0) {
		askForComPort(COM);
		action = action_connect_master; // Start talking to the master BLED
	} else {
		printf("Unknow state %s. Exit\n", START);
		return 0;
	}

	/*
	 * Create a function where we can send data to the COM port
	 */
	bglib_output = output;

	/*
	 *  Open our COM port now
	 */
	if (uart_open(COM)) {
		printf("ERROR: Unable to open serial port: %s\n", COM);
		printf("Closing port: %s\n", COM);
		uart_close();
		return 0;
	} else {
		printf("SUCCESS: Open serial port: %s\n", COM);
	}

	/*
	 * Reset dongle to get it into known state
	 */
	//ble_cmd_gap_end_procedure();
	ble_cmd_system_reset(0);
	uart_close();
	do {
		usleep(500000); // 0.5s
	} while (uart_open(COM));


	/*
	 * Actions
	 */
	if (action == action_connect_peripheral) {
        talkToPeripheral(connect_addr, gap_address_type_public, 40, 60, 100, 0);
	}else if(action == action_scan) {
        scanning();
	}else if(action == action_info){
        getInfo();
	}else if(action == action_connect_master){
		printf("Start listing to the master...\n");
		talkToMaster();
	}

	printf("Closing port: %s\n", COM);
	uart_close();

	return 0;
}

/*
 * Ask for the COM port
 */
void askForComPort(char* COM) {
	/*
	 * Begin to ask for COM port - Where the BLED112 is located
	 */
	printf("Enter COM port e.g /dev/ttyXXXX or COMx: ");
	scanf("%s", COM);
	printf("Selected COM port is: %s\n", COM);
}

/*
 * Output connection to the COM port
 */
void output(uint8 len1, uint8* data1, uint16 len2, uint8* data2) {
	if (uart_tx(len1, data1) || uart_tx(len2, data2)) {
		printf("ERROR: Writing to serial port failed\n");
		exit(1);
	}
}

/*
 * Get the MAC address into uint8
 */
void hexToUint8(const char* macAddress) {
	short unsigned int addr[6];
	int macAddresSize = sscanf(macAddress, "%02hx:%02hx:%02hx:%02hx:%02hx:%02hx", &addr[5], &addr[4], &addr[3], &addr[2], &addr[1], &addr[0]);
	if (macAddresSize == 6) {
		for (int i = 0; i < 6; i++) {
			connect_addr.addr[i] = addr[i];
		}
	}else{
		printf("MAC address size need to be 6. This address have the length %d\n", macAddresSize);
	}
}

/*
 * Ask for the MAC address
 */
void askForMacAdress(char* MAC) {
	/*
	 * Begin to ask for COM port - Where the BLED112 is located
	 */
	printf("Enter 6 hex MAC address e.g ´hex:hex:hex:hex:hex:hex´: ");
	scanf("%s", MAC);
	printf("Selected MAC address is: %s\n", MAC);
}


