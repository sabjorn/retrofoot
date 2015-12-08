#ifndef MIDIDEVICECHOOSER_H_INCLUDED
#define MIDIDEVICECHOOSER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class MidiDeviceChooser : public ComboBox,
                            public Thread
{
public:

    MidiDeviceChooser(const String &componentName=String::empty);
    ~MidiDeviceChooser();
    
    void run();

private:

    StringArray devices;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiDeviceChooser)
};


#endif  // MIDIDEVICECHOOSER_H_INCLUDED
