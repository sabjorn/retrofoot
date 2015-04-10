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
	

}

void reset() //reset object without need to kill/resurect object
{
	state = 0;
	counter = 0;
}