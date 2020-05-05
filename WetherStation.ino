/*
 Name:		WetherStation.ino
 Created:	3/29/2020 9:16:50 PM
 Author:	raynor
*/

// the setup function runs once when you press reset or power the board
#include <Adafruit_Sensor.h>
#include <DHT_U.h>
#include <DHT.h>

#define DHTPIN   D3     // Pin which is connected to the DHT sensor.
#define DHTTYPE  DHT22 // DHT 22 
DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

#include <SPI.h>
#include <Wire.h>
/// OLED define
#include <Adafruit_GFX.h>  // Include core graphics library for the display
#include <Adafruit_SSD1306.h>  // Include Adafruit_SSD1306 library to drive the display
//Adafruit_SSD1306 display;  // Create display
#include <Fonts/FreeMonoBold12pt7b.h>  // Add a custom font
#include <Fonts/FreeMono9pt7b.h>  // Add a custom font
////NodeMCU OLED Wiring
//OLED VCC – NodeMCU 3.3V
//OLED GND – NodeMCU GND
//OLED SCL – NodeMCU D1
//OLED SDA – NodeMCU D2

#define OLED_RESET 13//LED_BUILTIN  //4
Adafruit_SSD1306 display(OLED_RESET);
//Adafruit_SSD1306 display;

#if (SSD1306_LCDHEIGHT != 64) // To check that OLED display meets the resolution
#error(F("Height incorrect, please fix Adafruit_SSD1306.h!"));
#endif

void setup() {
    Serial.begin(9600);
    dhtSetup();
    setup_OLED();
}

// the loop function runs over and over again until power down or reset
void loop() {
    //float mq2 = read_MQ2();
    //float temp = read_DHT_temp();
    int temp = read_DHT_temp();
    sendDataToOLED(temp, F("C"));
    //OLED_Text(temp, humd);
    //sendToOLED_both(temp, humd);

    int humd = read_DHT_humd();
    sendDataToOLED(humd, F("%"));
}

void dhtSetup() {
    // Initialize device.
    //Serial.println("-- DHT Setup ---");
    dht.begin();
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    dht.humidity().getSensor(&sensor);
    delayMS = sensor.min_delay / 300;
}

//float read_DHT_temp() {
int read_DHT_temp() {
    // Delay between measurements.
    delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    dht.temperature().getEvent(&event);
    if (isnan(event.temperature)) {
        Serial.println(F("Error reading temperature!"));
        return 0.0;
    }
    else {
        //float tempr = event.temperature;
        int tempr = (int)event.temperature;
        Serial.print(F("Temp: ")); Serial.print(tempr, 1); Serial.print(F(" *C; "));
        return tempr;
    }
}

int read_DHT_humd() {
    // Delay between measurements.
    delay(delayMS);
    // Get temperature event and print its value.
    sensors_event_t event;
    // Get humidity event and print its value.
    dht.humidity().getEvent(&event);
    if (isnan(event.relative_humidity)) {
        Serial.println(F("Error reading humidity!"));
        return 0;
    }
    else {
        int humdt = (int)event.relative_humidity;
        Serial.print(F("Humidity: ")); Serial.print(humdt, 1); Serial.print(F(" %"));
        return humdt;
    }
}

void setup_OLED() {
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // Clear the display so we can refresh
    display.clearDisplay();
    display.display();
    display.setRotation(0);
    display.setFont(&FreeMono9pt7b);
    display.setTextSize(3); // Set text size. We are using a custom font so you should always use the text size of 0
    display.setTextColor(WHITE);
}

void sendDataToOLED(int data, String str) {

    display.clearDisplay();
    // Print text:
    display.setCursor(7, 45);  // (x,y)
    display.print(data, 1);  // Text or value to print 
    display.println(str);

    //Serial.print(F("OLED: ")); Serial.print(data, 1); Serial.println(str);
    display.display();    
}

void sendToOLED_both(float tempr, int hmd) {

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    // Clear the display so we can refresh
    display.clearDisplay();
    display.display();
    display.setTextColor(WHITE);    

    display.setTextSize(2);  
    display.setRotation(0);
    display.setFont(&FreeMonoBold12pt7b);
    display.setTextSize(1); // Set text size. We are using a custom font so you should always use the text size of 0
    // Print text:
    display.setCursor(0, 22);  // (x,y)

    display.print("  T ");  // Text or value to print
    display.print(tempr, 0);  // Text or value to print 
    display.println(F(" C"));
    //display.println();
    display.print(F(" RH "));  // Text or value to print
    display.print(hmd, 1);  // Text or value to print 
    display.println(F(" %"));

    display.display();
}
