/* iot_imag_fox_DemoTest.ino
*
* Copyright Hackables.cc 2018
*
* This sample code is a demo project for Sigfox Library :
* - Check if TD1208 is available on setup 
* - Read TD1208 temperature, 
*               supply voltage, 
*               ID, 
*               hardware version,
*               firmware version, 
*               power level.
* - Send temperature & supply voltage over the network
*/

#include <Arduino.h>        // required before wiring_private.h
#include "wiring_private.h" // pinPeripheral() function
#include <Akeru.h>

// Pins Mapping for the IOT Immagination Board
// Radio
#define RADIO_RST_PIN 8
// Mikro Bus 1
#define MB1_AN  A0
#define MB1_RST A2
#define MB1_CS  A4
#define MB1_PWM 3
#define MB1_INT 6
// Mikro Bus 2
#define MB2_AN  A1
#define MB2_RST A3
#define MB2_CS  A5
#define MB2_PWM 4
#define MB2_INT 7

// Serial ports
// The serial ports in the IOT Imag. board are 
// distributed in the following manner:
// - Serial  . Used for debug (redirected to USB serial).
// - Serial1 . Used for MikroBus 1 Serial
// - Serial2 . Used for MikroBus 2 Serial
// - Serial3 . Used to communicate with Lora radio RN2483A

// This will redirect the serial prints to the USB.
#if defined(ARDUINO_SAMD_ZERO) && defined(SERIAL_PORT_USBVIRTUAL)
  // Required for Serial on Zero based boards
  #define Serial SERIAL_PORT_USBVIRTUAL
#endif


// Serial ports
// Serial3 pin and pad definitions (in Arduino files Variant.h & Variant.cpp)
#define PIN_SERIAL3_RX       (5ul)                // Pin description number for PIO_SERCOM on D5
#define PIN_SERIAL3_TX       (2ul)                // Pin description number for PIO_SERCOM on D4
#define PAD_SERIAL3_TX       (UART_TX_PAD_2)      // SERCOM pad 2 TX
#define PAD_SERIAL3_RX       (SERCOM_RX_PAD_3)    // SERCOM pad 3 RX

#define PIN_SERIAL2_RX       (13ul)               // Pin description number for PIO_SERCOM on D5
#define PIN_SERIAL2_TX       (11ul)               // Pin description number for PIO_SERCOM on D4
#define PAD_SERIAL2_TX       (UART_TX_PAD_0)      // SERCOM pad 2 TX
#define PAD_SERIAL2_RX       (SERCOM_RX_PAD_1)    // SERCOM pad 3 RX

// Serial1 already setup by default on Arduino Zero
Uart Serial2( &sercom1, PIN_SERIAL2_RX, PIN_SERIAL2_TX, PAD_SERIAL2_RX, PAD_SERIAL2_TX );
Uart Serial3( &sercom2, PIN_SERIAL3_RX, PIN_SERIAL3_TX, PAD_SERIAL3_RX, PAD_SERIAL3_TX );

void SERCOM1_Handler()
{
  Serial2.IrqHandler();
}

void SERCOM2_Handler()
{
  Serial3.IrqHandler();
}

#define sigfoxSerial Serial3

// Sigfox instance management 
// Replace Serial1 with Serial2 or Serial3 if needed
Akeru akeru( &sigfoxSerial ); // D0+D1

void setup()
{
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  
  // Assign pins 11 & 13 SERCOM functionality on Serial2
  pinPeripheral(11, PIO_SERCOM);
  pinPeripheral(13, PIO_SERCOM);
  // Assign pins 2 & 5 SERCOM functionality on Serial3
  pinPeripheral(2, PIO_SERCOM);
  pinPeripheral(5, PIO_SERCOM);

  // Sigfox Radio Hard Reset
  pinMode(RADIO_RST_PIN, OUTPUT);
  digitalWrite(RADIO_RST_PIN, LOW);
  delay(1000);
  digitalWrite(RADIO_RST_PIN, HIGH);
  delay(1000);
  
  while (!Serial); // wait for Serial Monitor 
  Serial.println("Demo sketch for Akeru library :)");
  
  Serial.println("Waiting for Sigfox Modem Serial port...");
  while(!sigfoxSerial); // wait for Sigfox Serial Monitor 
  Serial.println("Sigfox Modem serial Ready!");

  // Check TD1208 communication
  if (!akeru.begin())
  {
    Serial.println("TD1208 KO");
    while(1);
  }

  Serial.println("TD1208 OK! :)");
  akeru.echoOn(); // comment this line to hide AT commands
}

void loop()
{
  // Read module temperature
  int temperature = 0;
  if (akeru.getTemperature(&temperature))
  {
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" C");
  }
  else
  {
    Serial.println("Temperature KO");
  }

  // Read module supply voltage
  float voltage = 0.0;
  if (akeru.getVoltage(&voltage))
  {
    Serial.print("Supply voltage = ");
    Serial.print(voltage);
    Serial.println(" V");
  }
  else
  {
    Serial.println("Supply voltage KO");
  }

  // Read module identification
  String id = "";
  if (akeru.getID(&id))
  {
    Serial.print("ID = ");
    Serial.println(id);
  }
  else
  {
    Serial.println("ID KO");
  }

  // Read module hardware version
  String hardware = "";
  if (akeru.getHardware(&hardware))
  {
    Serial.print("Hardware version = ");
    Serial.println(hardware);
  }
  else
  {
    Serial.println("Hardware version KO");
  }

  // Read module firmware version
  String firmware = "";
  if (akeru.getFirmware(&firmware))
  {
    Serial.print("Firmware version = ");
    Serial.println(firmware);
  }
  else
  {
    Serial.println("Firmware version KO");
  }

  // Read power 
  int power = 0;
  if (akeru.getPower(&power))
  {
    Serial.print("Power level = ");
    Serial.print(power);
    Serial.println(" dB");
  }
  else
  {
    Serial.println("Power level KO");
  }

  // Convert to hexadecimal before sending
  String temp = akeru.toHex(temperature);
  String volt = akeru.toHex(voltage);
  
  String msg = temp + volt; // Put everything together
  
  if (akeru.sendPayload(msg))
  {
    Serial.println("Message sent !");
  }
  else
  {
    Serial.println("Message not sent !");
  }

  // End of tests
  while (1);
}

