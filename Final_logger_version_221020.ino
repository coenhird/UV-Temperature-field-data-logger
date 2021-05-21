// Working 27082020 with 2x VEML6075s on MUX ports 5 and 6 and 1x MCP9808 on port 0
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
float UVa1 = 0, UVb1 = 0;
float UVa2 = 0, UVb2 = 0;
float temperature1 = 0;

// the logging file
File logfile;

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
  pinMode(8,OUTPUT); // DONE signal 
  digitalWrite(8,LOW); //set DONE signal LOW

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
 // initialize the SD card
  Serial.print(F("Initializing SD card..."));
  // make sure that the default chip select pin is set to
  // output, even if you don't use it:
  bool sd_initialized = SD.begin(SD_CS_PIN);


// Opens file logger.csv if it exists, if not it creates and opens the file
  logfile = SD.open("logger.txt", FILE_WRITE); 

 
//-----------------------------------------------------------------
  DateTime now = RTC.now();
  
  // log time
  //logfile.print(now.unixtime()); // seconds since 1/1/1970
 // logfile.print(", ");
  logfile.print(now.year(), DEC);
  logfile.print(",");
  logfile.print(now.month(), DEC);
  logfile.print(",");
  logfile.print(now.day(), DEC);
  logfile.print(",");
  logfile.print(now.hour(), DEC);
  logfile.print(":");
  logfile.print(now.minute(), DEC);
  logfile.print(":");
  logfile.print(now.second(), DEC);

 #if ECHO_TO_SERIAL 
  Serial.print(now.year(), DEC);
  Serial.print(",");
  Serial.print(now.month(), DEC);
  Serial.print(",");
  Serial.print(now.day(), DEC);
  Serial.print(",");
  Serial.print(now.hour(), DEC);
  Serial.print(":");
  Serial.print(now.minute(), DEC);
  Serial.print(":");
  Serial.print(now.second(), DEC);
  Serial.print('"');
  #endif //ECHO_TO_SERIAL

// read the sensors values
  tcaselect(5);
  UVa1 = uv1.rawUva();
  UVb1 = uv1.rawUvb();
  logfile.print(", ");    
  logfile.print(UVa1);
  logfile.print(", ");
  logfile.print(UVb1);

  #if ECHO_TO_SERIAL
  Serial.print(", ");   
  Serial.print(UVa1);
  Serial.print(", ");   
  Serial.print(UVb1);
  #endif //ECHO_TO_SERIAL 

// read the sensors values
  tcaselect(6);
  UVa2 = uv2.rawUva();
  UVb2 = uv2.rawUvb();
  logfile.print(", ");    
  logfile.print(UVa2); 
  logfile.print(", ");
  logfile.print(UVb2); 

  #if ECHO_TO_SERIAL
  Serial.print(", ");   
  Serial.print(UVa2); 
  Serial.print(", ");   
  Serial.print(UVb2);
  #endif //ECHO_TO_SERIAL 

  //temp sensor
  tcaselect(0);
  // read the sensors value
  temperature1 = temp1.readTempC();
  logfile.print(", ");
  logfile.print(temperature1);

  #if ECHO_TO_SERIAL
  Serial.print(", ");
  Serial.print(temperature1);
  #endif //ECHO_TO_SERIAL 

  logfile.println();
  #if ECHO_TO_SERIAL
  Serial.println();
  #endif // ECHO_TO_SERIAL

  /* Display the results */
  Serial.print("Sensor #1 - ");
  Serial.print("Raw UVA reading:  "); Serial.println(UVa1);
  Serial.print("Sensor #1 - ");
  Serial.print("Raw UVB reading:  "); Serial.println(UVb1);

  Serial.print("Sensor #2 - ");
  Serial.print("Raw UVA reading:  "); Serial.println(UVa2);
  Serial.print("Sensor #2 - ");
  Serial.print("Raw UVB reading:  "); Serial.println(UVb2);

  Serial.println(temperature1);

  logfile.close();
  delay (1000);
}

void loop() 
{
  digitalWrite(8,HIGH); //set DONE signal HIGH
  // Power should be OFF
}
// The end
