
//Libraries for LoRa device
#include <SPI.h>
#include <LoRa.h>

//Libraries for the Display
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Libraries for BME280
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

//define pins of the Lora Reciver/Sender
#define SCK 5
#define MISO 19
#define MOSI 27
#define SS 18
#define RST 14
#define DIO0 26

//866E6 for Europe (only up to 25mw)
//915E6 for North America
//433E6 for Asia
#define BAND 433E6

//BME280 definition
#define SDA 21
#define SCL 22

//OLED
#define OLED_SDA 4
#define OLED_SCL 15 
#define OLED_RST 16
#define SCREEN_WIDTH 128 
#define SCREEN_HEIGHT 64

TwoWire I2Cone = TwoWire(1);
Adafruit_BME280 bme;

//packet count
int readingID = 0;

int counter = 0;
String LoRaMessage = "";

float temperature = 0;
float humidity = 0;
float pressure = 0;

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RST);

//Initialize OLED display
void startOLED(){
  //reset OLED display via software
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, LOW);
  delay(20);
  digitalWrite(OLED_RST, HIGH);

  //initialize OLED
  Wire.begin(OLED_SDA, OLED_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3c, false, false)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(0,0);
  display.print("LORA SENDER");
}

//Initialize LoRa module
void startLoRA(){
  //SPI LoRa pins
  SPI.begin(SCK, MISO, MOSI, SS);
  //setup LoRa transceiver module
  LoRa.setPins(SS, RST, DIO0);
  LoRa.setTxPower(20);

  while (!LoRa.begin(BAND) && counter < 10) {
    Serial.print(".");
    counter++;
    delay(500);
  }
  if (counter == 10) {
    // Increment readingID on every new reading
    readingID++;
    Serial.println("Starting LoRa failed!"); 
  }
  Serial.println("LoRa Initialization OK!");
  display.setCursor(0,10);
  display.clearDisplay();
  display.print("LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void startBME(){
  I2Cone.begin(SDA, SCL, 100000); 
  bool status1 = bme.begin(0x76, &I2Cone);  
  if (!status1) {
    Serial.println("Could not find a valid BME280_1 sensor, check wiring!");
    while (1);
  }
}

void getReadings(){
  temperature = bme.readTemperature();
  humidity = bme.readHumidity();
  pressure = bme.readPressure() / 100.0F;
}
//Main oled display part
void sendReadings() {
  LoRaMessage = String(readingID) + "/" + String(temperature) + "&" + String(humidity) + "#" + String(pressure);
  //Send LoRa packet to receiver
  LoRa.beginPacket();
  LoRa.print(LoRaMessage);
  LoRa.endPacket();
  
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.print("LoRa packet sent!");
  display.setCursor(0,20);
  display.print("Temperature:");
  display.setCursor(72,20);
  display.print(temperature);
  display.setCursor(0,30);
  display.print("Humidity:");
  display.setCursor(54,30);
  display.print(humidity);
  display.setCursor(0,40);
  display.print("Pressure:");
  display.setCursor(54,40);
  display.print(pressure);
  display.setCursor(0,50);
  display.print("Reading ID:");
  display.setCursor(66,50);
  display.print(readingID);
  display.display();
  Serial.print("Sending packet: ");
  Serial.println(readingID);
  readingID++;
}

void setup() {
  //initialize Serial Monitor
  Serial.begin(115200);
  startOLED();
  startBME();
  startLoRA();
}
void loop() {
  getReadings();
  sendReadings();
  delay(10000); //sends Readings every 10 seconds 
}
