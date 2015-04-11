#ifndef SERIALTHREAD_H_INCLUDED
#define SERIALTHREAD_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParseStream.h"

#ifndef RETROFOOT_SERIAL_SIM
// Uses libserialport. 
#include <libserialport.h>
#endif


class SerialThread : private Thread,
                     public ActionBroadcaster
{
public:
    //==============================================================================
    SerialThread();
    ~SerialThread();

    int start(const String &device, int baudRate);
    int stop();
    
private:

    void run();

#ifndef RETROFOOT_SERIAL_SIM
    sp_port *sp;
#else
    MemoryBlock simData;
#endif

    ParseStream parser;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerialThread)
};


#endif  // SERIALTHREAD_H_INCLUDED
