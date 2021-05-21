//  with 2x VEML6075s on MUX ports 5 and 6 and 1x MCP9808 on port 0
// Coen Hird

#include <Wire.h> //include Wire library
#include <SD.h>
#include <RTClib.h>
#include <SPI.h>

// Sensor includes
#include <Adafruit_MCP9808.h>   // Temperature Sensor
#include <SparkFun_VEML6075_Arduino_Library.h>

// Definitions
#define TCAADDR 0x70  //define then select multiplexor port address
#define SD_CS_PIN 10  // SD card chip select pin
#define ECHO_TO_SERIAL   1 // echo data to serial port

/* Assign sensor IDs/objects*/
VEML6075 uv1 = VEML6075();
VEML6075 uv2 = VEML6075();
Adafruit_MCP9808 temp1 = Adafruit_MCP9808();
RTC_DS1307 RTC;

// Global variables
float irCOMP1 = 0, vlCOMP1 = 0;
float irCOMP2 = 0, vlCOMP2 = 0;
float UVa1 = 0, UVb1 = 0;
float UVa2 = 0, UVb2 = 0;
float temperature1 = 0;

// Multiplexer function
void tcaselect(uint8_t i) 
{
  if (i > 7) return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << i);
  Wire.endTransmission();  
}

//--------------------------------------------------------------------------
void setup(void)  
 {

  Wire.begin();
  Serial.begin(9600);
  RTC.begin();

  Serial.println(F("Setup complete"));
    
  /* Initialise the 1st sensor */
  tcaselect(0);
  if (!temp1.begin(0x18))
  {
    Serial.println(F("Couldn't find MCP9808!"));
    while (1); // hang forever
  }
  Serial.println(F("Found MCP9808!"));
  
  /* Initialise the 2nd sensor */
  tcaselect(5);
  if(! uv1.begin())
  {
      // There was a problem detecting the VEML6075 1 ... check your connections
      Serial.println(F("Failed to communicate with VEML6075 sensor1"));
      while(1);
    }
    Serial.println(F("Found UV sensor 1!"));
   /* Initialise the 3rd sensor */
  tcaselect(6);
  if(! uv2.begin())
  {
      // There was a problem detecting the VEML6075 #2 ... check your connections
      Serial.println(F("Failed to communicate with VEML6075 sensor2"));
      while(1);
    }
    Serial.println(F("Found UV sensor 2!"));
}

void loop() 
{

  DateTime now = RTC.now();

// read the sensors values
  tcaselect(5);
  irCOMP1 = uv1.irCompensation();
  vlCOMP1 = uv1.visibleCompensation();
  UVa1 = uv1.rawUva();
  UVb1 = uv1.rawUvb();

// read the sensors values
  tcaselect(6);
  irCOMP2 = uv2.irCompensation();
  vlCOMP2 = uv2.visibleCompensation();
  UVa2 = uv2.rawUva();
  UVb2 = uv2.rawUvb();

  //temp sensor
  tcaselect(0);
  // read the sensors value
  temperature1 = temp1.readTempC();

  /* Display the results */
  Serial.print("Sensor #1 - ");
  Serial.print("infrared compensation reading:  "); Serial.println(irCOMP1);
  Serial.print("Sensor #1 - ");
  Serial.print("visible compensation reading:  "); Serial.println(vlCOMP1);
  Serial.print("Sensor #1 - ");
  Serial.print("Raw UVA reading:  "); Serial.println(UVa1);
  Serial.print("Sensor #1 - ");
  Serial.print("Raw UVB reading:  "); Serial.println(UVb1);
  
  Serial.print("infrared compensation reading:  "); Serial.println(irCOMP2);
  Serial.print("Sensor #2 - ");
  Serial.print("visible compensation reading:  "); Serial.println(vlCOMP2);
  Serial.print("Sensor #2 - ");
  Serial.print("Raw UVA reading:  "); Serial.println(UVa2);
  Serial.print("Sensor #2 - ");
  Serial.print("Raw UVB reading:  "); Serial.println(UVb2);

  Serial.print("Temp sensor - ");
  Serial.println(temperature1);

  delay (2000);
}
// The end
