#include "SerialThread.h"
#include <iostream>



SerialThread::SerialThread() 
    : Thread("Serial Reader"),
      sp(NULL)
{
    
}

SerialThread::~SerialThread()
{
    if (NULL != sp)
    {
	sp_free_port(sp);
    }
}

int SerialThread::openSerialDevice(const String &device, int baudRate)
{
    sp_return rc;

    rc = sp_get_port_by_name(device.toRawUTF8(), &sp);

    if ((SP_OK != rc) || (NULL == sp))
	return -1;

    rc = sp_open(sp, SP_MODE_READ_WRITE);

    if (SP_OK != rc)
	return -1;

    // Config baud rate
    rc = sp_set_baudrate(sp, baudRate);

    if (SP_OK != rc)
	return -1;

    return 0;

} 

void SerialThread::run()
{
    const size_t BUF_SIZE = 256;

    char buf[BUF_SIZE];
    char wbuf = ' ';

    while (!threadShouldExit()) 
    {

	sp_blocking_write(sp, &wbuf, 1, 1000); 

	if (sp_output_waiting(sp) > 0)
	{
	    uint32 nRead = sp_nonblocking_read(sp, buf, BUF_SIZE);
	    for (uint32_t i = 0; i < nRead; i++)
	    {
		std::cout << (int)buf[i] << " ";
	    }
	    std::cout << std::endl;
	}
	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (5);
    }

}
