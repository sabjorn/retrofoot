#ifndef SERIALTHREAD_H_INCLUDED
#define SERIALTHREAD_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


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

private:


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerialThread)
};


#endif  // SERIALTHREAD_H_INCLUDED
