/*  
 *  ------ [802_02] - send packets -------- 
 *  
 *  Explanation: This program shows how to send packets to a gateway
 *  indicating the MAC address of the receiving XBee module 
 *  
 *  Copyright (C) 2016 Libelium Comunicaciones Distribuidas S.L. 
 *  http://www.libelium.com 
 *  
 *  This program is free software: you can redistribute it and/or modify 
 *  it under the terms of the GNU General Public License as published by 
 *  the Free Software Foundation, either version 3 of the License, or 
 *  (at your option) any later version. 
 *  
 *  This program is distributed in the hope that it will be useful, 
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of 
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 *  GNU General Public License for more details. 
 *  
 *  You should have received a copy of the GNU General Public License 
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>. 
 *  
 *  Version:           3.0
 *  Design:            David Gascón 
 *  Implementation:    Yuri Carmona
 */
 
#include <WaspXBee802.h>
#include <WaspFrame.h>

// Destination MAC address
//////////////////////////////////////////
char RX_ADDRESS[] = "0013A2004069379D"; //gateway
//char RX_ADDRESS[] = "0013A2004069379F"; //uredaj2
//////////////////////////////////////////

// Define the Waspmote ID
char WASPMOTE_ID[] = "node_02";


// define variable
uint8_t error;

int humidity = 0;
char *lastTemp;
char *tempNumber;
char *token;

void setup()
{
  // init USB port
  USB.ON();
  USB.println(F("Receiving and sending packets..."));
  
  // store Waspmote identifier in EEPROM memory
  frame.setID( WASPMOTE_ID );
  
  // init XBee
  xbee802.ON();

}


void loop()
{
  // receive XBee packet (wait for 10 seconds)
  error = xbee802.receivePacketTimeout( 10000 );

  // check answer  
  if( error == 0 ) 
  {
    // Show data stored in '_payload' buffer indicated by '_length'
    USB.print(F("Data: "));  
    USB.println( xbee802._payload, xbee802._length);
    
    // Show data stored in '_payload' buffer indicated by '_length'
    USB.print(F("Length: "));  
    USB.println( xbee802._length,DEC);
  }
  else {
    USB.print(F("Error receiving a packet:"));
    USB.println(error,DEC);     
  }

  //parsiranje procitane temperature
  char *temp = (char*)xbee802._payload;
  while ((temp = strtok(NULL,"#")) != NULL) {
    lastTemp = temp;
  }
  USB.println(lastTemp);
  
  token = strtok(lastTemp, ":");
  while( token != NULL ) {
    tempNumber = token;
    token = strtok(NULL, ":");
  }
  USB.println(tempNumber);

  free(temp);
  ///////////////////////////////////////////
  // 1. Create ASCII frame
  ///////////////////////////////////////////  

  // create new frame
  frame.createFrame(ASCII);   

  humidity = Utils.readHumidity();
  frame.addSensor(SENSOR_TCA, tempNumber);
  frame.addSensor(SENSOR_HUMA, humidity);
  
  ///////////////////////////////////////////
  // 2. Send packet
  ///////////////////////////////////////////  

  // send XBee packet
  error = xbee802.send( RX_ADDRESS, frame.buffer, frame.length );   
  
  // check TX flag
  if( error == 0 )
  {
    USB.println(F("send ok"));
    
    // blink green LED
    Utils.blinkGreenLED();
    
  }
  else 
  {
    USB.println(F("send error"));
    
    // blink red LED
    Utils.blinkRedLED();
  }

  // wait for five seconds
  delay(5000);
}



