#include "RS232c.h"
#include <Windows.h>
#include <cstdint>
#include <iostream>
#include <stdio.h>

using namespace std;

int main() {
	RS232c mbed;
	char buf[256];
	char message[256] = "R,100,150,10,\r";

	mbed.Connect("COM4", 115200, 8, NOPARITY, 0, 0, 0, 5000, 20000);
	mbed.Send(message);
	//int sizes = sizeof(message) / sizeof(message[0]);
	/*while (1) {
		mbed.Read_CRLF(buf, 256);
		printf(buf);
	}*/
	/*int num = atoi(buf);
	printf("%d\n", num);*/

	return 0;
}