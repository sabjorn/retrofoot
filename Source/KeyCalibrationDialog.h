/*
  ==============================================================================

    KeyCalibrationDialog.h
    Created: 19 Apr 2015 2:31:43pm
    Author:  msteve

  ==============================================================================
*/

#ifndef KEYCALIBRATIONDIALOG_H_INCLUDED
#define KEYCALIBRATIONDIALOG_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class KeyCalibrationDialog    : public Component,
                                public Slider::Listener,
                                public Button::Listener
    
{
public:
    KeyCalibrationDialog(uint32_t numKeys, uint32_t *minValues, uint32_t *maxValues);
    ~KeyCalibrationDialog();

    void paint (Graphics&);
    void resized();

    void sliderValueChanged (Slider *slider);
    void buttonClicked(Button *button);

    void setKeyValue(int key, int value);

private:

    Slider **sliderCalib;
    Button **buttonCalib;
    TextButton buttonCalibAll;
    uint32_t nKeys;
    uint32_t *pMinValues;
    uint32_t *pMaxValues;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (KeyCalibrationDialog)
};


#endif  // KEYCALIBRATIONDIALOG_H_INCLUDED
