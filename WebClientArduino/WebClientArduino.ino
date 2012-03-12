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

void setup() {
  // setting pins 0 to 9 as outputs
  // pins 10-13 are used by the Ethernet Shield
  for(int  i= 0; i < 10; i++)  pinMode(i, OUTPUT);

  // start the serial library:
  Serial.begin(9600);
  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    for(;;)
      ;
  }
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

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
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
    Serial.print("===");    
  for (int j= 0; j < textBuffSize; j =j + 1) {
    
    Serial.print(textBuff[j]);
    Serial.print("_");
    int c = textBuff[j];    
    Serial.print(c);  
  }
  Serial.print("===");

  parseReceivedText();
     
  }

  // if you're not connected, and ten seconds have passed since
  // your last connection, then connect again and send data:
  if(!client.connected()) {
    if (client.connect(server, 8080)) {
      Serial.println("connected");
      startData=false;
      charsReceived=0;

      memset(textBuff,0,sizeof(textBuff));
    
      // Make a HTTP request:
      queryString = "GET /arduino?id=atomsfat";
    
      readDigitalPins();

      queryString = queryString + "&d0=" + digitalValues[0] + "&d1=" + digitalValues[1] + "&d2=" + digitalValues[2] 
            + "&d3=" +  digitalValues[3] + "&d=4" +  digitalValues[4] + "&d5=" +  digitalValues[5] 
            + "&d6=" +  digitalValues[6] + "&d=7" +  digitalValues[7] + "&d8=" + digitalValues[8]
            + "&d9=" + digitalValues[9] ;
  
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
  for (int i = 0; i < 10; i++) outputPinState(i);
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
  switch (textBuff[1]) {
    case 'a' : writeAnalogPin();	  break;
    case 'd' : writeDigitalPin();	 break;
    case 'p' : setPinMode();		 break;
  }
}
void writeDigitalPin()
  // if we got here, textBuff[0] = 'd' and textBuff[1] = 'w'
{
  int pin = -1;
  int pinSetting = -1;
  if (textBuff[4] == '=' && textBuff[7] == '}') {
    Serial.println("->dw"); 
	  //if yes, get the pin number, setting, and set the pin
	  pin = parseDigit(textBuff[3]);
	  pinSetting = parsePinSetting();
	  if(pin > -1 && pinSetting == 0) {
	    digitalWrite(pin, LOW);
	    client.println("OK");
	  }
	  if(pin > -1 && pinSetting == 1) {
	    digitalWrite(pin, HIGH);
	    client.println("OK");
	  }
	 // if(pin < 0 || pinSetting < 0) printErrorMessage();
	}
  // else printErrorMessage();
}
int parseDigit(char c)
{
  int digit = -1;
  digit = (int) c - 0x30; // subtracting 0x30 from ASCII code gives value
  if(digit < 0 || digit > 9) digit = -1;
  return digit;
}
int parsePinSetting()
//look in the text buffer to find the pin setting
//return -1 if not valid
{
  int pinSetting = -1;
  if(textBuff[5] == 'l' && textBuff[6] == 'o') pinSetting = 0;
  if(textBuff[5] == 'h' && textBuff[6] == 'i') pinSetting = 1;
  return pinSetting;
}
void writeAnalogPin()
  // if we got here, textBuff[1] = 'a' and textBuff[2] = 'w'
{
  int pin = -1;
  int pwmSetting = -1;
   if (textBuff[4] == '=') {
    Serial.println("------>aw___"); 
	//if yes, get the pin number, setting, and set the pin
	pin = parseDigit(textBuff[3]);
	 Serial.println(pin); 
	if(pin == 3 || pin == 5 || pin == 6 || pin == 9) {
	  pwmSetting = parsepwmSetting();
	   Serial.println("::");
	   Serial.println(pwmSetting); 
	  if(pwmSetting >= 0 && pwmSetting <= 255) {
	    analogWrite(pin,pwmSetting);
	    client.println("OK");
	  }
	 // else printErrorMessage();
	}
//	else printErrorMessage();
    }
  // else printErrorMessage();
}

int parsepwmSetting()
{
  int pwmSetting = 0;
  int textPosition = 5;  //start at textBuff[5]
  int digit;
  do {
    digit = parseDigit(textBuff[textPosition]); //look for a digit in textBuff
    if (digit >= 0 && digit <=9) {		  //if digit found
	pwmSetting = pwmSetting * 10 + digit;     //shift previous result and add new digit
    }
    else pwmSetting = -1;
    textPosition++;				     //go to the next position in textBuff
  }
  //if not at end of textBuff and not found a CR and not had an error, keep going
  while(textPosition < 8 && textBuff[textPosition] != '}' && pwmSetting > -1);
   //if value is not followed by a CR, return an error
  if(textBuff[textPosition] != '}') pwmSetting = -1;
  return pwmSetting;
}

 void setPinMode()
  // if we got here, textBuff[0] = 'p'
{
  int pin = -1;
  int pinModeSetting = -1;
  if (textBuff[2] == 'm' && textBuff[4] == '=' && textBuff[7] == '}') {
	  //if yes, get the pin number, setting, and set the pin
	  pin = parseDigit(textBuff[3]);
	  pinModeSetting = parseModeSetting();
	  if(pin > -1 && pinModeSetting == 0) {
	    pinMode(pin, OUTPUT);
	    client.println("OK");
	  }
	  if(pin > -1 && pinModeSetting == 1) {
	    pinMode(pin, INPUT);
	    client.println("OK");
	  }
	 // if(pin < 0 || pinModeSetting < 0) printErrorMessage();
	}
   //else printErrorMessage();
}
int parseModeSetting()
//look in the text buffer to find the pin setting
//return -1 if not valid
{
  int pinSetting = -1;
  if(textBuff[5] == 'o' && textBuff[6] == 'u') pinSetting = 0;
  if(textBuff[5] == 'i' && textBuff[6] == 'n') pinSetting = 1;
  return pinSetting;
}
