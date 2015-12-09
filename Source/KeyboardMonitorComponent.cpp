#include "KeyboardMonitorComponent.h"
#include <iostream>


KeyboardMonitorComponent::KeyboardMonitorComponent(uint32_t numKeys) 
    : MidiKeyboardComponent(keyboardState, MidiKeyboardComponent::horizontalKeyboard)
{
    setColour(mouseOverKeyOverlayColourId, Colour());
    setColour(blackNoteColourId, Colours::grey);
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

	g.setColour(Colours::red);
	g.drawLine(x+w/2-2,y+h*noteOffThresh,x+w/2+2,y+h*noteOffThresh);

	g.setColour(Colours::green);
	g.drawLine(x+w/2-2,y+h*noteOnThresh,x+w/2+2,y+h*noteOnThresh);

	g.setColour(Colours::purple);
	g.drawLine(x+w/2-2,y+h*aftertouchThresh,x+w/2+2,y+h*aftertouchThresh);
}

void KeyboardMonitorComponent::drawBlackNote(int midiNoteNumber, Graphics &g, int x, int y, int w, int h, bool isDown, bool isOver, const Colour &noteFillColour)
{
    MidiKeyboardComponent::drawBlackNote(midiNoteNumber, g, x, y, w, h, isDown, isOver, noteFillColour);
    g.setColour(Colour(0,0,128));
    g.setOpacity(0.5);
    g.fillRect(x+1,y,w-2,int(keyGauge[midiNoteNumber]*(h-2)));

	g.setColour(Colours::red);
	g.drawLine(x+w/2-2,y+h*noteOffThresh,x+w/2+2,y+h*noteOffThresh);

	g.setColour(Colours::green);
	g.drawLine(x+w/2-2,y+h*noteOnThresh,x+w/2+2,y+h*noteOnThresh);

	g.setColour(Colours::purple);
	g.drawLine(x+w/2-2,y+h*aftertouchThresh,x+w/2+2,y+h*aftertouchThresh);
}

void KeyboardMonitorComponent::setKeyPosition(int midiNoteNumber, float keyPosition)
{
    keyGauge[midiNoteNumber] = keyPosition;
    repaint();
}

void KeyboardMonitorComponent::setNoteOn(float thresh)
{
		noteOnThresh = thresh;
}

void KeyboardMonitorComponent::setNoteOff(float thresh)
{
		noteOffThresh = thresh;
}

void KeyboardMonitorComponent::setAftertouch(float thresh)
{
		aftertouchThresh = thresh;
}


void KeyboardMonitorComponent::clearKeys()
{
    for (uint32_t i = getRangeStart(); i <= getRangeEnd(); i++)
    {
	setKeyPosition(i, 0.0);
    }
}

bool KeyboardMonitorComponent::mouseDownOnKey(int midiNoteNumber, const MouseEvent &e)
{
    sendActionMessage("CK " + String(midiNoteNumber));

    return false;
}

KeyboardMonitorComponent::~KeyboardMonitorComponent()
{
    delete [] keyGauge;
}


