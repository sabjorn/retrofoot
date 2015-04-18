#include <math.h>

int analogPin[2] = {0,1}; // read from multiplexer using analog inputs 0 and 1
int binary_pins[4] = {2,3,4,5}; // strobe is attached to digital pin 2
uint16_t input_value[2][16]; // to hold a2d values
uint8_t message_header[3];
uint8_t output_array[64]; // store input values to output for serial

uint8_t counter = 0;


void setup()
{
 Serial.begin(115200);

 for (int i=0; i < 4; i++){
  pinMode(binary_pins[i],OUTPUT);
 }
 pinMode(analogPin[0], INPUT);
 pinMode(analogPin[1], INPUT);
 
 analogReference(DEFAULT);
}

void loop()
{
  // for thie sketch, these will always be like this;
  message_header[0] = 0xFF;
  message_header[1] = 0xFF;
  message_header[2] = 0; // number of channels and channel offset, zero for this instance

  //read inputs
  for (int i = 0; i < 16; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      digitalWrite(binary_pins[j],(i>>j) & 1);
    }

    input_value[0][i] = analogRead(analogPin[0]);
    input_value[1][i] = analogRead(analogPin[1]);
  }

  // test case
  for (int i = 0; i < 2; i++)
  {
    for (int j = 0; j < 16; j++)
    {
      input_value[i][j] = j * counter;
    }
  }
  counter++;

  if (counter == 64)
  {
    counter = 0;
  }
  
  for(int i = 0; i < 2; i++)
  {
    for (int j=0; j < 16; j++)
    {
      uint8_t channel = (j + (i*16)); //which key
      uint16_t value = input_value[i][j]; // value of key
      uint16_t merge = ((channel << 10) | value);

      output_array[((j+(i*16)) * 2)] = (merge >> 8); //merge channel and value to one key
      output_array[((j+(i*16)) * 2) + 1] = (merge & 0xFF);
    }
  }  
  Serial.write(message_header,3);
  Serial.write(output_array,64);
}

