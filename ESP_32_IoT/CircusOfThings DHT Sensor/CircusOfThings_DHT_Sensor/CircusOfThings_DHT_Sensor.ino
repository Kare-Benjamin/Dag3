#include <CircusESP32Lib.h>
#include <DHT.h>


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

// ------------------------------------------------
// These are the Temperature Example related declarations
// ------------------------------------------------

#define DHTPIN 4      // digital of your ESP32 connected to DHT11
#define DHTTYPE DHT22 // exact model of temperature sensor DHT 11 for the general library
DHT dht(DHTPIN, DHTTYPE); // The object representing your DHT11 sensor


 
void setup() {
    Serial.begin(115200); // Remember to match this value with the baud rate in your console
    dht.begin(); // Set the DHT11 ready
    circusESP32.begin(); // Let the Circus object set up itself for an SSL/Secure connection
}
 
void loop() { // Now that all is set up, let's begin with the tasks

    delay(10000);
    // Let the library get the Temperature that DHT11 probe is measuring.
    float t = dht.readTemperature(); 
    if (isnan(t))
        t=-1; // if so, check the connection of your DHT11 sensor... something is disconnected ;-)
    float h = dht.readHumidity();
    if (isnan(h))
        h=-1; // if so, check the connection of your DHT11 sensor... something is disconnected ;-)
    // Show values, just for debuging
    Serial.println(""); Serial.print("Temperature: "); Serial.println(t); Serial.print("Humidity: "); Serial.println(h);

    // Report the values gathered by the sensor to the Circus
    circusESP32.write(temperature_key,t,token); // Report the temperature measured to Circus.
    circusESP32.write(humidity_key,h,token); // Report the humidity measured to Circus.
}
