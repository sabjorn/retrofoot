#include "SerialThread.h"
#include <iostream>



SerialThread::SerialThread() 
    : Thread("Serial Reader")
{
    
}

SerialThread::~SerialThread()
{
}

void SerialThread::run()
{
    while (!threadShouldExit()) 
    {
	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (1000);
        std::cout << "Reading the serial port1!!" << std::endl;
    }

std::cout << "Got exit signal. Bim." << std::endl;
}
