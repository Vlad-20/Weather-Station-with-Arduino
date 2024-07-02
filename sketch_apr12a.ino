#include <Wire.h> //Library for I2C communication
#include <Adafruit_BMP280.h> //Library for BMP280 sensor
#include <SimpleDHT.h> //Library for DHT11 sensor
#include <U8glib.h> //Library for OLED display

//Define the width and height of the OLED display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Initialize the OLED display object
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);  // I2C / TWI

//Initialize the BMP280 sensor & DHT11 sensor objects
Adafruit_BMP280 bmp;
SimpleDHT11 dht11;

void setup() {
  //Begin serial communication at 96000 rate
  Serial.begin(9600);
  while (!Serial) delay(100); //wait for the serial communication to be ready

  //Initialize BMP280 sensor
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or try a different address!"));
    while (1) delay(10); //infinite loop if sensor initialization fails
  }

  u8g.setFont(u8g_font_unifont); //set the font for the display
}

void loop() {
  int temperature, humidity, pressure, altitude;

  // Read BMP280
  pressure = bmp.readPressure() / 133.3; // Convert to mmHg
  altitude = bmp.readAltitude(1013.25); // Adjusted to local forecast

  // Read DHT11
  byte temp, hum;
  byte data[40] = {0};
  if (dht11.read(2, &temp, &hum, data)) {
    Serial.println("Read DHT11 failed");
    return; //exit loop if reading fails
  }
  temperature = (int)temp; //convert byte to int for temp
  humidity = (int)hum; //convert byte to int for humidity

  // Draw on OLED
  u8g.firstPage();
  do {
    //Set font and draw labels on the display
    u8g.setFont(u8g_font_unifont);
    u8g.drawStr(0, 15, "Temp:");
    u8g.drawStr(0, 30, "Humidity:");
    u8g.drawStr(0, 45, "Pressure:");
    u8g.drawStr(0, 60, "Altitude:");

    //Set font and draw the sensor readings on the display
    u8g.setFont(u8g_font_fixed_v0r);
    u8g.setPrintPos(80, 15);
    u8g.print(temperature);
    u8g.drawStr(105, 15, "*C");
    u8g.setPrintPos(80, 30);
    u8g.print(humidity);
    u8g.drawStr(105, 30, "%");
    u8g.setPrintPos(80, 45);
    u8g.print(pressure);
    u8g.drawStr(105, 45, "mmHg");
    u8g.setPrintPos(80, 60);
    u8g.print(altitude);
    u8g.drawStr(105, 60, "m");
  } while (u8g.nextPage()); //continue drawing until all pages are done

  delay(2000); //delay for 2 seconds before the next reading
}
