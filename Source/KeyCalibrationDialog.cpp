/*
  ==============================================================================

    KeyCalibrationDialog.cpp
    Created: 19 Apr 2015 2:31:43pm
    Author:  msteve

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "KeyCalibrationDialog.h"

//==============================================================================
KeyCalibrationDialog::KeyCalibrationDialog(uint32_t numKeys, uint32_t *minValues, uint32_t *maxValues)
    : nKeys(numKeys),
      pMinValues(minValues),
      pMaxValues(maxValues)
{
    const char *notes[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }; 

    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize(numKeys*30,450);

    sliderCalib = new Slider*[numKeys];
    buttonCalib = new Button*[numKeys];
    
    for (uint32_t i = 0; i < numKeys; i++)
    {
	sliderCalib[i] = new Slider();
	sliderCalib[i]->setSliderStyle(Slider::ThreeValueVertical);
	sliderCalib[i]->setBounds(30*i, 0, 30,400);
	sliderCalib[i]->setInterceptsMouseClicks(false, false);
	addAndMakeVisible(sliderCalib[i]);

	if (NULL != minValues && NULL != maxValues)
	{
	    sliderCalib[i]->setRange(0,1024);
	    sliderCalib[i]->setMinAndMaxValues(minValues[i], maxValues[i], dontSendNotification);
	}

	sliderCalib[i]->addListener(this);

	buttonCalib[i] = new TextButton(notes[i%12]);
	buttonCalib[i]->setBounds(30*i, 400, 25, 20);
	buttonCalib[i]->setClickingTogglesState(true);
	buttonCalib[i]->addListener(this);
	addAndMakeVisible(buttonCalib[i]);
    }

    buttonCalibAll.setButtonText("Calibrate All");
    buttonCalibAll.setBounds(30*nKeys/2-50,425,100,20);
    buttonCalibAll.setClickingTogglesState(true);
    buttonCalibAll.addListener(this);
    addAndMakeVisible(buttonCalibAll);

}

KeyCalibrationDialog::~KeyCalibrationDialog()
{
    for (uint32_t i = 0; i < nKeys; i++)
    {
	delete sliderCalib[i];
	delete buttonCalib[i];
    }

    delete [] sliderCalib;
    delete [] buttonCalib;

}

void KeyCalibrationDialog::setKeyValue(int key, int value)
{
    if (buttonCalib[key]->getToggleState())
    {
	if (value < pMinValues[key])
	{
	    sliderCalib[key]->setMinValue(value, dontSendNotification);
	    pMinValues[key] = value;
	    if (pMaxValues[key] == value)
	    {
		pMaxValues[key] = value + 1;
	    }
	}

	if (value > pMaxValues[key])
	{
	    sliderCalib[key]->setMaxValue(value, dontSendNotification);
	    pMaxValues[key] = value;
	    if (pMinValues[key] == value)
	    {
		pMinValues[key] = value - 1;
	    }
	}

    }

    sliderCalib[key]->setValue(value);

}

void KeyCalibrationDialog::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::white);   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::lightblue);
    g.setFont (14.0f);
    g.drawText ("KeyCalibrationDialog", getLocalBounds(),
                Justification::centred, true);   // draw some placeholder text
}

void KeyCalibrationDialog::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

void KeyCalibrationDialog::sliderValueChanged (Slider *slider)
{
/*    if (NULL != pMaxValues && NULL != pMinValues)
    {
	for (uint32_t i = 0; i < nKeys; i++)
	{
	    pMaxValues[i] = sliderCalib[i]->getMaxValue();
	    pMinValues[i] = sliderCalib[i]->getMinValue();
	}
	}*/
}

void KeyCalibrationDialog::buttonClicked (Button *button)
{
    if (button == &buttonCalibAll)
    {
	for (uint32_t i = 0; i < nKeys; i++)
	{
	    buttonCalib[i]->setToggleState(buttonCalibAll.getToggleState(), sendNotification);
	}
    } 
    else
    {
	for (uint32_t i = 0; i < nKeys; i++)
	{
	    if (button == buttonCalib[i] && buttonCalib[i]->getToggleState())
	    {
		pMaxValues[i] = 0;
		pMinValues[i] = 1024;
                sliderCalib[i]->setMinAndMaxValues(512, 512, dontSendNotification);
	    }
	}
    }
}
