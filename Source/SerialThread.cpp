#include "SerialThread.h"

/* HACK!! */
#include <termios.h>
struct sp_port {
	char *name;
	char *description;
	enum sp_transport transport;
	int usb_bus;
	int usb_address;
	int usb_vid;
	int usb_pid;
	char *usb_manufacturer;
	char *usb_product;
	char *usb_serial;
	char *bluetooth_address;
#ifdef _WIN32
	char *usb_path;
	HANDLE hdl;
	COMMTIMEOUTS timeouts;
	OVERLAPPED write_ovl;
	OVERLAPPED read_ovl;
	OVERLAPPED wait_ovl;
	DWORD events;
	BYTE pending_byte;
	BOOL writing;
	BOOL wait_running;
#else
	int fd;
#endif
};

SerialThread::SerialThread() 
    : Thread("Serial Reader")
#ifndef RETROFOOT_SERIAL_SIM
    , sp(NULL)
#endif
{
		setPriority(10);
}

SerialThread::~SerialThread()
{
    stop();
}

int SerialThread::start(const String &device, int baudRate)
{
#ifndef RETROFOOT_SERIAL_SIM
    sp_return rc;
    sp_port_config *cfg;
    termios t;
    
    // Get a port pointer by name
    rc = sp_get_port_by_name(device.toRawUTF8(), &sp);

    if ((SP_OK != rc) || (NULL == sp))
	return -1;

    // Try to open the port
    rc = sp_open(sp, SP_MODE_READ_WRITE);

    if (SP_OK != rc)
	return -1;

    // Config baud rate
    rc = sp_set_baudrate(sp, baudRate);

    if (SP_OK != rc)
	return -1;

    // Set raw mode using low-level termios stuff (HACK!!!)
    tcgetattr(sp->fd, &t);
    cfmakeraw(&t);
    if (0 != tcsetattr(sp->fd, TCSANOW, &t))
	return -1;

#else
    File simFile("./serial_sim.dat");
    if (simFile.loadFileAsData(simData) == false)
    {
	return -1;
    }
#endif

    // Reset the Parser
    parser.reset();

    // Start the thread object
    startThread();

    return 0;

} 

int SerialThread::stop()
{
#ifndef RETROFOOT_SERIAL_SIM
    sp_return rc;
#endif
    // Stop the thread
    stopThread(1000);

#ifndef RETROFOOT_SERIAL_SIM
    // If the serial port is null, do nothing.
    if (NULL == sp)
    {
	return -1;
    }

    // Close the port.
    rc = sp_close(sp);

    if (SP_OK != rc)
	return -1;

    sp_free_port(sp);

    sp = NULL;
#endif

    return 0;

} 

void SerialThread::run()
{
    StringArray strArr;

#ifndef RETROFOOT_SERIAL_SIM

    const size_t BUF_SIZE = 256;
    uint8_t buf[BUF_SIZE];

    sp_return rc;

    while (!threadShouldExit()) 
    {
	rc = sp_output_waiting(sp);

	if (rc >= 0)
	{
	    rc = sp_nonblocking_read(sp, buf, BUF_SIZE);

	    if (rc > 0) 
	    {
		// do the thing
		parser.parse(buf, rc, strArr);

/*		for (uint32_t i = 0; i < strArr.size(); i++)
		{
		    sendActionMessage(strArr[i]);
			}*/
		sendActionMessage(strArr.joinIntoString(String(" ")));
		

		strArr.clear();
	    }
	} 

	// error happened
	else if (rc < 0)
	{
	    sendActionMessage("SerialPortDied");
	    stop();
	}

	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (5);
    }

#else
    
    // Random Number Generator (random Seed)
    Random r;

    // Get a random index to start from.
    // Limit the value to the size of the memory block.
    uint32_t idx = r.nextInt(simData.getSize());
    uint32_t numBytes = 0;

    while (!threadShouldExit()) 
    {
	// Generate a random number of bytes to read.
	// A 5ms timer should give 57.6 bytes on average. 
	// Lets set the max to 115. 
	numBytes = r.nextInt(115);
	
	// If there are fewer than numBytes bytes left in the buffer
	// just return the bytes in the buffer. This will appear random-ish when it occurs.
	if ((simData.getSize() - idx) < numBytes)
	{
	    numBytes = simData.getSize() - idx;
	} 

	// Call the parser
	parser.parse(&(((uint8_t *)simData.getData())[idx]), numBytes, strArr);
	
	// Move the idx
	idx = (idx + numBytes) % simData.getSize();

	// do the thing

/*	for (uint32_t i = 0; i < strArr.size(); i++)
	{
	    sendActionMessage(strArr[i]);
		}*/

	sendActionMessage(strArr.joinInToString(String(" ")));
	
	strArr.clear();

	// sleep a bit so the threads don't all grind the CPU to a halt..
	wait (5);
    }

#endif

}
