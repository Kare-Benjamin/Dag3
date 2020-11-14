#include <ss_oled.h>
#include <string>   

// if your system doesn't have enough RAM for a back buffer, comment out
// this line (e.g. ATtiny85)
#define USE_BACKBUFFER

#ifdef USE_BACKBUFFER
static uint8_t ucBackBuffer[1024];
#else
static uint8_t *ucBackBuffer = NULL;
#endif

// Use -1 for the Wire library default pins
// or specify the pin numbers to use with the Wire library or bit banging on any GPIO pins
// These are the pin numbers for the M5Stack Atom default I2C
#define SDA_PIN 21
#define SCL_PIN 22
// Set this to -1 to disable or the GPIO pin number connected to the reset
// line of your display if it requires an external reset
#define RESET_PIN -1
// let ss_oled figure out the display address
#define OLED_ADDR -1
// don't rotate the display
#define FLIP180 0
// don't invert the display
#define INVERT 0
// Bit-Bang the I2C bus
#define USE_HW_I2C 0

// Change these if you're using a different OLED display
#define MY_OLED OLED_128x64
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
//#define MY_OLED OLED_64x32
//#define OLED_WIDTH 64
//#define OLED_HEIGHT 32

SSOLED ssoled;




#include "DHT.h"

#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.

// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);



  int measures;

  float avg_temperature;
  float avg_humidity;

  float sum_temperature;
  float sum_humidity;





void setup() {
int rc;
// The I2C SDA/SCL pins set to -1 means to use the default Wire library
// If pins were specified, they would be bit-banged in software
// This isn't inferior to hw I2C and in fact allows you to go faster on certain CPUs
// The reset pin is optional and I've only seen it needed on larger OLEDs (2.4")
//    that can be configured as either SPI or I2C
//
// oledInit(SSOLED *, type, oled_addr, rotate180, invert, bWire, SDA_PIN, SCL_PIN, RESET_PIN, speed)

rc = oledInit(&ssoled, MY_OLED, OLED_ADDR, FLIP180, INVERT, USE_HW_I2C, SDA_PIN, SCL_PIN, RESET_PIN, 400000L); // use standard I2C bus at 400Khz
  if (rc != OLED_NOT_FOUND)
  {
    char *msgs[] = {(char *)"SSD1306 @ 0x3C", (char *)"SSD1306 @ 0x3D",(char *)"SH1106 @ 0x3C",(char *)"SH1106 @ 0x3D"};
    oledFill(&ssoled, 0, 1);
    oledWriteString(&ssoled, 0,0,0,msgs[rc], FONT_NORMAL, 0, 1);
    oledSetBackBuffer(&ssoled, ucBackBuffer);
    delay(2000);
  }

  
Serial.begin(9600);
Serial.println(F("DHTxx test!"));

dht.begin();  


oledFill(&ssoled, 0, 1);
} /* setup() */



void loop() {


  // Wait a few seconds between measurements.
  delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print(F("Humidity: "));
  Serial.print(h);
  Serial.print(F("%  Temperature: "));
  Serial.print(t);
  Serial.print(F("°C "));
  Serial.print(f);
  Serial.print(F("°F  Heat index: "));
  Serial.print(hic);
  Serial.print(F("°C "));
  Serial.print(hif);
  Serial.println(F("°F"));


  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  sum_humidity += humidity;
  sum_temperature += temperature;
  
  measures += 1;
char* test = "hello";

  if(measures > 2){
       avg_temperature = (sum_temperature)/measures;
       avg_humidity = (sum_humidity)/measures;
       
         Serial.println("hello1");
    if(measures > 10000){
      avg_temperature = 0;
      avg_humidity = 0;
      sum_humidity = 0;
      sum_temperature = 0;
      measures = 0;
        Serial.println("hello2");
    }
 }

  
  char temperature_string[10];
  sprintf(temperature_string, "%.2f", temperature);

  char avg_temperature_string[10];
  sprintf(avg_temperature_string, "%.2f", avg_temperature);

  char humidity_string[10];
  sprintf(humidity_string, "%.2f", humidity);

  char avg_humidity_string[10];
  sprintf(avg_humidity_string, "%.2f", avg_humidity); 

  char measures_string[10];
  sprintf(measures_string, "%d", measures); 
  
  // put your main code here, to run repeatedly:

 
  oledWriteString(&ssoled, 0,0,0,(char *)"Temp: ", FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,44,0,temperature_string , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,0,2,(char *)"Tavg: ", FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,44,2,avg_temperature_string , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,88,2,(char *)"@" , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,96,2,measures_string , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,0,4,(char *)"Hum: ", FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,44,4,humidity_string , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,0,6,(char *)"Havg: ", FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,44,6,avg_humidity_string , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,88,6,(char *)"@" , FONT_NORMAL, 0, 1);
  oledWriteString(&ssoled, 0,96,6,measures_string , FONT_NORMAL, 0, 1);
  //oledWriteString(&ssoled, 0,0,8,(char *)"Sampl: ", FONT_NORMAL, 0, 1);
  //oledWriteString(&ssoled, 0,44,8,measures_string , FONT_NORMAL, 0, 1);
  delay(2000);
  
  Serial.print("Measures are: ");
  Serial.println(measures);
} /* loop() */
