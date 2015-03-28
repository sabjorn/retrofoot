#include "SerialThread.h"
#include <iostream>



SerialThread::SerialThread() 
    : Thread("Serial Reader"),
      serialPort(NULL)
{
    
}

SerialThread::~SerialThread()
{
    delete serialPort;
}

int SerialThread::openSerialDevice(const String &device)
{

    serialPort = new SerialPort(device.toStdString());

    if (NULL != serialPort)
    {
	serialPort->Open();

	if (serialPort->IsOpen())
	{
	    serialPort->SetBaudRate(SerialPort::BAUD_115200);
	    return 0;
	}
    }

    return -1;
} 

void SerialThread::run()
{
    std::string tmp;

    while (!threadShouldExit()) 
    {

	serialPort->WriteByte(' ');

	if (serialPort->IsDataAvailable())
	{
	    tmp = serialPort->ReadLine();
	    for (uint32_t i = 0; i < tmp.length(); i++)
	    {
		std::cout << (int)tmp[i] << " ";
	    }
	    std::cout << std::endl;
	}

	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (1);
    }

}
