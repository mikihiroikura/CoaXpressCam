#include "RS232c.h"
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
	RS232c mbed;
	char buf[256];

	mbed.Connect("COM4", 115200, 8, NOPARITY, 0, 0, 0, 5000, 20000);
	mbed.Send("240\r\n");
	mbed.Read_CRLF(buf, 256);
	printf(buf);

	int num = atoi(buf);
	printf("%d\n", num);

	return 0;
}