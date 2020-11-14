#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <CircusESP32Lib.h>

// Module connection pins (Digital Pins)
#define CLK 32
#define DIO 33
#define TEST_DELAY   2000

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme; // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

// ------------------------------------------------
// These are the CircusESP32Lib related declarations
// ------------------------------------------------

char ssid[] = "ARNardo"; // Place your wifi SSID here
char password[] =  "A1R2E3K4"; // Place your wifi password here
char token[] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiIyMDQ4In0.UJfXNTEHjAK2nywsuIMd8ZaZXGNIxYodu8zwAHwGyTg"; // Place your token, find it in 'account' at Circus. It will identify you.
char server[] = "www.circusofthings.com";
char temperature_key[] = "16906";  // Place the Key of the signal you created at Circus Of Things for the Temperature
char humidity_key[] = "15160";  // Place the Key of the signal you created at Circus Of Things for the Humidity
CircusESP32Lib circusESP32(server,ssid,password); // The object representing an ESP32 to whom you can order to Write or Read

void setup() {
  Serial.begin(115200);

  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text


  //display.setBrightness(0x0f);
  bool status;

  // default settings
  // (you can also pass in a Wire library object like &Wire2)
  
  status = bme.begin(0x76);  
  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1);
  }

  Serial.println("-- Default Test --");
  delayTime = 1000;

  Serial.println();


      circusESP32.begin(); // Let the Circus object set up itself for an SSL/Secure connection

}

void loop() {

  float temperature = bme.readTemperature();
  float humidity = bme.readHumidity();


  display.setCursor(0, 10);
  display.clearDisplay();
  display.print("T:");
  display.print(bme.readTemperature());
  display.println(" C");
  display.print("H:");
  display.print(bme.readHumidity());
  display.println("%"); 
  display.display(); 

  printValues();
  delay(delayTime);
  //display.showNumberDec(bme.readTemperature()); // Expect: _301
  delay(3000);
  //display.showNumberDec(bme.readHumidity()); // Expect: _301
  delay(2000);

  circusESP32.write(temperature_key,temperature,token); // Report the temperature measured to Circus.
  circusESP32.write(humidity_key,humidity,token); // Report the humidity measured to Circus.
  
}

void printValues() {
  Serial.print("Temperature = ");
  Serial.print(bme.readTemperature());
  Serial.println(" *C");
  
  // Convert temperature to Fahrenheit
  /*Serial.print("Temperature = ");
  Serial.print(1.8 * bme.readTemperature() + 32);
  Serial.println(" *F");*/
  
  Serial.print("Pressure = ");
  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Approx. Altitude = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Humidity = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");
  
  Serial.println();

}
