#include <EasyTransfer.h>

//create object
EasyTransfer ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int pixel;
  int red;
  int green;
  int blue;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

void setup(){
  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial);
  
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));
  
}

void loop(){
  //this is how you access the variables. [name of the group].[variable name]
  mydata.red = random(255);
  mydata.green = random(255);
  mydata.blue = random(255);
  mydata.pixel = 3;
  //send the data
  ET.sendData();
  

  
  delay(100);
}
