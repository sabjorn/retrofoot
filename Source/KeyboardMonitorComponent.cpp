#include "KeyboardMonitorComponent.h"
#include <iostream>


KeyboardMonitorComponent::KeyboardMonitorComponent(uint32_t numKeys) 
    : MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setAvailableRange(0, numKeys-1);
    keyGauge = new float[numKeys];
    for (uint32_t i = 0; i < numKeys; i++)
    {
	keyGauge[i] = 0.0;
    }
}

void KeyboardMonitorComponent::drawWhiteNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &lineColour, const Colour &textColour)
{
    MidiKeyboardComponent::drawWhiteNote(midiNoteNumber, g, x, y, w, h, isDown, isOver, lineColour, textColour);
    g.setColour(Colour(0,0,128));
    g.setOpacity(0.5);
    g.fillRect(x+1,y,w-1,int(keyGauge[midiNoteNumber]*(h-1)));
}

void KeyboardMonitorComponent::drawBlackNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &noteFillColour)
{
    MidiKeyboardComponent::drawBlackNote(midiNoteNumber, g, x, y, w, h, isDown, isOver, noteFillColour);
    g.setColour(Colour(0,0,128));
    g.setOpacity(0.5);
    g.fillRect(x+1,y,w-2,int(keyGauge[midiNoteNumber]*(h-2)));
}

void KeyboardMonitorComponent::setKeyPosition(int midiNoteNumber, float keyPosition)
{
    keyGauge[midiNoteNumber] = keyPosition;
}

bool KeyboardMonitorComponent::mouseDownOnKey(int midiNoteNumber, const MouseEvent &e)
{

    return false;
}

void KeyboardMonitorComponent::actionListenerCallback(const String &message)
{
    int   key;
    float pos;

    key = (message.getIntValue() & 0xFFFF0000) >> 16;
    pos = (float)((message.getIntValue() & 0x0000FFFF) / 100.0);

    setKeyPosition(key, pos);
    repaint();
}	

KeyboardMonitorComponent::~KeyboardMonitorComponent()
{
    delete [] keyGauge;
}

