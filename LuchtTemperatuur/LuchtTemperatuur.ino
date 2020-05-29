#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A0     
#define DHTTYPE DHT11  
DHT dht(DHTPIN, DHTTYPE);

#include <SimpleTimer.h>

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display

//timer object
SimpleTimer timer;

int ledPin = 6; 


void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
    // Print a message to the LCD.
  lcd.backlight();
  lcd.init();                      // initialize the lcd 
  
  dht.begin();  
  
  timer.setInterval(10000, TimerTest);

  
}

void loop()
{
    timer.run();
}


void TimerTest()
{
  //checkt de data van de sensor en zet deze in de serial monitor
  //dit doet hij elke 10 seconden door de timer die ik heb ingesteld. 
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  
  float hic = dht.computeHeatIndex(t, h, false);
  
  lcd.setCursor(0,0);
  lcd.print("Temperatuur:");
  lcd.setCursor(12,0);
  lcd.print(hic);

  lcd.setCursor(0,1);
  lcd.print("Humidity:"); 
  lcd.setCursor(12,1);
  lcd.print(h);

  if(digitalRead(ledPin) == HIGH){
    digitalWrite(ledPin, LOW);
  } else{
    digitalWrite(ledPin, HIGH);
  }
}
