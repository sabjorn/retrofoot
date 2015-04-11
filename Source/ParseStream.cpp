#include "ParseStream.h"

//parses byte stream sent as an array in the form:
// |FF|FF|Total Number Channels|Offset|

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
	uint16_t i = 0;

	if (0 == state)
	{
		for(i = 0; (i < size) && 0 == state; i++)
		{
			// Find beginning of stream
			if((i + 1 < size) && (buf[i] == 0xFF) && (buf[i+1] == 0xFF))
				state = 1;
		}
	}
	if (state)
	{
		header_state = buf[i]; //store header
		i++;

		for(i; (i < size) && state; i+2) //add 2 because each two-byte chunks
		{
			uint8_t channel_offset = (header_state & 0x0F) + 1;
			
			uint16_t merge_bytes = (buf[i] << 8) | buff[i+1]; //merge bytes

			unsigned short current_channel = ((buf[i] >> 10) & 0x1F) + (channel_offset * 32); //current channel
			unsigned short current_value = (merge_bytes & 0x3FF);
			strArr.add('AK');
			strArr.add(String(current_channel));
			strArr.add(String(current_value));
		}
	}


}

void reset() //reset object without need to kill/resurect object
{
	state = 0; //if found a frame to sync up to
	frame_found  = 0;
	header_state = 0; // count offsef for state.
}