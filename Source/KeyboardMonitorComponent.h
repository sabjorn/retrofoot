#ifndef KEYBOARDMONITORCOMPONENT_H_INCLUDED
#define KEYBOARDMONITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class KeyboardMonitorComponent : public MidiKeyboardComponent,
                                 public ActionBroadcaster
{

public:
    //==============================================================================
    KeyboardMonitorComponent(uint32_t numKeys);
    ~KeyboardMonitorComponent();

    void clearKeys();
    void setKeyPosition(int midiNoteNumber, float keyPosition);
	void setNoteOn(float thresh);
	void setNoteOff(float thresh);
	void setAftertouch(float thresh);

private:

    MidiKeyboardState keyboardState;
    void drawWhiteNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &lineColour, const Colour &textColour);
    void drawBlackNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &noteFillColour);
    String getWhiteNoteText(const int midiNoteNumber) { return String::empty; };
    bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e);

    float *keyGauge;
	float aftertouchThresh;
	float noteOnThresh;
	float noteOffThresh;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardMonitorComponent)
};



#endif  // KEYBOARDMONITORCOMPONENT_H_INCLUDED
