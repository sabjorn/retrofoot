#include "SerialDeviceChooser.h"
#include "libserialport.h"
#include <iostream>

SerialDeviceChooser::SerialDeviceChooser(const String &componentName) 
    : ComboBox(componentName),
      Thread("Serial Device Detect")
{
    
}

SerialDeviceChooser::~SerialDeviceChooser()
{
    stopThread(1000);
}

void SerialDeviceChooser::run()
{
    uint32_t selId;
    sp_port **sps;

    while (!threadShouldExit()) 
    {
	wait (500);

	const MessageManagerLock mml (Thread::getCurrentThread());

	if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
	    return;                 // will fail, in which case we'd better return..

	if (sp_list_ports(&sps) != SP_OK)
	    return;

	StringArray tmpDevices;
	uint32_t itr = 0;
	while (sps[itr] != NULL)
	{
	    tmpDevices.add(sp_get_port_name(sps[itr]));
	    itr++;
	}

	sp_free_port_list(sps);

	selId = getSelectedId();
	for (uint32_t i = 0; i < tmpDevices.size(); i++)
	{
	    if (!devices.contains(tmpDevices[i]))
	    {
		// Select any device that is "New"
		// if multiple devices show up it will select whichever
		// one shows up last. Oh well.
		selId = tmpDevices[i].hash();
	    }
	}

	devices = tmpDevices;

	clear();
	for (uint32_t i = 0; i < devices.size(); i++)
	{
	    addItem(devices[i], devices[i].hash());
	}
	setSelectedId(selId);

    }

}
