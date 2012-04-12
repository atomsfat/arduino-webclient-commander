/*
  Web client
 
 This sketch connects to a website (http://www.google.com)
 using an Arduino Wiznet Ethernet shield. 
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 18 Dec 2009
 by David A. Mellis
 
 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = {  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress server(10,42,43,1); // node server
//10.42.43.1
//http://192.168.199.101/
// Initialize the Ethernet client library
// with the IP address and port of the server 
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;
// Other global variables
#define textBuffSize 10 //length of longest command string plus two spaces for CR + LF
char textBuff[textBuffSize]; //someplace to put received text
int charsReceived = 0;
boolean digitalValues[10];
int analogValues[6];


//status

long lastConnectionTime = 0;        // last time you connected to the server, in milliseconds
boolean lastConnected = false;      // state of the connection last time through the main loop
boolean startData = false; 

String queryString;


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



void setup() {

  Serial.begin(9600);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Serial);
  
  pinMode(13, OUTPUT);
  
  randomSeed(analogRead(0));


  // setting pins 0 to 9 as outputs
  // pins 10-13 are used by the Ethernet Shield
  //for(int  i= 0; i < 10; i++)  pinMode(i, OUTPUT);

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
   
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(200);
 

  // if you get a connection, report back via serial:

}
void loop()
{

  if (client.available()) {
    char c = client.read();

    if( c == '{'  ){
      if( startData==false){  
      startData=true;  
      int d = c;   
      }
    }

    if(startData==true){     
      textBuff[charsReceived] = c;           
      charsReceived++;
    } 
  }  
  
  // if there's no net connection, but there was one last time
  // through the loop, then stop the client:
  if (!client.connected() && lastConnected) {


    client.stop();
  
  for (int j= 0; j < textBuffSize; j =j + 1) {   
    int c = textBuff[j];    
  }


  parseReceivedText();
     
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected()) {
  
    if (client.connect(server, 8080)) {

      startData=false;
      charsReceived=0;

      memset(textBuff,0,sizeof(textBuff));
    
      // Make a HTTP request:
      queryString = "GET /arduino?id=atomsfat";
    
      readDigitalPins();

      queryString = queryString + "&d2=" + digitalValues[0] + "&d3=" + digitalValues[1] + "&d4=" + digitalValues[2] 
            + "&d5=" +  digitalValues[3] + "&d=6" +  digitalValues[4] + "&d7=" +  digitalValues[5] 
            + "&d8=" +  digitalValues[6] + "&d=9" +  digitalValues[7] + ;
  
      readAnalogPins();

      queryString = queryString + "&a0=" + analogValues[0] + "&a1=" + analogValues[1] + "&a2=" + analogValues[2] 
            + "&a3=" +  analogValues[3] + "&a=4" +  analogValues[4] + "&a5=" +  analogValues[5] 
            + "&a6=" +  analogValues[6] ;
 
      queryString = queryString + " HTTP/1.0";
      client.println(queryString);
      client.println();
    }
  }
  // store the state of the connection for next time through
  // the loop:
  lastConnected = client.connected();
}

void readDigitalPins()
{
  // output the valueof each digital pin
  for (int i = 2; i < 10; i++) outputPinState(i);
}
void outputPinState(int pin)
{
 
  if (digitalRead(pin)) {
    digitalValues[pin]=true;
  }
  else
    digitalValues[pin]=false;
   
}

void readAnalogPins()
{
  for (int i = 0; i < 6; i++) {
    analogValues[i] = analogRead(i);
  }
}

void parseReceivedText()
{

  mydata.red = random(255);
  mydata.green = random(255);
  mydata.blue = random(255);
  mydata.pixel = 3;
	//send the data
  ET.sendData();
  

  
  delay(100);  

}

