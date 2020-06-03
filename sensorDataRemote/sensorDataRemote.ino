#include <DHT.h>
#include <DHT_U.h>
//library for ethernet
#include <Ethernet.h>
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
int waterTemp;  // Here we will place our reading
int temperatureRound;

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

//timer object
SimpleTimer timer;

int LedPinR = 6;
int LedPinG = 5;
int LedPinB = 3;

int waterTempActive = true;
int luchtTempActive = true;
int networkActive = true;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// Enter the IP address for Arduino, as mentioned we will use 192.168.0.16
// Be careful to use , insetead of . when you enter the address here
IPAddress ip(192, 168, 0, 40);

char server[] = "www.amxdev.nl"; // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie. "www.yourwebpage.com")

EthernetClient client;


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

  pinMode(LedPinG, OUTPUT);
  pinMode(LedPinB, OUTPUT);
  pinMode(LedPinR, OUTPUT);

  ledStatus();

  lcd.backlight();
  lcd.init();

  dht.begin();

  timer.setInterval(5000, TimerTest);

  sensors.begin();

  // start the Ethernet connection
  Ethernet.begin(mac, ip);


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

  lcd.setCursor(0, 2);
  lcd.print("Het water is:");

  lcd.setCursor(0, 1);
  lcd.print("De lucht is:");

  Serial.println("Finished");
}


void TimerTest()
{

  meetLuchtTemp();
  meetWaterTemp();

  sendDataToDB();

  ledStatus();

  //  if (digitalRead(LedPinG) == HIGH) {
  //    digitalWrite(LedPinG, LOW);
  //  } else {
  //    digitalWrite(LedPinG, HIGH);
  //  }
}

void meetLuchtTemp()
{
  //checkt de data van de sensor en zet deze in de serial monitor
  //dit doet hij elke 10 seconden door de timer die ik heb ingesteld.

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float hic = dht.computeHeatIndex(t, h, false);

  if (hic > 0 && hic < 51) {
    temperatureRound = (hic);

    // laat lucht temperatuur zien op display
    lcd.setCursor(14, 1);
    lcd.print(temperatureRound);

    lcd.setCursor(16, 1);
    lcd.print("C   ");

    luchtTempActive = true;
  } else {
    luchtTempActive = false;
    lcd.setCursor(14, 1);
    lcd.print("Error");
  }
}

void meetWaterTemp()
{
  sensors.requestTemperatures(); // Send the command to get temperatures
  waterTemp = sensors.getTempCByIndex(0);

  //laat water tempratuur zien op display
  lcd.setCursor(14, 2);
  lcd.print(waterTemp);


  if (waterTemp > -55 && waterTemp < 125) {
    //laat water tempratuur zien op display
    lcd.setCursor(14, 2);
    lcd.print(waterTemp);
    lcd.setCursor(16, 2);
    lcd.print("C   ");
    waterTempActive = true;
  } else if (waterTemp == -127) {

    waterTempActive = false;

    lcd.setCursor(14, 2);
    lcd.print("Error");
  }
}

void sendDataToDB()
{
  // Connect to the server (your computer or web page)
  //  if (client.connect(server, 80)) {
  //    client.print("GET /sendDataToDB.php?"); // This
  //    client.print("waterTemp="); // This
  //    client.print(waterTemp); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
  //    client.print("&airTemp="); // This
  //    client.print(temperatureRound); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
  //    client.println(" HTTP/1.1"); // Part of the GET request
  //    client.println("Host: www.amxdev.nl/"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
  //    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
  //    client.println(); // Empty line
  //    client.println(); // Empty line
  //    client.stop();    // Closing connection to server
  //
  //    networkActive = true;
  //    lcd.setCursor(0, 3);
  //    lcd.print("             ");
  //  }
  //  else {
  //    // If Arduino can't connect to the server (your computer or web page)
  //    ("--> connection failed\n");
  //
  //    networkActive = false;
  //
  //    lcd.setCursor(0, 3);
  //    lcd.print("Network Error");
  //  }

  // start the Ethernet connection:
  Serial.println("Initialize Ethernet with DHCP:");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // Check for Ethernet hardware present
    if (Ethernet.hardwareStatus() == EthernetNoHardware) {
      Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
      while (true) {
        delay(1); // do nothing, no point running without Ethernet hardware
      }
    }
    if (Ethernet.linkStatus() == LinkOFF) {
      Serial.println("Ethernet cable is not connected.");
    }
    // try to congifure using IP address instead of DHCP:
    //    Ethernet.begin(mac, ip, myDns);
  } else {
    Serial.print("  DHCP assigned IP ");
    Serial.println(Ethernet.localIP());
  }

  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.print("connecting to ");
  Serial.print(server);
  Serial.println("...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    Serial.print("connected to ");
    Serial.println(client.remoteIP());
    // Make a HTTP request:
    client.print("GET /sendDataToDB.php?"); // This
    client.print("waterTemp="); // This
    client.print(waterTemp); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.print("&airTemp="); // This
    client.print(temperatureRound);
    client.println(" HTTP/1.1"); // Part of the GET request

    client.println("Host: www.amxdev.nl");
    client.println("Connection: close");
    client.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}




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
