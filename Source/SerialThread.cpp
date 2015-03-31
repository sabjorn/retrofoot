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

int SerialThread::openSerialDevice(const String &device, SerialPort::BaudRate baudRate)
{

    serialPort = new SerialPort(device.toStdString());

    if (NULL != serialPort)
    {
	try
	{
	    serialPort->Open();
	}
	catch(SerialPort::OpenFailed) 
	{
	    std::cout << "could not open (" << device.toStdString() << ")" << std::endl;
	}

	if (serialPort->IsOpen())
	{
	    serialPort->SetBaudRate(baudRate);
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

	while (serialPort->IsDataAvailable())
	{
	    tmp = serialPort->ReadLine();
	    for (uint32_t i = 0; i < tmp.length(); i++)
	    {
		std::cout << (int)tmp[i] << " ";
	    }
	    std::cout << std::endl;
	}

	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (5);
    }

    serialPort->Close();

}
