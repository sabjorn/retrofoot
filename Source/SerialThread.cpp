#include "SerialThread.h"
#include <iostream>



SerialThread::SerialThread() 
    : Thread("Serial Reader")
{
    
}

SerialThread::~SerialThread()
{
}

int SerialThread::openSerialDevice(const String &device, int baudRate)
{
} 

void SerialThread::run()
{
    while (!threadShouldExit()) 
    {
	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (5);
    }

}
