/*
  ==============================================================================

    KeyCalibrationDialog.cpp
    Created: 19 Apr 2015 2:31:43pm
    Author:  msteve

  ==============================================================================
*/

#include "../JuceLibraryCode/JuceHeader.h"
#include "KeyCalibrationDialog.h"

ApplicationProperties& getAppProperties();

//==============================================================================
KeyCalibrationDialog::KeyCalibrationDialog(uint32_t numKeys, uint32_t *minValues, uint32_t *maxValues)
    : nKeys(numKeys),
      pMinValues(minValues),
      pMaxValues(maxValues)
{
    const char *notes[12] = { "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B" }; 

    pCurValues = new uint32_t[numKeys];
    
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.
    setSize((numKeys+1)*20,350);

    sliderCalib = new Slider*[numKeys];
    buttonCalib = new Button*[numKeys];
    
    for (uint32_t i = 0; i < numKeys; i++)
    {
	sliderCalib[i] = new Slider();
	sliderCalib[i]->setSliderStyle(Slider::ThreeValueVertical);
	sliderCalib[i]->setBounds(20*i+10, 10, 20, 270);
	sliderCalib[i]->setInterceptsMouseClicks(false, false);
	addAndMakeVisible(sliderCalib[i]);

	if (NULL != minValues && NULL != maxValues)
	{
	    sliderCalib[i]->setRange(0,1024);
	    sliderCalib[i]->setMinAndMaxValues(minValues[i], maxValues[i], dontSendNotification);
	}

	sliderCalib[i]->addListener(this);

	buttonCalib[i] = new TextButton(notes[i%12]);
	buttonCalib[i]->setBounds(20*i+10, 290, 20, 20);
	buttonCalib[i]->setClickingTogglesState(true);

	if (i == 0)
	{
	    buttonCalib[i]->setConnectedEdges(Button::ConnectedOnRight);
	}
	else if (i == 31)
	{
	    buttonCalib[i]->setConnectedEdges(Button::ConnectedOnLeft);
	}
	else
	{
	    buttonCalib[i]->setConnectedEdges(Button::ConnectedOnLeft | Button::ConnectedOnRight);
	}

	buttonCalib[i]->addListener(this);
	addAndMakeVisible(buttonCalib[i]);
    }

    buttonCalibAll.setButtonText("Calibrate All");
    buttonCalibAll.setBounds((20*(nKeys+1))/2-50,320,100,20);
    buttonCalibAll.setClickingTogglesState(true);
    buttonCalibAll.addListener(this);
    addAndMakeVisible(buttonCalibAll);

    startTimer(100);
    
}

KeyCalibrationDialog::~KeyCalibrationDialog()
{
    stopTimer();

    for (uint32_t i = 0; i < nKeys; i++)
    {
	delete sliderCalib[i];
	delete buttonCalib[i];
    }

    delete [] sliderCalib;
    delete [] buttonCalib;
    delete [] pCurValues;


}

void KeyCalibrationDialog::setKeyValue(int key, int value)
{
    if (buttonCalib[key]->getToggleState())
    {
	if (value < pMinValues[key])
	{
	    pMinValues[key] = value;
	    getAppProperties().getUserSettings()->setValue(String("min") + String(key), String(pMinValues[key]));
	    if (pMaxValues[key] == value)
	    {
		pMaxValues[key] = value + 1;
		getAppProperties().getUserSettings()->setValue(String("max") + String(key), String(pMaxValues[key]));
	    }
	}

	if (value > pMaxValues[key])
	{
	    pMaxValues[key] = value;
	    getAppProperties().getUserSettings()->setValue(String("max") + String(key), String(pMaxValues[key]));
	    if (pMinValues[key] == value)
	    {
		pMinValues[key] = value - 1;	
		getAppProperties().getUserSettings()->setValue(String("min") + String(key), String(pMinValues[key]));
	    }
	}

        getAppProperties().getUserSettings()->saveIfNeeded();
    }

    pCurValues[key] = value;
    //sliderCalib[key]->setValue(value);
    
}

void KeyCalibrationDialog::timerCallback()
{
    if (this->isShowing())
    {
    for (uint32 key = 0; key < nKeys; key++)
    {
        sliderCalib[key]->setMinValue(pMinValues[key], dontSendNotification);
	    sliderCalib[key]->setMaxValue(pMaxValues[key], dontSendNotification);
        sliderCalib[key]->setValue(pCurValues[key]);
    }
    }
}


void KeyCalibrationDialog::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (Colours::lightgrey);   // clear the background


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
