#include "MainComponent.h"
#include "KeyCalibrationDialog.h"

ApplicationProperties& getAppProperties();

//==============================================================================
MainContentComponent::MainContentComponent()
    : groupSerialSetup(String::empty, "Serial Port Setup"),
      groupOSC(String::empty, "Open Sound Control"),
      groupMidi(String::empty, "MIDI"),
      labelSerialDevice(String::empty, "Device:"),
      labelSerialStatus(String::empty, "Status:"),
      labelSerialIndicator(String::empty, ""),
      labelOSCHost(String::empty, "Host:"),
      labelOSCPort(String::empty, "Port:"),
      labelMidiDevice(String::empty, "Device:"),
      labelMidiCh(String::empty, "Ch:"),
      labelMidiOctave(String::empty, "Octave:"),
      labelMidiProgram(String::empty, "Prog:"),
      labelMidiVelocity(String::empty, "Vel:"),
      labelMidiAftertouch(String::empty, "Aftertouch:"),
      sliderNoteOnThresh(Slider::TwoValueHorizontal, Slider::NoTextBox), 
      sliderAftertouchThresh(Slider::TwoValueHorizontal, Slider::NoTextBox), 
      sliderAftertouchDepth(Slider::LinearBar, Slider::NoTextBox), 
      keyboardMonitor(numKeys),
      isSynced(false),
      midiOutput(NULL)
{
    uint32 y = 10;

    setSize (xSize, ySize);

    //==========================================================================
    // Serial Port Setup
    groupSerialSetup.setBounds(10,y,xSize-20,55);
    addAndMakeVisible(groupSerialSetup);
    y += 20;

    // Serial On/Off button
    enableSerial.setBounds(21, y, 20, 20);
    addAndMakeVisible(enableSerial);
    enableSerial.addListener(this);

    // "Device:" label
    labelSerialDevice.setBounds(41, y, 100, 20);
    addAndMakeVisible(labelSerialDevice);

    // Serial Device Chooser
    comboSerialDevice.setBounds(100, y, 150, 20);
    addAndMakeVisible(comboSerialDevice);
    comboSerialDevice.startThread();
	
    // "Status:" label
    labelSerialStatus.setBounds(268, y, 100, 20);
    addAndMakeVisible(labelSerialStatus);
	
    // Status Indicator
    labelSerialIndicator.setBounds(310, y, 60, 20);
    addAndMakeVisible(labelSerialIndicator);
		
    y += 40;

    //==========================================================================
    // OSC Setup
    groupOSC.setBounds(10,y,xSize-20,55);
    addAndMakeVisible(groupOSC);
    y += 20;

    // OSC enable
    enableOSC.setBounds(21, y, 20, 20);
    addAndMakeVisible(enableOSC);
    enableOSC.addListener(this);
	
    // "OSC Host:" label
    labelOSCHost.setBounds(41, y, 100, 20);
    addAndMakeVisible(labelOSCHost);

    // OSC Host Text entry
    textOSCHost.setBounds(100, y, 150, 20);
    textOSCHost.setText(getAppProperties().getUserSettings()->getValue("oschost", "localhost"));
    textOSCHost.addListener(this);
    addAndMakeVisible(textOSCHost);

    // "OSC Port:" label
    labelOSCPort.setBounds(268, y, 100, 20);
    addAndMakeVisible(labelOSCPort);

    // OSC Port Text entry
    textOSCPort.setBounds(310, y, 60, 20);
    textOSCPort.setText(getAppProperties().getUserSettings()->getValue("oscport", "2112"));
    textOSCPort.addListener(this);
    addAndMakeVisible(textOSCPort);
    y += 40;

    //==========================================================================
    // MIDI Setup
    groupMidi.setBounds(10,y,xSize-20,165);
    addAndMakeVisible(groupMidi);
    y += 20;

    // MIDI enable
    enableMidi.setBounds(21, y, 20, 20);
    addAndMakeVisible(enableMidi);
    enableMidi.addListener(this);
    
    // "MIDI Device:" label
    labelMidiDevice.setBounds(41, y, 100, 20);
    addAndMakeVisible(labelMidiDevice);
    
    // Midi Device Chooser
    deviceMidi.setBounds(100, y, 150, 20);
    deviceMidi.startThread();
    addAndMakeVisible(deviceMidi);
    
    // "Midi Channel:" label
    labelMidiCh.setBounds(268, y, 100, 20);
    addAndMakeVisible(labelMidiCh);
    
    // Midi channel chooser
    channelMidi.setBounds(310, y, 60, 20);
    for (uint32 i = 0; i < 16; i++)
    {
	channelMidi.addItem(String(i+1), i+1);
    }
    addAndMakeVisible(channelMidi);
    channelMidi.setSelectedId(getAppProperties().getUserSettings()->getIntValue("midich", 1));
    channelMidi.addListener(this);
    y += 25;
    
    // "Midi Program:" label
    labelMidiProgram.setBounds(41, y, 100, 20);
    addAndMakeVisible(labelMidiProgram);
    
    // Midi Program Chooser
    programMidi.setBounds(100, y, 50, 20);
    for (uint32 i = 0; i < 128; i++) // Midi Programs!
    {
	programMidi.addItem(String(i+1), i+1);
    }
    addAndMakeVisible(programMidi);
    programMidi.setSelectedId(getAppProperties().getUserSettings()->getIntValue("midiprog", 1));
    programMidi.addListener(this);
    
    // "Midi Octave:" label
    labelMidiOctave.setBounds(160, y, 50, 20);
    addAndMakeVisible(labelMidiOctave);
    
    // Midi Octave Chooser
    octaveMidi.setBounds(210, y, 40, 20);
    for (uint32 i = 0; i < 9; i++) // Octaves 0 to 8. Octave 8 goes up to note 127 with the high G on the keyboard.
    {
	octaveMidi.addItem(String(i), i+1);
    }
    addAndMakeVisible(octaveMidi);
    octaveMidi.setSelectedId(getAppProperties().getUserSettings()->getIntValue("midioct", 3));
    octaveMidi.addListener(this);
    
    // "Midi Velocity:" label
    labelMidiVelocity.setBounds(268, y, 100, 20);
    addAndMakeVisible(labelMidiVelocity);
    
    // Midi Velocity Chooser
    velocityMidi.setBounds(310, y, 60, 20);
    velocityMidi.addItem("Auto", 128);
    velocityMidi.addItem("64", 64);
    velocityMidi.addItem("100", 100);
    velocityMidi.addItem("127", 127);
    
    addAndMakeVisible(velocityMidi);
    velocityMidi.setSelectedId(getAppProperties().getUserSettings()->getIntValue("midivel", 128));
    velocityMidi.addListener(this);
    y += 30;
    
    // "Midi Aftertouch:" label
    labelMidiAftertouch.setBounds(41, y, 100, 20);
    addAndMakeVisible(labelMidiAftertouch);
    
    // Midi Aftertouch Chooser
    aftertouchMidi.setBounds(100, y, 100, 20);
    aftertouchMidi.addItem("Off", ID_AFTERTOUCH_OFF);
    aftertouchMidi.addItem("Mono", ID_AFTERTOUCH_MONO);
    aftertouchMidi.addItem("Poly", ID_AFTERTOUCH_POLY);
    aftertouchMidi.setSelectedId(getAppProperties().getUserSettings()->getIntValue("midiaft", ID_AFTERTOUCH_OFF));
    aftertouchMidi.addListener(this);
    addAndMakeVisible(aftertouchMidi);

    // Aftertouch Depth
    sliderAftertouchDepth.setBounds(268, y, 100, 20);
    sliderAftertouchDepth.setRange(0.0, 1.0);
    sliderAftertouchDepth.setValue(getAppProperties().getUserSettings()->getDoubleValue("aft_depth", 0.5));
    sliderAftertouchDepth.addListener(this);
    addAndMakeVisible(sliderAftertouchDepth);
    
    y += 30;

    // Note Off / Note On Threshold Slider
    sliderNoteOnThresh.setBounds(41, y, 300, 20);
    sliderNoteOnThresh.setRange(0.0, 1.0);
    sliderNoteOnThresh.addListener(this);
    sliderNoteOnThresh.setMaxValue(getAppProperties().getUserSettings()->getDoubleValue("thr_on", 0.55));
    sliderNoteOnThresh.setMinValue(getAppProperties().getUserSettings()->getDoubleValue("thr_off", 0.45));
    addAndMakeVisible(sliderNoteOnThresh);

    y += 30;
    sliderAftertouchThresh.setBounds(41, y, 300, 20);
    sliderAftertouchThresh.setRange(0.0, 1.0);
    sliderAftertouchThresh.addListener(this);
    sliderAftertouchThresh.setMaxValue(getAppProperties().getUserSettings()->getDoubleValue("thr_aft", 0.80));
    sliderAftertouchThresh.setMinValue(getAppProperties().getUserSettings()->getDoubleValue("thr_off", 0.45));
    addAndMakeVisible(sliderAftertouchThresh);


    y += 47;
    
    //==========================================================================
    // Keyboard Monitor and calibration.
    keyboardMonitor.setBounds(10, y, xSize-20, 70);
    keyboardMonitor.setKeyWidth(20);
    keyboardMonitor.addActionListener(this);
    addAndMakeVisible(keyboardMonitor);
    serialPortReader.addActionListener(this);

    // Initialize calibration table
    for (uint32 i = 0; i < 32; i++) 
    {
	keyMax[i] = getAppProperties().getUserSettings()->getIntValue(String("max") + String(i), 1024);
	keyMin[i] = getAppProperties().getUserSettings()->getIntValue(String("min") + String(i), 0);
	midiIsNoteOn[i] = false;
    }

    kcd = new KeyCalibrationDialog(numKeys, keyMin, keyMax);

    //==========================================================================
    // Set the GUI in its initial state
    updateGui();

}

MainContentComponent::~MainContentComponent()
{
    comboSerialDevice.stopThread(1000);
    deviceMidi.stopThread(1000);
    serialPortReader.stop();
    if (NULL != midiOutput)
    {
	midiOutput->sendMessageNow(MidiMessage::allNotesOff(midiChannel()));
	delete midiOutput;
	midiOutput = NULL;
    }
    delete kcd;
}

// Main GUI enable/disable logic
void MainContentComponent::updateGui()
{
    if (isSerialEnabled())
    {
	// Cannot change serial device while serial is enabled
	comboSerialDevice.setEnabled(false);
	labelSerialDevice.setEnabled(false);

	enableOSC.setEnabled(true);

	if (isOscEnabled())
    	{
	    // Have to disable OSC to change host/port
	    labelOSCHost.setEnabled(false);
	    textOSCHost.setEnabled(false);
	    labelOSCPort.setEnabled(false);
	    textOSCPort.setEnabled(false);
	}
	else
	{
	    labelOSCHost.setEnabled(true);
	    textOSCHost.setEnabled(true);
	    labelOSCPort.setEnabled(true);
	    textOSCPort.setEnabled(true);
	}

	enableMidi.setEnabled(true);

	if (isMidiEnabled())
	{
	    // Have to disable MIDI to change device
	    labelMidiDevice.setEnabled(false);
	    deviceMidi.setEnabled(false);
	    labelMidiCh.setEnabled(false);
	    channelMidi.setEnabled(false);
	    labelMidiOctave.setEnabled(false);
	    octaveMidi.setEnabled(false);
	    labelMidiProgram.setEnabled(false);
	    programMidi.setEnabled(false);
	    labelMidiVelocity.setEnabled(false);
	    velocityMidi.setEnabled(false);
	}
	else
	{
	    labelMidiDevice.setEnabled(true);
	    deviceMidi.setEnabled(true);
	    labelMidiCh.setEnabled(true);
	    channelMidi.setEnabled(true);
	    labelMidiOctave.setEnabled(true);
	    octaveMidi.setEnabled(true);
	    labelMidiProgram.setEnabled(true);
	    programMidi.setEnabled(true);
	    labelMidiVelocity.setEnabled(true);
	    velocityMidi.setEnabled(true);
	}
    }
    else // Serial is not enabled
    {
	comboSerialDevice.setEnabled(true);
	labelSerialDevice.setEnabled(true);

	isSynced = false;
	labelSerialIndicator.setText("IDLE", dontSendNotification);
	labelSerialIndicator.setColour(Label::textColourId, Colours::red);

	enableOSC.setEnabled(false);
	enableOSC.setToggleState(false, sendNotification);
	labelOSCHost.setEnabled(true);
	textOSCHost.setEnabled(true);
	labelOSCPort.setEnabled(true);
	textOSCPort.setEnabled(true);

	enableMidi.setEnabled(false);
	enableMidi.setToggleState(false, sendNotification);
	labelMidiDevice.setEnabled(true);
	deviceMidi.setEnabled(true);
	labelMidiCh.setEnabled(true);
	channelMidi.setEnabled(true);
	labelMidiOctave.setEnabled(true);
	octaveMidi.setEnabled(true);
	labelMidiProgram.setEnabled(true);
	programMidi.setEnabled(true);
	labelMidiVelocity.setEnabled(true);
	velocityMidi.setEnabled(true);
    }
}

void MainContentComponent::actionListenerCallback(const String &message)
{
    StringArray tokens = StringArray::fromTokens(message, true);

    if (tokens[0] == "SerialPortDied") // Serial Port Error
    {
	// Disable serial. Will update GUI.
	enableSerial.setToggleState(false, sendNotification);
    }
    else if (tokens[0] == "SM") // State Machine Messages
    {
	labelSerialIndicator.setText(tokens[1], dontSendNotification);

	if (tokens[1] == "PRESYNC")
	{
	    labelSerialIndicator.setColour(Label::textColourId, Colours::yellow);
	}
	else if (tokens[1] == "OOF")
	{
	    labelSerialIndicator.setColour(Label::textColourId, Colours::red);
	    enableSerial.setToggleState(false, sendNotification);
	}
	else if (tokens[1] == "SYNC")
	{
	    labelSerialIndicator.setColour(Label::textColourId, Colours::green);
	    isSynced = true;
	    updateGui();
	}

    }
    else if (tokens[0] == "AK") // Key message
    {
	uint32 keyIdx = tokens[2].getIntValue();
	uint32 adcVal = tokens[3].getIntValue();
	float calibratedValue = getCalibratedValue(keyIdx, adcVal);
		
	keyboardMonitor.setKeyPosition(keyIdx, calibratedValue);
	kcd->setKeyValue(keyIdx, adcVal);

	if (isOscEnabled())
	{
	    OSCMessage msg("/retrofoot/" + String(keyIdx), calibratedValue);
	    oscSender.send(msg);
	}

	if (isMidiEnabled())
	{
	    if (false == midiIsNoteOn[keyIdx] && calibratedValue > noteOnThresh())
	    {

		midiIsNoteOn[keyIdx] = true;
		midiOutput->sendMessageNow(MidiMessage::noteOn(midiChannel(), midiOffset()+keyIdx, midiVelocity(keyIdx, calibratedValue)));
	    }
			
	    if (true == midiIsNoteOn[keyIdx] && calibratedValue < noteOffThresh())
	    {
		midiIsNoteOn[keyIdx] = false;
		midiOutput->sendMessageNow(MidiMessage::noteOff(midiChannel(), midiOffset()+keyIdx, (uint8)127));
	    }

	    // Aftertouch
	    if ((true == midiIsNoteOn[keyIdx]) && (calibratedValue > aftertouchThresh()))
	    {
		uint8 aftertouchVal = (uint8)(((calibratedValue-aftertouchThresh())/(1.0-aftertouchThresh()))*aftertouchDepth()*127.0);

		if (aftertouchMidi.getSelectedId() == ID_AFTERTOUCH_MONO)
		{
		    midiOutput->sendMessageNow(MidiMessage::channelPressureChange(midiChannel(), aftertouchVal));
		}
		
		if (aftertouchMidi.getSelectedId() == ID_AFTERTOUCH_POLY)
		{
		    midiOutput->sendMessageNow(MidiMessage::aftertouchChange(midiChannel(), midiOffset()+keyIdx, aftertouchVal));
		}		
	    }

	    // Update velocity table.
	    midiPrevValue[keyIdx] = calibratedValue;
	}
    }
    else if (tokens[0] == "CK") // Calibrate key
    {
	DialogWindow::showModalDialog("Calibrate Keys", kcd, this, Colour(0,0,0), true);
    }
    else
    {
	std::cout << "Received unknown message: " << message << std::endl;
    }
}

uint8 MainContentComponent::midiProgram()
{
    return (uint8)(programMidi.getSelectedId()-1);
}

uint8 MainContentComponent::midiVelocity(uint32 keyIdx, float value)
{
    if (velocityMidi.getSelectedId() == 128)
    {
	float diff = value - midiPrevValue[keyIdx];
	float scaled_diff = 1.0 - exp(-7.0*diff);
	return (uint8)(127*scaled_diff + 0.5);
    }
    else
    {
	return (uint8)(velocityMidi.getSelectedId());
    }
}

bool MainContentComponent::isMidiEnabled()
{
    return enableMidi.getToggleState();
}

bool MainContentComponent::isOscEnabled()
{
    return enableOSC.getToggleState();
}

bool MainContentComponent::isSerialEnabled()
{
    return enableSerial.getToggleState() && isSynced;
}

uint8 MainContentComponent::midiChannel()
{
    return (uint8)(channelMidi.getSelectedId());
}

void MainContentComponent::paint (Graphics& g)
{
}

void MainContentComponent::buttonClicked(Button *button) 
{
    // Handle Serial Port Enable
    if (button == &enableSerial)
    {
	if (enableSerial.getToggleState())
    	{
	    if (0 == serialPortReader.start(comboSerialDevice.getText(), baudRate)) 
	    {
		updateGui();
	    }
	    else
	    {
		// TODO: Pop up an error message.
		enableSerial.setToggleState(false, dontSendNotification);
	    }
	}
	else
	{
	    // Make sure to update the GUI after stoping the serial port thread
	    // or sometimes the GUI will not reflect the correct state.
	    serialPortReader.stop();
	    keyboardMonitor.clearKeys();
	    updateGui();
	}
    }		

    // Handle OSC enable
    if (button == &enableOSC)
    {
	if (isOscEnabled())
    	{
	    oscSender.connect(textOSCHost.getText(), textOSCPort.getText().getIntValue());
	    //oscAddress = lo_address_new(textOSCHost.getText().toRawUTF8(), textOSCPort.getText().toRawUTF8());
	}
	else
	{
	    oscSender.disconnect();
//	    lo_address_free(oscAddress);
	}				

	updateGui();
    }

    // Handle MIDI enable
    if (button == &enableMidi)
    {
	if (isMidiEnabled())
	{
	    midiOutput = MidiOutput::openDevice(MidiOutput::getDevices().indexOf(deviceMidi.getText()));
	    midiOutput->sendMessageNow(MidiMessage::programChange(midiChannel(), midiProgram()));
	}
	else
	{
	    midiOutput->sendMessageNow(MidiMessage::allNotesOff(midiChannel()));
	    delete midiOutput;
	    midiOutput = NULL;
	}

	updateGui();
    }
}

void MainContentComponent::resized()
{
    // This is called when the MainContentComponent is resized.
    // If you add any child components, this is where you should
    // update their positions.
}

float MainContentComponent::getCalibratedValue(uint32 keyIdx, uint32 keyValue)
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

void MainContentComponent::textEditorTextChanged(TextEditor& t)
{
    if (&t == &textOSCHost)
    {
	getAppProperties().getUserSettings()->setValue("oschost", textOSCHost.getText());
    }

    if (&t == &textOSCPort)
    {
	getAppProperties().getUserSettings()->setValue("oscport", textOSCPort.getText());
    }

    getAppProperties().getUserSettings()->saveIfNeeded();
}

void MainContentComponent::comboBoxChanged(ComboBox *c)
{
    if (c == &channelMidi)
    {
	getAppProperties().getUserSettings()->setValue("midich", channelMidi.getSelectedId());
    }

    if (c == &octaveMidi)
    {
	getAppProperties().getUserSettings()->setValue("midioct", octaveMidi.getSelectedId());
    }
    
    if (c == &programMidi)
    {
	getAppProperties().getUserSettings()->setValue("midiprog", programMidi.getSelectedId());
    }
    
    if (c == &velocityMidi)
    {
	getAppProperties().getUserSettings()->setValue("midivel", velocityMidi.getSelectedId());
    }

    if (c == &aftertouchMidi)
    {
	getAppProperties().getUserSettings()->setValue("midiaft", aftertouchMidi.getSelectedId());
    }
	
    getAppProperties().getUserSettings()->saveIfNeeded();
}

uint8 MainContentComponent::midiOffset()
{
    return (octaveMidi.getSelectedId()-1)*12;
}

void MainContentComponent::sliderValueChanged(Slider *s)
{
    // Link the minimum range of the two sliders as they represent the same quantity
    if (s == &sliderNoteOnThresh)
    {
	if (sliderNoteOnThresh.getMinValue() > sliderAftertouchThresh.getMaxValue())
	{
	    sliderAftertouchThresh.setMaxValue(sliderNoteOnThresh.getMinValue());
	}
	sliderAftertouchThresh.setMinValue(sliderNoteOnThresh.getMinValue());
    }
    
    if (s == &sliderAftertouchThresh)
    {
	if (sliderAftertouchThresh.getMinValue() > sliderNoteOnThresh.getMaxValue())
	{
	    sliderNoteOnThresh.setMaxValue(sliderAftertouchThresh.getMinValue());
	}
	sliderNoteOnThresh.setMinValue(sliderAftertouchThresh.getMinValue());
    }

    getAppProperties().getUserSettings()->setValue("thr_on", sliderNoteOnThresh.getMaxValue());
    getAppProperties().getUserSettings()->setValue("thr_off", sliderNoteOnThresh.getMinValue());
    getAppProperties().getUserSettings()->setValue("thr_aft", sliderAftertouchThresh.getMaxValue());
    getAppProperties().getUserSettings()->saveIfNeeded();
}
