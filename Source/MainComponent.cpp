/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
    : groupSerialSetup(String::empty, "Serial Port Setup"),
      groupMode(String::empty, "Mode"),
      labelSerialDevice(String::empty, "Device:"),
      labelSerialBaud(String::empty, "Baud Rate:"),
      buttonStopGo("Go!")
{
    uint32_t y = 10;

    setSize (xSize, ySize);

    // Serial Port Setup
    groupSerialSetup.setBounds(10,y,xSize-20,100);
    addAndMakeVisible(groupSerialSetup);

    y += 20;

    labelSerialDevice.setBounds(21, y, 100, 20);
    addAndMakeVisible(labelSerialDevice);

    comboSerialDevice.setBounds(100, y, 200, 20);
    addAndMakeVisible(comboSerialDevice);
    comboSerialDevice.startThread();

    // Stop-go button
    buttonStopGo.setBounds(310, y, 60, 40);
    addAndMakeVisible(buttonStopGo);
    buttonStopGo.setClickingTogglesState(true);
    buttonStopGo.addListener(this);

    y += 20;

    labelSerialBaud.setBounds(21, y, 100, 20);
    addAndMakeVisible(labelSerialBaud);

    comboSerialBaud.setBounds(100, y, 200, 20);
    addAndMakeVisible(comboSerialBaud);
    comboSerialBaud.addItem("2400", 2400); 	
    comboSerialBaud.addItem("4800", 4800); 	
    comboSerialBaud.addItem("9600", 9600); 	
    comboSerialBaud.addItem("19200", 19200);	
    comboSerialBaud.addItem("38400", 38400); 	
    comboSerialBaud.addItem("57600", 57600); 	
    comboSerialBaud.addItem("115200", 115200);
    comboSerialBaud.setSelectedId(115200);

    y += 60;

    // Serial Port Setup
    groupMode.setBounds(10,y,xSize-20,40);
    addAndMakeVisible(groupMode);

//    
//    textSerialDevice.setBounds (100, y, 200, 20);
//    addAndMakeVisible(textSerialDevice);
//
//    y += 25;
//
//    // OSC Host Setting
//    labelOSCHost.setBounds(10, y, 100, 20);
//    addAndMakeVisible(labelOSCHost);
//    
//    textOSCHost.setBounds(100, y, 200, 20);
//    addAndMakeVisible(textOSCHost);
//
//    y += 25;
//
//    // OSC Post Setting
//    labelOSCPort.setBounds(10, y, 100, 20);
//    addAndMakeVisible(labelOSCPort);
//    
//    textOSCPort.setBounds(100, y, 200, 20);
//    addAndMakeVisible(textOSCPort);
//
//    y += 25;
//
//    // Serial Port Setup
//    groupMode.setBounds(10,y,xSize-10,100);
//    addAndMakeVisible(groupMode);
//


}

MainContentComponent::~MainContentComponent()
{
    comboSerialDevice.stopThread(1000);
}

void MainContentComponent::handleMessage(const Message &message)
{
    
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::buttonClicked(Button *button) 
{
    if (button == &buttonStopGo)
    {
	if (buttonStopGo.getToggleState()) 
	{
	    if (0 == serialPortReader.openSerialDevice(comboSerialDevice.getText(), comboSerialBaud.getSelectedId())) 
	    {
		buttonStopGo.setButtonText("Stop!");
		labelSerialDevice.setEnabled(false);
		comboSerialDevice.setEnabled(false);
		labelSerialBaud.setEnabled(false);
		comboSerialBaud.setEnabled(false);
		serialPortReader.startThread();
	    }
	    else
	    {
		buttonStopGo.setToggleState(false, dontSendNotification);
	    }
	} 
	else 
	{
	    buttonStopGo.setButtonText("Go!");
	    labelSerialDevice.setEnabled(true);
	    comboSerialDevice.setEnabled(true);
	    labelSerialBaud.setEnabled(true);
	    comboSerialBaud.setEnabled(true);
	    serialPortReader.stopThread(1000);
	}
    }
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
