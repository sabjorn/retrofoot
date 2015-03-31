/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SerialThread.h"
#include "SerialDeviceChooser.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               public ButtonListener,
                               public MessageListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void paint(Graphics &g);
    void buttonClicked(Button *button);
    void handleMessage(const Message &message);

private:

    // Constants
    static const uint32_t xSize = 400;
    static const uint32_t ySize = 200;

    // Serial Port Stuff
    GroupComponent      groupSerialSetup;
    Label               labelSerialDevice;
    SerialDeviceChooser comboSerialDevice;
    Label               labelSerialBaud;
    ComboBox            comboSerialBaud;
    SerialThread        serialPortReader;
    TextButton          buttonStopGo;

    // Mode Stuff
    GroupComponent groupMode;
    ToggleButton   buttonModeSelect[3];

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
