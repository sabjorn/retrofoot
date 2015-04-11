#include "ParseStream.h"

//parses byte stream sent as an array in the form:
// |FF|FF|Total Number Channels|Offset|

ParseStream::ParseStream()
{
	frame_state = 0;
	header_state = 0;
}

ParseStream::~ParseStream()
{
}

void ParseStream::parse(uint8_t *buf, uint32_t size, StringArray &strArr) //parse
{	
	uint16_t i = 0;

	if (0 == frame_state)
	{
		for(i = 0; (i < size) && 0 == frame_state; i++)
		{
			// Find beginning of stream
			if((i + 1 < size) && (buf[i] == 0xFF) && (buf[i+1] == 0xFF))
			{
				frame_state = 1;
			}
		}
	}
	if (frame_state)
	{
		header_state = buf[i]; //store header
		i++;

		for(; (i < size) && frame_state; i += 2) //add 2 because each two-byte chunks
		{
			uint8_t channel_offset = (header_state & 0x0F) + 1;
			
			uint16_t merge_bytes = ((buf[i] << 8) | buf[i+1]); //merge bytes

			unsigned short current_channel = ((buf[i] >> 10) & 0x1F) + (channel_offset * 32); //current channel
			unsigned short current_value = (merge_bytes & 0x3FF);
			strArr.add(String("AK")+String(current_channel)+String(current_value));
		}
	}


}

void ParseStream::reset() //reset object without need to kill/resurect object
{
	frame_state = 0; //if found a frame to sync up to
	header_state = 0; // count offsef for frame_state.
}