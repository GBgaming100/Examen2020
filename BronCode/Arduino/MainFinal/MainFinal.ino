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
int LedPinR = 6;   //RGB pin out Roodj
int LedPinG = 5;   //RGB pin out Groen
int ledProgress = 4;   //RGB pin out Groen
int LedPinB = 3;   //RGB pin out Blauw
int button = 8; //button voor het wisselen van schaal input

//variabele voor error dectection
bool waterTempActive = true;
bool luchtTempActive = true;
bool networkActive = true;
bool schaalSwitch; //Variable voor het wisselen van Schaal true = fahrenheit, false = celcius
bool  debugLogging;

//variable voor Serial.read
String serialInput;

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

int waterTemp;  //hier komt de waarde uit de sensor
//<--- Eind Water Sensor --->

LiquidCrystal_I2C lcd(0x3F, 20, 4); //zet het adres van de display en de grootte van het display

//timer object
SimpleTimer timer;

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
  Serial.println("Setup Start");
  initGlobals();//initialis alle globale variable
  lcdSetup();//laat een lcd demo zien
  Serial.println("Initialise sensors Start");
  meetLuchtTemp();
  meetWaterTemp();
  Serial.println("Initialise sensors End");
  Serial.println("Setup End");
}

void loop()
{
  //zet de timer aan
  timer.run();
}

void initGlobals()
{
  Serial.println("InitGlobals Start");
  //zet de schaalSwitch op de laats opgeslagen value van de EEPROM
  schaalSwitch = EEPROM.read(1);
  if ( schaalSwitch = true) {
    digitalWrite(ledSchaal, HIGH);
  } else {
    digitalWrite(ledSchaal, LOW);
  }
  //pin input en outputs
  pinMode(LedPinG, OUTPUT);
  pinMode(LedPinB, OUTPUT);
  pinMode(LedPinR, OUTPUT);
  pinMode(ledSchaal, OUTPUT);
  pinMode(ledProgress, OUTPUT);
  pinMode(button, INPUT);

  //lcd aanzetten en initialiseren
  lcd.backlight();
  lcd.init();

  //start de dht sensor
  dht.begin();

  //zet de timer op een interval van 5 seconden
  timer.setInterval(2500, MainTimer);

  //begint de water temperatuur sensor
  sensors.begin();

  // start de Ethernet connectie
  Ethernet.begin(mac, ip);
  Serial.println("InitGlobals End");
}

//functie voor een demo van het lcd scherm en de eerste initialisering
void lcdSetup()
{
  Serial.println("LCDSetup Start");
  lcd.setCursor(0, 0);
  lcd.print("Temperatuur meter");
  lcd.setCursor(0, 1);
  lcd.print("Versie: 2.7");
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

  Serial.println("LCDSetup End");
}


void MainTimer()
{
  schaalSwitch = EEPROM.read(1);
  ButtonPressed();
  meetLuchtTemp();
  meetWaterTemp();
  sendDataToDB();
  ledStatus();
  DebugOn();

  if (digitalRead(ledProgress) == LOW) {
    digitalWrite(ledProgress, HIGH);
  } else {
    digitalWrite(ledProgress, LOW);
  }
}

//functie voor het meten en laten zien op de display van de lucht temperatuur
void meetLuchtTemp()
{
  DebugLog("Lucht Sensor Read Start", 2);
  //lokale variabele die alleen in deze functie worden gebruikt
  float h = dht.readHumidity(); //meet de lucht luchtvochtigheid van de sensor
  DebugLog("luchtvochtigheid:" + String(h), 2);
  float t = dht.readTemperature();//meet de temperatuur van de sensor
  DebugLog("temperatuur:" + String(t), 2);

  float hic = dht.computeHeatIndex(t, h, false); //rekent met de luchtvochtigheid en temperatuur uit wat de gevoelstemperatuur is
  DebugLog("gevoelsTemperatuur:" , 2);
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
    DebugLog("Lucht Sensor Error", 0);
  }
  DebugLog("Lucht Sensor Read End", 2);
  DebugLog("", 4);
}

//functie voor het meten en laten zien op de display van de water temperatuur
void meetWaterTemp()
{

  DebugLog("Water Sensor Read Start", 2);
  //stuurt het command om de sensor waarde te krijgen
  sensors.requestTemperatures();
  //leest de eerste sensor uit en slaat de waarde op in waterTemp
  waterTemp = sensors.getTempCByIndex(0);

    DebugLog("WaterTemp:" + String(waterTemp), 2);

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

    DebugLog("Water sensor Error", 0);
    lcd.setCursor(14, 2);
    lcd.print("Error");
  }
  DebugLog("Water Sensor Read End", 2);
  DebugLog("", 4);
}

//fucntie voor de data naar de database te sturen via het ethernet shield
void sendDataToDB()
{
  DebugLog("sendDataToDB start", 2);
  // start de Ethernet connectie:
  //  Serial.println("Initialize Ethernet with DHCP:");
  DebugLog("Initialize Ethernet with DHCP:" , 2);
  if (Ethernet.begin(mac) == 0) {
    DebugLog("Failed to configure Ethernet using DHCP" , 0);
    // kijkt of het Ethernet shield correct is geinstalleerd
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      DebugLog("Ethernet shield was not found", 0);
      lcd.setCursor(0, 3);
      lcd.print("Shield Error");
      networkActive = false;
    }
    // kijkt of er en Ethernet kabel is aangelosten
    if (Ethernet.linkStatus() == LinkOFF) {
      DebugLog("Ethernet cable is not connected" , 0);
      lcd.setCursor(0, 3);
      lcd.print("Ethernet cable Error");
      networkActive = false;
    }
  }
  // Ethernet shield heeft tijd nodig voor het initialiseren
  delay(1000);
  DebugLog("Connecting to " + String(server), 2);

  // als er een connectie is, koppel dat terug via serial
  if (client.connect(server, 80)) {

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
    DebugLog("Connection was succes", 2);
  } else {
    // als er geen connectie is laat de error zien
    DebugLog("Connection Failed" ,  0);
    lcd.setCursor(0, 3);
    lcd.print("Connection failed");
    networkActive = false;
  }
  DebugLog("sendDataToDB END", 2);
  DebugLog("",4);
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
  if (digitalRead(button) == HIGH) { // if button is pressed
    if (schaalSwitch == true) {           // and the status flag is LOW
      schaalSwitch = false;                // make status flag HIGH
      Serial.println("Writes 1 to EEPROm");
      EEPROM.write(1, 0);
      digitalWrite(ledSchaal, HIGH);
    }                           //
    else {                        // otherwise...
      schaalSwitch = true;                // make status flag LOW
      Serial.println("Writes 2 to EEPROm");
      EEPROM.write(1, 1);
      digitalWrite(ledSchaal, LOW);
    }
    delay(1000);                    // wait a sec for the
  }                               // hardware to stabilize
}

void DebugOn() {
  if (Serial.available()) {
    serialInput = Serial.readStringUntil('\n');
    Serial.println(serialInput);
    if (serialInput == "Debug activate") {
      debugLogging = true;
      DebugLog("Debug is activated", 2);
    }
  }

}

void DebugLog(String a_text, int a_infoType) {
  if (debugLogging == true) {
    switch (a_infoType) {
      case 0:
        a_text = "ERROR: " + a_text;
        break;
      case 1:
        a_text = "Warning: " + a_text;
        break;
      case 2:
        a_text = "Info: " + a_text;
        break;
      case 3:
        a_text = "Alert: " + a_text;
        break;
      case 4:
        a_text = "";
        break;
    }
    Serial.println(a_text);
  }
}
