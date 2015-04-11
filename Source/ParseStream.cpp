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

    // You may need more than 2 states to represent this state machine robustly
    //
    // OUT_OF_FRAME : if current byte is 0xFF:
    //                   go to PRESYNC 
    //                else 
    //                   go to OUT_OF_FRAME.
    //                move to next byte
    //
    // PRESYNC      : if current byte is 0xFF:
    //                   go to READ_HEADER 
    //                else:
    //                   go to OUT_OF_FRAME (false sync). 
    //                move to next byte
    //
    // READ_HEADER  : store header (# and offset). 
    //                move to next byte
    //
    // READ_MSB     : if current byte is 0xFF:
    //                   go to PRESYNC (0xFF never occurs in MSB)
    //                else: 
    //                   store MSB go to READ_LSB. 
    //                move to next byte
    //
    // READ_LSB     : store LSB. 
    //                output message
    //                go to READ_MSB
    //                move to next byte                        
    //                                                             Current Byte 0xFF 
    //                             Current Byte is 0xFF     +---------------------------------------+
    //                           +----------------------+   |                                       |
    //                           |                      V   V  Current Byte 0xFF                    |
    //      START----->OUT_OF_FRAME                    PRESYNC------------------->READ_HEADER--+    |
    //                           ^                      |                                      V    |
    //                           +----------------------+                                     READ_MSB
    //                             Current Byte not 0xFF               Current Byte not 0xFF   |    ^
    //                                                              +--------------------------+    |
    //                                                              V                               |
    //                                                            READ_LSB--------------------------+
    //
    // If the packet is 1 byte short ie: the first 0xFF arrives during READ_LSB then the second 0xFF arrives during READ_MSB 
    // we go back to PRESYNC, then PRESYNC fails and we are out of frame and we'll resync to the next frame. We should probably
    // output some kind of warning if we ever fall out of frame, this would indicate a problem with the arduino programming.
    //
    // There's a corner case where a packet is 1 byte short and the first 0xFF arrives during READ_LSB, the second 0xFF arrives 
    // during READ_MSB, we go back to PRESYNC, the header byte happens to also be 0xFF which gets misinterpreted as the second sync byte, 
    // and then the subsequent packet parsing is total garbage but we don't know it until we fall out of sync at the end of the packet.
    // I don't forsee hitting this issue in practice but we could disallow 0xFF as the header byte (limited to 8x32=256 channels) 
    // 
    // Another corner case is where we are out of frame, the lsb of the data before the sync word is 0xFF, we see the first 0xFF
    // as the second, the second 0xFF as the header, and then parse the packet as garbage.. Disallowing 0xFF as the header 
    // can also solve this problem although we should just stay in READ_HEADER until the data is != 0xFF. If we go back to OUT_OF_FRAME
    // and the last LSB before the header is consistently 0xFF we may never sync. 
    //
    // I think this solves all corner cases I can think of so far. 

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
		    uint8_t channel_offset = (header_state & 0x0F); // + 1;
			
			uint16_t merge_bytes = ((buf[i] << 8) | buf[i+1]); //merge bytes

			unsigned short current_channel = ((merge_bytes >> 10) & 0x1F) + (channel_offset * 32); //current channel
			unsigned short current_value = (merge_bytes & 0x3FF);
			strArr.add(String("AK ")+String(current_channel)+String(" ")+String(current_value));
		}
	}


}

void ParseStream::reset() //reset object without need to kill/resurect object
{
	frame_state = 0; //if found a frame to sync up to
	header_state = 0; // count offsef for frame_state.
}
