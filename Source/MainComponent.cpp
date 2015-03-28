/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"


//==============================================================================
MainContentComponent::MainContentComponent()
    : labelSerialDevice(String::empty, "Serial Device:"),
      labelOSCHost(String::empty, "OSC Host:"),
      labelOSCPort(String::empty, "OSC Port:"),
      buttonStopGo("Go!")
{
    uint32_t y = 10;

    setSize (400, 100);

    // Serial Device Setting
    labelSerialDevice.setBounds(10, y, 100, 20);
    addAndMakeVisible(labelSerialDevice);
    
    textSerialDevice.setBounds (100, y, 200, 20);
    addAndMakeVisible(textSerialDevice);

    y += 20;

    // OSC Host Setting
    labelOSCHost.setBounds(10, y, 100, 20);
    addAndMakeVisible(labelOSCHost);
    
    textOSCHost.setBounds(100, y, 200, 20);
    addAndMakeVisible(textOSCHost);

    y += 20;

    // OSC Post Setting
    labelOSCPort.setBounds(10, y, 100, 20);
    addAndMakeVisible(labelOSCPort);
    
    textOSCPort.setBounds(100, y, 200, 20);
    addAndMakeVisible(textOSCPort);

    y += 20;

    // Stop-go button
    buttonStopGo.setBounds(10, y, 50, 20);
    addAndMakeVisible(buttonStopGo);
    buttonStopGo.addListener(this);

}

MainContentComponent::~MainContentComponent()
{
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::buttonClicked(Button *button) 
{
    if (buttonStopGo.getButtonText() == "Go!") {
	buttonStopGo.setButtonText("Stop!");
	labelSerialDevice.setEnabled(false);
	labelOSCHost.setEnabled(false);
	labelOSCPort.setEnabled(false);
	textSerialDevice.setEnabled(false);
	textOSCHost.setEnabled(false);
	textOSCPort.setEnabled(false);
	serialPortReader.startThread();
    } else {
	buttonStopGo.setButtonText("Go!");
	labelSerialDevice.setEnabled(true);
	labelOSCHost.setEnabled(true);
	labelOSCPort.setEnabled(true);
	textSerialDevice.setEnabled(true);
	textOSCHost.setEnabled(true);
	textOSCPort.setEnabled(true);
	serialPortReader.stopThread(10000000);
    }
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}
