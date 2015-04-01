#ifndef SERIALTHREAD_H_INCLUDED
#define SERIALTHREAD_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include <libserialport.h>


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class SerialThread : public Thread
{
public:
    //==============================================================================
    SerialThread();
    ~SerialThread();
    
    void run();
    int openSerialDevice(const String &device, int baudRate);

private:

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerialThread)
};


#endif  // SERIALTHREAD_H_INCLUDED
