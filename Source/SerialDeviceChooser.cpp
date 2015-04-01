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
    uint32_t oldId;
    sp_port **sps;
    sp_return rc;
    uint32_t itr = 0;

    while (!threadShouldExit()) 
    {
	wait (1000);

	const MessageManagerLock mml (Thread::getCurrentThread());

	if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
	    return;                 // will fail, in which case we'd better return..

	if (sp_list_ports(&sps) != SP_OK)
	    return;

	itr = 0;
	std::cout << "Devices: " << std::endl;
	while (sps[itr] != NULL)
	{
	    std::cout << sp_get_port_name(sps[itr]) << std::endl;
	    itr++;
	}

	sp_free_port_list(sps);

//	StringArray tmpDevices;
//
//	// Iterate over the files we find here. They should be links so 
//	// follow the link to the real device node. getLinkedTarget just 
//	// returns the same file if it is not a link so safe to use for
//	// OS X. 
//	while (dirIter.next())
//	{
//	    tmpDevices.add(dirIter.getFile().getLinkedTarget().getFullPathName());
//	}
//
//	oldId = getSelectedId();
//	for (uint32_t i = 0; i < tmpDevices.size(); i++)
//	{
//	    if (!devices.contains(tmpDevices[i]))
//	    {
//		oldId = tmpDevices[i].hash();
//	    }
//	}
//
//	devices = tmpDevices;
//
//	clear();
//	for (uint32_t i = 0; i < devices.size(); i++)
//	{
//	    addItem(devices[i], devices[i].hash());
//	}
//	setSelectedId(oldId);

    }

}
