/* Akeru.h - sendSingleValues.ino
 * 
 * Copyleft Snootlab 2016
 * 
 * How to send a single analog value on the Sigfox network
 */

#include <Akeru.h>

// Sigfox instance management 
// Replace Serial1 with Serial2 or Serial3 if needed
Akeru akeru(&Serial1); // D0+D1

void setup() 
{    
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  while (!Serial); // wait for Serial Monitor 
  Serial.println("Starting...");
  
  // Check TD1208 communication
  if (!akeru.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }
  
  //akeru.echoOn(); // uncomment this line to see AT commands
}

void loop() 
{
  int sensorValue = analogRead(A0);
  
  // Trace on serial console
  Serial.println(sensorValue);

  // convert to hexadecimal before sending
  String data = akeru.toHex(sensorValue); 
  
  // Send in the mighty internet!
  // akeru.sendPayload() returns 0 if message failed.
  if (akeru.sendPayload(data))
  {
    Serial.println("Message sent !");
  }

  // Wait for 10 minutes.
  // Note that delay(600000) will block the Arduino (bug in delay()?)
  for (int second = 0; second < 600; second++) 
  {
    delay(1000);
  }
}
