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
#include "MidiDeviceChooser.h"
#include "KeyboardMonitorComponent.h"
#include "KeyCalibrationDialog.h"
#include "lo/lo.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               public ButtonListener,
                               public ActionListener,
                               public TextEditor::Listener,
                               public ComboBox::Listener
{
public:
    //==============================================================================
    MainContentComponent();
    ~MainContentComponent();

    void resized();
    void paint(Graphics &g);
    void buttonClicked(Button *button);
    void actionListenerCallback(const String& message);
    void textEditorTextChanged(TextEditor& t);
	void comboBoxChanged(ComboBox *c);

private:

    float getCalibratedValue(uint32 keyIdx, uint32 keyValue);
	bool isMidiEnabled();
	bool isOscEnabled();
	bool isSerialEnabled();
	void updateGui();
	uint8 midiChannel();
	uint8 midiOffset();
	uint8 midiVelocity(uint32 keyIdx, float value);
	uint8 midiProgram();

	
    // Constants
    static const uint32 xSize = 400;
    static const uint32 ySize = 310;
	static const uint32 baudRate = 115200;
	static const uint32 numKeys = 32;
	

    // Serial Port Stuff
    GroupComponent      groupSerialSetup;
	ToggleButton        enableSerial;
    Label               labelSerialDevice;
    SerialDeviceChooser comboSerialDevice;
    SerialThread        serialPortReader;
	Label               labelSerialStatus;
	Label               labelSerialIndicator;

    // OSC Stuff
    GroupComponent groupOSC;
	ToggleButton   enableOSC;
    Label          labelOSCHost;
    Label          labelOSCPort;
    TextEditor     textOSCHost;
    TextEditor     textOSCPort;
	
    // MIDI Stuff
    GroupComponent    groupMidi;
	ToggleButton      enableMidi;
    MidiDeviceChooser deviceMidi;
    ComboBox          channelMidi;
    Label             labelMidiDevice;
    Label             labelMidiCh;
    MidiOutput       *midiOutput;
    bool              midiIsNoteOn[numKeys];
	float             midiPrevValue[numKeys];
	Label             labelMidiOctave;
	ComboBox          octaveMidi;
	Label             labelMidiProgram;
	ComboBox          programMidi;
	Label             labelMidiVelocity;
	ComboBox          velocityMidi;
	

    // Monitor Stuff
    KeyboardMonitorComponent keyboardMonitor;

    // OSC sender
    lo_address oscAddress;

    // Calibration Stuff
    KeyCalibrationDialog *kcd;
    uint32 keyMax[numKeys];
    uint32 keyMin[numKeys];
	float noteOnThresh;
	float noteOffThresh;
	float aftertouchThresh;
	bool  isSynced;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
