#ifndef KEYBOARDMONITORCOMPONENT_H_INCLUDED
#define KEYBOARDMONITORCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"


//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/

class KeyboardMonitorComponent : public MidiKeyboardComponent
{

public:
    //==============================================================================
    KeyboardMonitorComponent(uint32_t numKeys);
    ~KeyboardMonitorComponent();

    void clearKeys();
    
private:

    MidiKeyboardState keyboardState;
    void drawWhiteNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &lineColour, const Colour &textColour);
    void drawBlackNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &noteFillColour);
    String getWhiteNoteText(const int midiNoteNumber) { return String::empty; };
    bool mouseDownOnKey(int midiNoteNumber, const MouseEvent &e);
    void setKeyPosition(int midiNoteNumber, float keyPosition);

    float *keyGauge;



    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyboardMonitorComponent)
};



#endif  // KEYBOARDMONITORCOMPONENT_H_INCLUDED
