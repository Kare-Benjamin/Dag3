//DOIT1 has mac: 24:6F:28:9E:9F:5C
//DOIT1 has mac: 0x24, 0x6F, 0x28, 0x9E, 0x9F, 0x5C
//Devkit module C has mac: 24:6F:28:36:53:F4

#include <esp_now.h>
#include <WiFi.h>
#include <CircusESP32Lib.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ------------------------------------------------
// These are the CircusESP32Lib related declarations
// ------------------------------------------------

char ssid[] = "ARNardo"; // Place your wifi SSID here
char password[] =  "A1R2E3K4"; // Place your wifi password here
char token[] = "eyJhbGciOiJIUzI1NiJ9.eyJqdGkiOiIyMDQ4In0.UJfXNTEHjAK2nywsuIMd8ZaZXGNIxYodu8zwAHwGyTg"; // Place your token, find it in 'account' at Circus. It will identify you.
char server[] = "www.circusofthings.com";
char temperature_key[] = "16906";  // Place the Key of the signal you created at Circus Of Things for the Temperature
char humidity_key[] = "15160";  // Place the Key of the signal you created at Circus Of Things for the Humidity
char order_key[] = "5957"; // Type the Key of the Circus Signal you want the ESP32 listen to. 
CircusESP32Lib circusESP32(server,ssid,password); // The object representing an ESP32 to whom you can order to Write or Read

// Structure example to receive data
// Must match the sender structure
typedef struct struct_message {
    char a[32];
    int b;
    float c;
    String d;
    float e;
} struct_message;

// Create a struct_message called myData
struct_message myData;

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("Message: ");
  Serial.print(myData.a);
  Serial.print(" number: ");
  Serial.print(myData.b);
  Serial.print(" from: ");
  Serial.println(myData.d);
  Serial.print("Temperature is: ");
  Serial.println(myData.c);
  Serial.print("Preassure is : ");
  Serial.println(myData.e);
  Serial.println();
}



#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SEALEVELPRESSURE_HPA (1013.25)


unsigned long delayTime;
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  pinMode(LED_BUILTIN , OUTPUT); // Define our pin for the relay as an output   
  
  // Set device as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info
  esp_now_register_recv_cb(OnDataRecv);

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


  
}
 
void loop() {
  circusESP32.begin();
    double dashboard_order = circusESP32.read(order_key,token); 
    //Serial.print(dashboard_order);
    // I decided that a signal value of "1" at Circus will set the output as HIGH in ESP32 
    // and the normally open terminal of the relay where my light is will be OPEN. 
    // So, if Signal=1 then light turns OFF and if Signal=0 then light turns ON
    if(dashboard_order == 1){
        digitalWrite(LED_BUILTIN , HIGH);  // Open relay contact by setting the voltage HIGH
    } else {
        digitalWrite(LED_BUILTIN , LOW);  // Close relay contact by setting the voltage LOW
    }
  circusESP32.write(temperature_key,myData.c,token); // Report the temperature measured to Circus.
  circusESP32.write(humidity_key,myData.e,token); // Report the humidity measured to Circus.
  delay(10);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  delay(8000);
  display.setCursor(0, 10);
  display.clearDisplay();
  display.print("T:");
  display.print(myData.c);
  display.println(" C");
  display.print("H:");
  display.print(myData.e);
  display.println("%"); 
  display.display(); 
}
