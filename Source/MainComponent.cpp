/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"
#include "KeyCalibrationDialog.h"


//==============================================================================
MainContentComponent::MainContentComponent()
    : groupSerialSetup(String::empty, "Serial Port Setup"),
      groupMode(String::empty, "Mode"),
      labelSerialDevice(String::empty, "Device:"),
      labelSerialBaud(String::empty, "Baud Rate:"),
      buttonStopGo("Go!"),
      labelOSCHost(String::empty, "OSC Host:"),
      labelOSCPort(String::empty, "Port:"),
      keyboardMonitor(32)
{
    uint32_t y = 10;

    setSize (xSize, ySize);

    // Serial Port Setup
    groupSerialSetup.setBounds(10,y,xSize-20,75);
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

    y += 45;

    // Serial Port Setup
    groupMode.setBounds(10,y,xSize-20,55);
    addAndMakeVisible(groupMode);

    y += 20;

    // OSC Host Setting
    labelOSCHost.setBounds(21, y, 100, 20);
    addAndMakeVisible(labelOSCHost);

    textOSCHost.setBounds(100, y, 150, 20);
    addAndMakeVisible(textOSCHost);

    // OSC Port Setting
    labelOSCPort.setBounds(268, y, 100, 20);
    addAndMakeVisible(labelOSCPort);
    
    textOSCPort.setBounds(310, y, 60, 20);
    addAndMakeVisible(textOSCPort);

    y += 45;

    keyboardMonitor.setBounds(10, y, xSize-20, 70);
    keyboardMonitor.setKeyWidth(20);
    keyboardMonitor.addActionListener(this);
    addAndMakeVisible(keyboardMonitor);
    
    serialPortReader.addActionListener(this);

    // Initialize calibration table
    for (uint32_t i = 0; i < 32; i++) 
    {
	keyMax[i] = 1024;
	keyMin[i] = 0;
    }
}

MainContentComponent::~MainContentComponent()
{
    comboSerialDevice.stopThread(1000);
    serialPortReader.stop();
}

void MainContentComponent::actionListenerCallback(const String &message)
{
    StringArray tokens = StringArray::fromTokens(message, true);

    if (tokens[0] == "SerialPortDied") // Serial Port Error
    {
	buttonStopGo.setButtonText("Go!");
	labelSerialDevice.setEnabled(true);
	comboSerialDevice.setEnabled(true);
	labelSerialBaud.setEnabled(true);
	comboSerialBaud.setEnabled(true);
	serialPortReader.stop();
	keyboardMonitor.clearKeys();
	// TODO: Pop up a dialog saying the port died.
    }
    else if (tokens[0] == "OUT_OF_FRAME!") // Lost frame sync.
    {
	std::cout << "Lost Frame Sync!" << std::endl;
    }
    else if (tokens[0] == "AK") // Key message
    {
	String oscEndpoint = String("/retrofoot/") + tokens[2];
	float calibratedValue = getCalibratedValue(tokens[2].getIntValue(), tokens[3].getIntValue());

	lo_send(oscAddress, oscEndpoint.toRawUTF8(), "f", calibratedValue);  
	keyboardMonitor.setKeyPosition(tokens[2].getIntValue(), calibratedValue);
    }
    else if (tokens[0] == "CK") // Calibrate key
    {
	KeyCalibrationDialog kcd(32, keyMax, keyMin);

	DialogWindow::showModalDialog("Calibrate Keys", &kcd, this, Colour(0,0,0), true);
    }
    else
    {
	std::cout << "Received unknown message: " << message << std::endl;
    }
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
	    oscAddress = lo_address_new(textOSCHost.getText().toRawUTF8(), textOSCPort.getText().toRawUTF8());

	    if (0 == serialPortReader.start(comboSerialDevice.getText(), comboSerialBaud.getSelectedId())) 
	    {
		buttonStopGo.setButtonText("Stop!");
		labelSerialDevice.setEnabled(false);
		comboSerialDevice.setEnabled(false);
		labelSerialBaud.setEnabled(false);
		comboSerialBaud.setEnabled(false);
	    }
	    else
	    {
		// TODO: Pop up an error message.
		buttonStopGo.setToggleState(false, dontSendNotification);
		lo_address_free(oscAddress);
	    }
	} 
	else 
	{
	    buttonStopGo.setButtonText("Go!");
	    labelSerialDevice.setEnabled(true);
	    comboSerialDevice.setEnabled(true);
	    labelSerialBaud.setEnabled(true);
	    comboSerialBaud.setEnabled(true);
	    serialPortReader.stop();
	}
    }
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

float MainContentComponent::getCalibratedValue(uint32_t keyIdx, uint32_t keyValue)
{
    if (keyValue < keyMin[keyIdx])
    {
	return 0.0;
    }

    if (keyValue > keyMax[keyIdx])
    {
	return 1.0;
    }

    return ((float)(keyValue - keyMin[keyIdx]) / (float)(keyMax[keyIdx] - keyMin[keyIdx]));
}
