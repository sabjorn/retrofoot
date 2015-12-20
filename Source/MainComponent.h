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

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public Component,
                               public ButtonListener,
                               public ActionListener,
                               public TextEditor::Listener,
                               public ComboBox::Listener,
                         	   public Slider::Listener,
		                       public Timer
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
    uint8 midiVelocity(uint32 keyIdx, float value, bool attack);
    uint8 midiProgram();
    void sliderValueChanged(Slider *s);
	void timerCallback() { keyboardMonitor.repaint(); };
	
    float noteOnThresh() { return sliderNoteOnThresh.getMaxValue(); };
    float noteOffThresh() { return sliderNoteOnThresh.getMinValue(); };
    float aftertouchThresh() { return sliderAftertouchThresh.getMaxValue(); };
    float aftertouchDepth() { return sliderAftertouchDepth.getValue(); }; 
    
    // Constants
    static const uint32 xSize = 460;
    static const uint32 ySize = 450;
    static const uint32 baudRate = 115200;
    static const uint32 numKeys = 32;

	// GUI Placement Constants
	static const uint32 enablex = 20;
	static const uint32 col1x   = 40;
	static const uint32 col2x   = 110;
	static const uint32 col3x   = 260;
	static const uint32 col4x   = 320;
	static const uint32 col1w   = col2x-col1x;
	static const uint32 col2w   = col3x-col2x-20;
	static const uint32 col3w   = col4x-col3x;
	static const uint32 col4w   = xSize-20-col4x;
	
    // IDs for stuff
    static const uint32 ID_AFTERTOUCH_MONO = 1;
    static const uint32 ID_AFTERTOUCH_POLY = 2;
	static const uint32 ID_VELOCITY_AUTO = 1;
	static const uint32 ID_VELOCITY_FIXED = 2;

    // Serial Port Stuff
    GroupComponent      groupSerialSetup;
    ToggleButton        enableSerial;
    Label               labelSerialDevice;
    SerialDeviceChooser comboSerialDevice;
    SerialThread        serialPortReader;
    Label               labelSerialStatus;
    Label               labelSerialIndicator;
	String              serialData;
    
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
	Label             labelVelocityParam;
	Slider            sliderVelocityParam;
    ComboBox          aftertouchMidi;
    Label             labelMidiAftertouch;
	Label             labelNoteOnThresh;
    Slider            sliderNoteOnThresh;
    Slider            sliderAftertouchThresh;

	Label             labelAftertouchDepth;
	Slider            sliderAftertouchDepth;
	Label             labelAftertouchThresh;
	
    // Monitor Stuff
	GroupComponent groupMonitor;
    KeyboardMonitorComponent keyboardMonitor;

    // OSC sender
    OSCSender oscSender;

    // Calibration Stuff
    KeyCalibrationDialog *kcd;
    uint32 keyMax[numKeys];
    uint32 keyMin[numKeys];
    bool  isSynced;
	
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


#endif  // MAINCOMPONENT_H_INCLUDED
