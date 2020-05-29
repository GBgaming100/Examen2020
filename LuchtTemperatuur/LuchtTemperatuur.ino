#include <DHT.h>
#include <DHT_U.h>

//libraries for water temperature
#include <OneWire.h>
#include <DallasTemperature.h>

#include <SimpleTimer.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

// Data wire is plugged into pin 2 on the Arduino
#define ONE_WIRE_BUS 2

// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

int photocellPin = 0;  // Analog input pin on Arduino we connected the SIG pin from sensor
int photocellReading;  // Here we will place our reading

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

//timer object
SimpleTimer timer;

int ledPin = 6;


void setup() {
  Serial.begin(9600);
  initGlobals();//initialis all globals variables
  lcdSetup();//shows lcd demo
  meetLuchtTemp();
  meetWaterTemp();
}

void loop()
{
  timer.run();
}

void initGlobals()
{
  Serial.println("Init Globals...");

  pinMode(ledPin, OUTPUT);

  lcd.backlight();
  lcd.init();

  dht.begin();

  timer.setInterval(4000, TimerTest);

  sensors.begin();

  Serial.println("Finished");
}

void lcdSetup()
{
  Serial.println("Start lcd Demo");
  lcd.setCursor(0, 0);
  lcd.print("Temperatuur meter");
  lcd.setCursor(0, 1);
  lcd.print("Versie: 1.0");
  lcd.setCursor(0, 2);
  lcd.print("Gemaakt door:");
  lcd.setCursor(0, 3);
  lcd.print("Max van den Boom");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 0);
  lcd.print("Temperatuur");

  lcd.setCursor(0, 1);
  lcd.print("Het water is:");

  lcd.setCursor(16, 1);
  lcd.print("C");

  lcd.setCursor(0, 2);
  lcd.print("De lucht is:");

  lcd.setCursor(16, 2);
  lcd.print("C");
  Serial.println("Finished");
}


void TimerTest()
{

  meetLuchtTemp();
  meetWaterTemp();

  if (digitalRead(ledPin) == HIGH) {
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(ledPin, HIGH);
  }
}

void meetLuchtTemp()
{
  //checkt de data van de sensor en zet deze in de serial monitor
  //dit doet hij elke 10 seconden door de timer die ik heb ingesteld.

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float hic = dht.computeHeatIndex(t, h, false);

  int temperatureRound = (hic);
  // laat lucht temperatuur zien op display
  lcd.setCursor(14, 1);
  lcd.print(temperatureRound);


}

void meetWaterTemp()
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  photocellReading = sensors.getTempCByIndex(0);

  //laat water tempratuur zien op display
  lcd.setCursor(14, 2);
  lcd.print(photocellReading);
}
