/*
 * Standard libraries
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>
#include <time.h>

/*
 * API Libraries
 */
#include "apitypes.h"
#include "cmd_def.h"
#include "actions.h"
#include "uart.h"
#include "action_done.h"
#define UART_TIMEOUT 1

/*
 * Talk to the peripheral
 */
void talkToPeripheral(bd_addr connect_addr, int addressType, int minConnVal, int maxConnVal, int timeOut, int latency){
    printf("Connecting to peripheral...\n");
    ble_cmd_gap_set_mode(0x00,0x00);
    ble_cmd_gap_connect_direct(&connect_addr, addressType, minConnVal, maxConnVal, timeOut, latency);

    int breakInt = 1;
    while(breakInt){
        if (read_message(UART_TIMEOUT) > 0) {
            break;
        }
        //usleep(1000);

        breakInt = control();
    }
}

void talkToMaster(){
    printf("Waiting for connection...\n");
    eventDone = 0;
    ble_cmd_gap_set_mode(0x03,0x02);
    while(eventDone == 0){
        if (read_message(UART_TIMEOUT) > 0) {
            break;
        }
        usleep(1000);
        printf("Waiting....\n");
    }

    int breakInt = 1;
    while(breakInt){
        if (read_message(UART_TIMEOUT) > 0) {
            break;
        }
        //usleep(1000);

        breakInt = control();
    }


}


int control(){
    /*
     * Ask what to do?
     */
    const int messageSize = 20;
    char c[messageSize];
    printf("What do you want to do?\n");
    printf("1. Write string of length at maximum 20 bytes.\n");
    printf("2. Read incomming message.\n");
    printf("3. Abort.\n");
    printf("Any number just update any message.\n");
    printf("Enter: ");
    scanf("%s", c);

    /*
     * Select
     */
    if(strcmp(c, "1") == 0){
        memset(c, 0, messageSize*sizeof(char));
        printf("Enter text: ");
        scanf("%s", c);
        ble_cmd_attclient_attribute_write(0x00, 20, 20, (uint8*) c);
    }else if(strcmp(c, "2") == 0){
        printf("Message: %s\n", (char*) message);
    }else if(strcmp(c, "3") == 0){
        printf("Disconnecting...\n");
        ble_cmd_connection_disconnect(0); // Close connection
        return 0; // Break
    }
    return 1; // Continue
}

/*
 * Get information about the BLE device connected at your COM port
 */
void getInfo(){
    printf("Collecting info...\n");
    responseDone = 0; // We set this to 0 because we want to have our data first.
    ble_cmd_system_get_info();

    while(responseDone == 0){
        if (read_message(UART_TIMEOUT) > 0) {
            break;
        }
        printf("responseDone = %d\n", responseDone);
        usleep(1000);
    }
}

/*
 * Scanning for other BLE devices
 */
void scanning(){
    printf("Scanning...\n");
    ble_cmd_gap_discover(gap_discover_observation);
    char s[1];
    while(1){
        if(read_message(UART_TIMEOUT) == 0){
            printf("Stop scanning (y/N): ");
            scanf("%s", s);
            if(strcmp(s, "y") == 0){
                break;
            }
        }else{
            break;
        }
    }
}

/*
 * Read message from BLED112
 */
int read_message(int timeout_ms) {
	unsigned char data[256]; // enough for BLE
	struct ble_header hdr;

	int r = uart_rx(sizeof(hdr), (unsigned char *) &hdr, UART_TIMEOUT);
	if (!r) {
		return -1; // timeout
	} else if (r < 0) {
		printf("ERROR: Reading header failed. Error code:%d\n", r);
		return 1;
	}

	if (hdr.lolen) {
		r = uart_rx(hdr.lolen, data, UART_TIMEOUT);
		if (r <= 0) {
			printf("ERROR: Reading data failed. Error code:%d\n", r);
			return 1;
		}
	}

	const struct ble_msg *msg = ble_get_msg_hdr(hdr);

	if (!msg) {
		printf("ERROR: Unknown message received\n");
		exit(1);
	}

	msg->handler(data);

	return 0;
}

