#ifndef SERIALDEVICECHOOSER_H_INCLUDED
#define SERIALDEVICECHOOSER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class SerialDeviceChooser : public ComboBox,
                            public Thread
{
public:

    SerialDeviceChooser(const String &componentName=String::empty);
    ~SerialDeviceChooser();
    
    void run();

private:

    StringArray devices;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SerialDeviceChooser)
};


#endif  // SERIALDEVICECHOOSER_H_INCLUDED
