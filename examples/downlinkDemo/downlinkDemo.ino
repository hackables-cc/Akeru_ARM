/* Akeru.h - downlinkDemo.ino
 * 
 * Copyleft Snootlab 2016
 * 
 * Downlink activation, and reception of a Sigfox network message
 * Note : downlink can take up to 1 minute so be patient if data doesn't come right away ;)
 */

#include <Akeru.h>

// Sigfox instance management 
// Replace Serial1 with Serial2 or Serial3 if needed
Akeru akeru(&Serial1); // D0+D1

void setup() 
{
  Serial.begin(9600);
  while (!Serial); // wait for Serial Monitor 
  
  // Check TD1208 communication
  if (!akeru.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }
  
  //akeru.echoOn(); // uncomment this line to see AT commands
  
  String data = "";
  if (akeru.receive(&data))
  {
    Serial.print("Data received : 0x");
    Serial.println(data);
  }
}

void loop() 
{
}
