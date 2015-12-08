#include "MidiDeviceChooser.h"
#include <iostream>

MidiDeviceChooser::MidiDeviceChooser(const String &componentName) 
    : ComboBox(componentName),
      Thread("Midi Device Detect")
{
    
}

MidiDeviceChooser::~MidiDeviceChooser()
{
    stopThread(1000);
}

void MidiDeviceChooser::run()
{
    uint32_t selId;

    while (!threadShouldExit()) 
    {
	wait (500);

	const MessageManagerLock mml (Thread::getCurrentThread());

	if (! mml.lockWasGained())  // if something is trying to kill this job, the lock
	    return;                 // will fail, in which case we'd better return..

	StringArray tmpDevices = MidiOutput::getDevices();

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
