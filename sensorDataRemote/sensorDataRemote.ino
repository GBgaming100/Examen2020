/*
  ArduinoPoolMonitor

  Dit programma is gemaakt voor mijn eind examen op het ROC ter AA in helmond.
  Het doel van het programma is dat er 2 sensoren worden uitgelezen en verzonden worden
  naar een webserver. de arduino heeft 2 sensoren erop zitten deze zijn: dHT11 en DS18B20.
  verder zit er een Ethernet shield en lcd display op. deze zijn voor het versturen en visualiseren
  van de data.

  Gemaakt Door:
  Max van den Boom
  Date:
  4-6-2020

  http://amxdev.nl/
*/

//<--- Begin libraries --->
//lucht temperatuur libary
#include <DHT.h>
#include <DHT_U.h>
//Ethernet library
#include <Ethernet.h>
//Water temperatuur library
#include <OneWire.h>
#include <DallasTemperature.h>
//EEPROM library
#include <EEPROM.h>
//Timer library
#include <SimpleTimer.h>
//LCD display library
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
//<--- Eind libraries --->

//<--- Begin Variabelen --->
int ledSchaal = 7; //pin out Led voor schaal
int LedPinR = 6;   //RGB pin out Rood
int LedPinG = 5;   //RGB pin out Groen
int LedPinB = 3;   //RGB pin out Blauw
int button = 8; //button voor het wisselen van schaal input

//variabele voor error dectection
bool waterTempActive = true;
bool luchtTempActive = true;
bool networkActive = true;
bool schaalSwitch; //Variable voor het wisselen van Schaal true = fahrenheit, false = celcius

//<--- Begin Lucht Sensor --->
#define DHTPIN A0 //pin out voor lucht sensor
#define DHTTYPE DHT11 //bepaald welk type sensor ik heb in dit geval de DHT11
DHT dht(DHTPIN, DHTTYPE);
int temperatureRound; //variable om te lucht waarde af te ronden naar een heel getal
//<--- Eind Lucht Sensor --->

//<--- Begin Water Sensor --->
#define ONE_WIRE_BUS 2
// Setup a oneWire instance to communicate with any OneWire devices
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS); //maakt een one wire instantie aan om te kunnen communiceren met de library

DallasTemperature sensors(&oneWire);//laat de library weten welke onewire instance hij moet gebruiken

int photocellPin = 0;  //pin out voor water temperatuur
int waterTemp;  //hier komt de waarde uit de sensor
//<--- Eind Water Sensor --->

LiquidCrystal_I2C lcd(0x3F, 20, 4); //zet het adres van de display en de grootte van het display

SimpleTimer timer; //timer object

//<--- Begin web server --->
//mac adres van de arduino
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

//het lokale ip die de arduino opzich gaat nemen
IPAddress ip(192, 168, 0, 40);

//De webserver die de arduino gaat proberen te bereiken
char server[] = "www.amxdev.nl";

//Start een Web client
EthernetClient client;
//<--- Eind web server --->

void setup() {

  Serial.begin(9600);

  initGlobals();//initialis alle globale variable
  lcdSetup();//laat een lcd demo zien
  meetLuchtTemp();
  meetWaterTemp();
}

void loop()
{
  //zet de timer aan
  timer.run();
}

void initGlobals()
{
  Serial.println("Init Globals...");

  //pin input en outputs
  pinMode(LedPinG, OUTPUT);
  pinMode(LedPinB, OUTPUT);
  pinMode(LedPinR, OUTPUT);
  pinMode(ledSchaal, OUTPUT);
  pinMode(button, INPUT);

  //lcd aanzetten en initialiseren
  lcd.backlight();
  lcd.init();

  //start de dht sensor
  dht.begin();

  //zet de timer op een interval van 5 seconden
  timer.setInterval(5000, MainTimer);

  //begint de water temperatuur sensor
  sensors.begin();

  // start de Ethernet connectie
  Ethernet.begin(mac, ip);

  //zet de schaalSwitch op de laats opgeslagen value van de EEPROM
  schaalSwitch = EEPROM.read(1);

  Serial.println("Init finished");
}

//functie voor een demo van het lcd scherm en de eerste initialisering
void lcdSetup()
{
  Serial.println("Start lcd Demo");
  lcd.setCursor(0, 0);
  lcd.print("Temperatuur meter");
  lcd.setCursor(0, 1);
  lcd.print("Versie: 2.4");
  lcd.setCursor(0, 2);
  lcd.print("Gemaakt door:");
  lcd.setCursor(0, 3);
  lcd.print("Max van den Boom");
  delay(3000);
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.setCursor(0, 0);
  lcd.print("Temperatuur");

  lcd.setCursor(0, 2);
  lcd.print("Het water is:");

  lcd.setCursor(0, 1);
  lcd.print("De lucht is:");

  Serial.println("Finished");
}


void MainTimer()
{
  ButtonPressed();
  meetLuchtTemp();
  meetWaterTemp();
  sendDataToDB();
  ledStatus();
}

//functie voor het meten en laten zien op de display van de lucht temperatuur
void meetLuchtTemp()
{
  //lokale variabele die alleen in deze functie worden gebruikt
  float h = dht.readHumidity(); //meet de lucht luchtvochtigheid van de sensor
  float t = dht.readTemperature();//meet de temperatuur van de sensor

  float hic = dht.computeHeatIndex(t, h, false); //rekent met de luchtvochtigheid en temperatuur uit wat de gevoelstemperatuur is

  //als de sensor tussen de minimale en maximale waarde zit dan werkt hij correct
  if (hic > 0 && hic < 51) {
    //als schaal switch false is dan laat hij Celcius zien
    if ( schaalSwitch == false) {
      //rond de gevoels temperatuur af op een heel getal
      temperatureRound = (hic);

      // laat lucht temperatuur zien op display
      lcd.setCursor(14, 1);
      lcd.print(temperatureRound);

      lcd.setCursor(16, 1);
      lcd.print("C   ");
    } else {
      //verandert de gevoels temperatuur van Celcius naar Fahrenheit
      temperatureRound = 1.8 * (hic) + 32;
      //rond de gevoels temperatuur af op een heel getal
      temperatureRound = (temperatureRound);
      // laat lucht temperatuur zien op display
      lcd.setCursor(14, 1);
      lcd.print(temperatureRound);

      lcd.setCursor(16, 1);
      lcd.print("F   ");
    }
    luchtTempActive = true;
  } else {
    //als de sensor niet actief is laat hij op het display een Error messeage zien en gaat het rgb ledje op oranje
    luchtTempActive = false;
    lcd.setCursor(14, 1);
    lcd.print("Error");
  }
}

//functie voor het meten en laten zien op de display van de water temperatuur
void meetWaterTemp()
{
  //stuurt het command om de sensor waarde te krijgen
  sensors.requestTemperatures();
  //leest de eerste sensor uit en slaat de waarde op in waterTemp
  waterTemp = sensors.getTempCByIndex(0);

  //als de sensor tussen de minimale en maximale waarde zit dan werkt hij correct
  if (waterTemp > -55 && waterTemp < 125) {
    //als schaal switch false is dan laat hij Celcius zien
    if ( schaalSwitch == false) {
      //laat water tempratuur zien op display
      lcd.setCursor(14, 2);
      lcd.print(waterTemp);
      lcd.setCursor(16, 2);
      lcd.print("C   ");
    } else {
      waterTemp = 1.8 * waterTemp + 32;
      lcd.setCursor(14, 2);
      lcd.print(waterTemp);
      lcd.setCursor(16, 2);
      lcd.print("F   ");
    }
    waterTempActive = true;
  } else if (waterTemp == -127) {
    //als de sensor niet actief is laat hij op het display een Error messeage zien en gaat het rgb ledje op oranje
    waterTempActive = false;

    lcd.setCursor(14, 2);
    lcd.print("Error");
  }
}

//fucntie voor de data naar de database te sturen via het ethernet shield
void sendDataToDB()
{
  // start de Ethernet connectie:
  //  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // kijkt of het Ethernet shield correct is geinstalleerd
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      lcd.setCursor(0, 3);
      lcd.print("Shield Error");
      networkActive = false;
    }
    // kijkt of er en Ethernet kabel is aangelosten
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
      lcd.setCursor(0, 3);
      lcd.print("Ethernet cable Error");
      networkActive = false;
    }
  } else {
    // DHCP settings
    //    Serial.print("  DHCP assigned IP ");
    //    Serial.println(Ethernet.localIP());
  }
  // Ethernet shield heeft tijd nodig voor het initialiseren 
  delay(1000);
  
  //  Serial.print("connecting to ");
  //  Serial.print(server);
  //  Serial.println("...");

  // als er een connectie is, koppel dat terug via serial
  if (client.connect(server, 80)) {
    // connection info
    //    Serial.print("connected to ");
    //    Serial.println(client.remoteIP());

    //als het netwerk oke is display op lcd
    lcd.setCursor(0, 3);
    lcd.print("Network Ok");
    networkActive = true;
    // Maakt een HTTP request naar www.amxdev.nl met de values van de sensoren in de POST
    client.print("GET /sendDataToDB.php?"); // This
    client.print("waterTemp="); // This
    client.print(waterTemp);
    client.print("&airTemp="); // This
    client.print(temperatureRound);
    client.print("&schaalSwitch="); // This
    client.print(schaalSwitch);
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: www.amxdev.nl");
    client.println("Connection: close");
    client.println();
  } else {
    // als er geen connectie is laat de error zien 
    Serial.println("connection failed");
    lcd.setCursor(0, 3);
    lcd.print("Connection failed");
    networkActive = false;
  }
}

// kijkt elke 5 seconden of 1 van de sensoren niet goed werkt.
// als 1 van die sensoren niet goed werkt gaat er een oranje lampje aan
// als alle 3 de factoren fout zijn gaat het rode ledje aan. 
void ledStatus()
{

  String ledStatus;

  if ( waterTempActive == true && luchtTempActive == true &&  networkActive == true)
  {
    ledStatus = "groen";
  } else if ( waterTempActive == false || luchtTempActive == false ||  networkActive == false)
  {
    ledStatus = "oranje";
  }
  if ( waterTempActive == false && luchtTempActive == false &&  networkActive == false)
  {
    ledStatus = "rood";
  }

  if (ledStatus == "rood")
  {
    analogWrite(LedPinR, 255);
    analogWrite(LedPinG, 0);
    analogWrite(LedPinB, 0);
  }
  else if ( ledStatus == "groen")
  {
    analogWrite(LedPinR, 0);
    analogWrite(LedPinG, 255);
    analogWrite(LedPinB, 0);
  }
  else if (ledStatus == "oranje")
  {
    analogWrite(LedPinR, 255);
    analogWrite(LedPinG, 127);
    analogWrite(LedPinB, 0);
  }
}

// als de button wordt ingedrukt switch hij schaalSwitch naar true of false
// dit bepaald of het faherenheit of celcius is. 
// in deze functie slaat hij ook wanneer de knop ingedrukt is de value op in de EEPROM
// zodat bij startup de laatste waarde wordt gebruikt
void ButtonPressed() {

  if (schaalSwitch == true) {
    digitalWrite(ledSchaal, HIGH);
  } else {
    digitalWrite(ledSchaal, LOW);
  }
  
  if (digitalRead(button) == HIGH) { // if button is pressed
    if (schaalSwitch == true) {           // and the status flag is LOW
      schaalSwitch = false;                // make status flag HIGH
      Serial.println("Writes 1 to EEPROm");
      EEPROM.write(1, 0);
    }                           //
    else {                        // otherwise...
      schaalSwitch = true;                // make status flag LOW
      Serial.println("Writes 2 to EEPROm");
      EEPROM.write(1, 1);
    }
    delay(1000);                    // wait a sec for the
  }                               // hardware to stabilize
}
