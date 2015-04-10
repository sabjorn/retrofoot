//
//  ParseStream.h
//  retrofoot
//
//  Created by Steven A. Bjornson on 2015-03-27.
//
//
#ifndef PARSESTREAM_H_INCLUDED
#define PARSESTREAM_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class ParseStream
{
public:
	//==============================================================================
	ParseStream();
	~ParseStrea();

	void parse(char *buf, uint32_t size, StringArray &strArr); //parse
	void reset(); //reset object without need to kill/resurect object

private:
	bool state; //1 if inframe
	uint8_t header_state; //store counter info



	//==============================================================================
};

#endif //PARSESTREAM_H_INCLUDED
