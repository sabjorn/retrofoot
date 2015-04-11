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
	~ParseStream();

	void parse(uint8_t *buf, uint32_t size, StringArray &strArr); //parse
	void reset(); //reset object without need to kill/resurect object

private:
	uint8_t frame_state; //1 if inframe
	uint8_t header_state; //store counter info



	//==============================================================================
};

#endif //PARSESTREAM_H_INCLUDED
