#include <EasyTransfer.h>

#include <TimerOne.h>  
#include "LPD6803.h"


// Choose which 2 pins you will use for output.
// Can be any valid output pins.
int dataPin = 2;       // 'yellow' wire
int clockPin = 3;      // 'green' wire
int analogValues[6];


LPD6803 strip = LPD6803(20, dataPin, clockPin);
int ledLights[20];
//create object
EasyTransfer ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int pixel;
  int red;
  int green;
  int blue;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

unsigned int Color(byte r, byte g, byte b) {
  //Take the lowest 5 bits of each value and append them end to end
  return( ((unsigned int)g & 0x1F )<<10 | ((unsigned int)b & 0x1F)<<5 | (unsigned int)r & 0x1F);
}


//create initial image
void showInitImage() {
    for (int i=0; i < strip.numPixels(); i++) {
      strip.setPixelColor(i, 0);
      ledLights[i]=0;
    }   
      
     
}

void setup(){

 strip.setCPUmax(70);  // start with 50% CPU usage. up this if the strand flickers or is slow
    // Start up the LED counter
  strip.begin();
  
  showInitImage();
  
  // Update the strip, to start they are all 'off'
  strip.show();


  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Serial);
  
  
}

void loop(){
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    //this is how you access the variables. [name of the group].[variable name]
    //since we have data, we will blink it out. 
    
      int c;
      c = Color(mydata.red, mydata.green, mydata.blue);
      
      
      if(mydata.pixel < strip.numPixels()){
        ledLights[mydata.pixel]= c;
      }
      

    for (byte i=0; i < strip.numPixels(); i++) {
        strip.setPixelColor(i,ledLights[i]);   
    }
     strip.doSwapBuffersAsap(20);   
     
     
     // strip.setPixelColor(mydata.pixel, c);
     // strip.doSwapBuffersAsap(20); 
    }
  
    
  
  //you should make this delay shorter then your transmit delay or else messages could be lost
  delay(50);
}
