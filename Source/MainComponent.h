/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "SerialThread.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               public ButtonListener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void paint(Graphics &g);
    void buttonClicked(Button *button);

private:

    Label    labelSerialDevice;
    Label    labelOSCHost;
    Label    labelOSCPort;
    
    TextEditor textSerialDevice;
    TextEditor textOSCHost;
    TextEditor textOSCPort;

    TextButton buttonStopGo;

    SerialThread serialPortReader;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
