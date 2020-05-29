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
int photocellReading;  // Here we will place our reading
int temperatureRound;

LiquidCrystal_I2C lcd(0x3F, 20, 4); // set the LCD address to 0x27 for a 16 chars and 2 line display

//timer object
SimpleTimer timer;

int ledPin = 6;

byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// Enter the IP address for Arduino, as mentioned we will use 192.168.0.16
// Be careful to use , insetead of . when you enter the address here
IPAddress ip(192, 168, 0, 40);

char server[] = "192.168.0.39"; // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie. "www.yourwebpage.com")

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

  pinMode(ledPin, OUTPUT);

  lcd.backlight();
  lcd.init();

  dht.begin();

  timer.setInterval(30000, TimerTest);

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

  sendDataToDB();

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

  temperatureRound = (hic);
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

void sendDataToDB()
{
  // Connect to the server (your computer or web page)
  if (client.connect(server, 80)) {
    client.print("GET /website/sendDataToDB.php?"); // This
    client.print("waterTemp="); // This
    client.print(photocellReading); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.print("&airTemp="); // This
    client.print(temperatureRound); // And this is what we did in the testing section above. We are making a GET request just like we would from our browser but now with live data from the sensor
    client.println(" HTTP/1.1"); // Part of the GET request
    client.println("Host: 192.168.0.39"); // IMPORTANT: If you are using XAMPP you will have to find out the IP address of your computer and put it here (it is explained in previous article). If you have a web page, enter its address (ie.Host: "www.yourwebpage.com")
    client.println("Connection: close"); // Part of the GET request telling the server that we are over transmitting the message
    client.println(); // Empty line
    client.println(); // Empty line
    client.stop();    // Closing connection to server
  }
  else {
    // If Arduino can't connect to the server (your computer or web page)
    Serial.println("--> connection failed\n");
  }

}
