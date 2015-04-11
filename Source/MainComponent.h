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
#include "KeyboardMonitorComponent.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               public ButtonListener,
                               public ActionListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void paint(Graphics &g);
    void buttonClicked(Button *button);
    void actionListenerCallback(const String& message);

private:

    // Constants
    static const uint32_t xSize = 400;
    static const uint32_t ySize = 250;

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

    Label          labelOSCHost;
    Label          labelOSCPort;
    TextEditor     textOSCHost;
    TextEditor     textOSCPort;

    // Monitor Stuff
    KeyboardMonitorComponent keyboardMonitor;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
