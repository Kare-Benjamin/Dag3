/*
Author: Kåre-Benjamin Hammervold Rørvik 8.9.2020
On behald of NTNU.
Edited 19.10.2020

Et oppsett med BME 280 sensor koblet via I2C til ESP32.
Sketchen er testet med variantene: ESP32 DEV MODULE og DOIT ESP-32 DEVKIT V1
Sensordata skrives ut i konsollen (serial monitor).

Pinout:

BME280       |     ESP-32
----------------------------------
SCL/SCK:     |     D22
SDA/DATA:    |     D21
VDD:         |     3V3 (3.3V)
GND:         |     GND

Det er anbefalt å koble opp en kondensator (avkoppling) mellom GND og VDD på sensorer. F.eks.: 0.1uF keramisk kondensator.
 */
 
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//Brukes til å telle antall målinger som er gjort uten restart.
int measureCounter;

// Lager et Adafruit_BME280 objekt kalt bme280.  
// Slik får vi tilgang på funksjonene tilhørende objektet.
Adafruit_BME280 bme280;

void setup() {
  //115200 er anbefalt Baud rate for ESP32
  Serial.begin(115200);
  //Sjekker sensoren, se funksjonsbeskrivelsen lengre ned.
  checkBME280();
}


void loop() { 
  //Leser av sensoren og skriver til monitor, se funksjonsbeskrivelsen lengre ned.
  measureCounter = sensorValuesToMonitor(measureCounter);
  
  delay(2000);
}


int sensorValuesToMonitor(int successfullMeassures) {
  //bem280 objektet har flere funksjoner, blant annet for temperature, trykk og fuktighet.
  //Disse verdiene lagres lokalt i variabler og blir skrevet ut til monitor.
  //Sjekker om tall er i gydlighetsområdet, hvis ikke kjøres en restart.

  //Flyttall (float) tillater desimaltall i avlesningen (to desimaler). 
  float temperature = bme280.readTemperature();
  float preassure = bme280.readPressure()/ 100.0F;
  float humidity = bme280.readHumidity();

  successfullMeassures ++;
  
  Serial.println("---------------------");
  Serial.print("Antall målinger på rad: ");
  Serial.println(successfullMeassures);
  
  
  Serial.print("Temperatur: ");
  Serial.print(temperature);
  Serial.println(" *C");
  
  Serial.print("Trykk: ");
  Serial.print(preassure);
  Serial.println(" hPa");

  Serial.print("Fuktighet: ");
  Serial.print(humidity);
  Serial.println(" %");

  Serial.println("---------------------");
  Serial.println();

  if(((temperature < -30 || humidity < -30) || temperature  > 100 || humidity > 100)){
    Serial.print("Verdier er utenfor gydlighetsområdet, det antas en error, en restart påfølger om 5 sekunder");
      delay(5000);
      ESP.restart();
  }
  return successfullMeassures;
}


void checkBME280() {
  // En TRUE/FALSE variabel (bool) som sjekker om sensoren er detektert og oppfører seg som forventet. 
  // bme280.begin tar inn addressen på I2C buss (0x76) og sjekker på denne addressen. 
  bool readyCheck = bme280.begin(0x76); 
  
  //Sjekker om sensoren er klar, hvis ikke henger systemet i while og sjekker om sensoren blir detektert. 
  if (!readyCheck) {
    Serial.println("BME 280 er ikke funnet, se over skjematikken og koden");
    Serial.println("Sjekk at: SCL/SCK er koblet til D22 og SDA/DATA på D21");
    Serial.println("Programmet resetartes om sensoren blir riktig detektert");
    while (1){
      // Står i loop...
      // Men om sensoren detekteres restartes programmet.
      readyCheck = bme280.begin(0x76);
      // Et delay kan ofte være greit for å unnga timing problemer, og unødvendig prossesering. 
      // Ulike størrelser kan testes.
      delay(1000); 
      if(readyCheck){
        ESP.restart();
        }    
    }
  }
  Serial.println("BME 280 er klar og fungerer som forventet");
}
