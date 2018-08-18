#include <windows.h>
#ifndef SERIALPORT_H
#define SERIALPORT_H

#define ARDUINO_WAIT_TIME 1
#define MAX_DATA_LENGTH 6


class SerialPort
{
private:
	HANDLE handler;
	bool connected;
	COMSTAT status;
	DWORD errors;
public:
	SerialPort(const char *portName);
	~SerialPort();

	int readSerialPort(char *buffer, unsigned int buf_size);
	bool writeSerialPort(char *buffer, unsigned int buf_size);
	bool isConnected();
};

#endif // SERIALPORT_H
