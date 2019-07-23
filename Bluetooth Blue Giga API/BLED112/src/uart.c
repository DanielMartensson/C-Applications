//
// Bluegiga’s Bluetooth Smart Demo Application
// Contact: support@bluegiga.com.
//
// This is free software distributed under the terms of the MIT license reproduced below.
//
// Copyright (c) 2012, Bluegiga Technologies
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this
// software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
// EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
// MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR PURPOSE.
//

#include <stdio.h>

#include "uart.h"

#ifdef PLATFORM_WIN

#ifdef _MSC_VER
#define snprintf _snprintf
#endif

#include <windows.h>
#include <Setupapi.h>

HANDLE serial_handle;

int uart_open(char *port) {
	char str[20];

	snprintf(str, sizeof(str) - 1, "\\\\.\\%s", port);
	serial_handle = CreateFileA(str, GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ | FILE_SHARE_WRITE,
			NULL, OPEN_EXISTING, 0, //FILE_FLAG_OVERLAPPED,
			NULL);

	if (serial_handle == INVALID_HANDLE_VALUE) {
		return -1;
	}

	return 0;
}
void uart_close() {
	CloseHandle(serial_handle);
}

int uart_tx(int len, unsigned char *data) {
	DWORD r, written;
	while (len) {

		r = WriteFile(serial_handle, data, len, &written,
		NULL);
		if (!r) {
			return -1;
		}
		len -= written;
		data += len;
	}

	return 0;
}
int uart_rx(int len, unsigned char *data, int timeout_ms) {
	int l = len;
	DWORD r, rread;
	COMMTIMEOUTS timeouts;
	timeouts.ReadIntervalTimeout = MAXDWORD;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = timeout_ms;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;

	SetCommTimeouts(serial_handle, &timeouts);
	while (len) {
		r = ReadFile(serial_handle, data, len, &rread,
		NULL);

		if (!r) {
			l = GetLastError();
			if (l == ERROR_SUCCESS)
				return 0;
			return -1;
		} else {
			if (rread == 0)
				return 0;
		}
		len -= rread;
		data += len;
	}

	return l;
}

#else

#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <unistd.h>

int serial_handle;

void uart_list_devices() {
}

int uart_open(char *port) {
	struct termios options;
	int i;

	serial_handle = open(port, (O_RDWR | O_NOCTTY /*| O_NDELAY*/));

	if (serial_handle < 0) {
		return -1;
	}

	/*
	 * Get the current options for the port...
	 */
	tcgetattr(serial_handle, &options);

	/*
	 * Set the baud rates to 115200...
	 */
	cfsetispeed(&options, B115200);
	cfsetospeed(&options, B115200);

	/*
	 * Enable the receiver and set parameters ...
	 */
	options.c_cflag &= ~(PARENB | CSTOPB | CSIZE | CRTSCTS | HUPCL);
	options.c_cflag |= (CS8 | CLOCAL | CREAD);
	options.c_lflag &= ~(ICANON | ISIG | ECHO | ECHOE | ECHOK | ECHONL | ECHOCTL
			| ECHOPRT | ECHOKE | IEXTEN);
	options.c_iflag &= ~(INPCK | IXON | IXOFF | IXANY | ICRNL);
	options.c_oflag &= ~(OPOST | ONLCR);

	//printf( "size of c_cc = %d\n", sizeof( options.c_cc ) );
	for (i = 0; i < sizeof(options.c_cc); i++)
	options.c_cc[i] = _POSIX_VDISABLE;

	options.c_cc[VTIME] = 0;
	options.c_cc[VMIN] = 1;

	/*
	 * Set the new options for the port...
	 */
	tcsetattr(serial_handle, TCSAFLUSH, &options);

	return 0;
}
void uart_close() {
	close(serial_handle);
}

int uart_tx(int len, unsigned char *data) {
	ssize_t written;

	while (len) {
		written = write(serial_handle, data, len);
		if (!written) {
			return -1;
		}
		len -= written;
		data += len;
	}

	return 0;
}
int uart_rx(int len, unsigned char *data, int timeout_ms) {
	int l = len;
	ssize_t rread;
	struct termios options;

	tcgetattr(serial_handle, &options);
	options.c_cc[VTIME] = timeout_ms / 100;
	options.c_cc[VMIN] = 0;
	tcsetattr(serial_handle, TCSANOW, &options);

	while (len) {
		rread = read(serial_handle, data, len);

		if (!rread) {
			return 0;
		} else if (rread < 0) {
			return -1;
		}
		len -= rread;
		data += len;
	}

	return l;
}

#endif
