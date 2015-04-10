#include "ParseStream.h"

ParseStream::ParseStream()
{
	state = 0;
	counter = 0;    
}

ParseStream::~ParseStream()
{
}

void parse(char *buf, uint32_t size, StringArray &strArr) //parse
{	
	for(uint16_t i = 0; i < size; i++)
	{
		if(!state)
		{	
			// Find beginning of stream
			if((i + 1 < size) && (buf[i] == 0xFF) && (buf[i+1] == 0xFF))
			{
				state = 1;
				i++;
			}
		}
		else
	}

}

void reset() //reset object without need to kill/resurect object
{
	state = 0; //if found a frame to sync up to
	header_state = 0; // count offsef for state.
}