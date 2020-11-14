//DOIT1 has mac: 24:6F:28:9E:9F:5C
//DOIT1 has mac: 0x24, 0x6F, 0x28, 0x9E, 0x9F, 0x5C
//Devkit module C has mac: 24:6F:28:36:53:F4

#include <esp_now.h>
#include <WiFi.h>
#include <CircusESP32Lib.h>

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
 
void setup() {
  // Initialize Serial Monitor
  Serial.begin(115200);
  
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
}
 
void loop() {
  circusESP32.begin();
  delay(8000);
  WiFi.disconnect(true);
  WiFi.mode(WIFI_STA);
  delay(8000);
}
