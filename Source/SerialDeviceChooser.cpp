#include "SerialDeviceChooser.h"
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

    while (!threadShouldExit()) 
    {
	wait (1000);

	const MessageManagerLock mml (Thread::getCurrentThread());

	if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
	    return;                 // will fail, in which case we'd better return..

#if JUCE_LINUX
	DirectoryIterator dirIter (File("/dev/serial/by-id"), false, "*", File::findFiles);
#endif

#if JUCE_MAC
	DirectoryIterator dirIter (File("/dev"), false, "cu.*", File::findFiles);
#endif

	StringArray tmpDevices;

	// Iterate over the files we find here. They should be links so 
	// follow the link to the real device node. getLinkedTarget just 
	// returns the same file if it is not a link so safe to use for
	// OS X. 
	while (dirIter.next())
	{
	    tmpDevices.add(dirIter.getFile().getLinkedTarget().getFullPathName());
	}

	oldId = getSelectedId();
	for (uint32_t i = 0; i < tmpDevices.size(); i++)
	{
	    if (!devices.contains(tmpDevices[i]))
	    {
		oldId = tmpDevices[i].hash();
	    }
	}

	devices = tmpDevices;

	clear();
	for (uint32_t i = 0; i < devices.size(); i++)
	{
	    addItem(devices[i], devices[i].hash());
	}
	setSelectedId(oldId);

    }

}
