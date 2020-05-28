#include <dht.h>
#include <SimpleTimer.h>

//timer object
SimpleTimer timer;

dht DHT;

#define DHT11_PIN 7
int ledPin = 6; 


void setup(){
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  
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
  
  int chk = DHT.read11(DHT11_PIN);  
  Serial.print("Temperature = ");
  Serial.println(DHT.temperature);
  Serial.print("Humidity = ");
  Serial.println(DHT.humidity);

  //ledje gaat elke 10 seconden aan en na 10 seconden weer uit
  //dit is voor visualisatie zodat ik kan zien of de timer werkt
  if(digitalRead(ledPin) == HIGH){
  digitalWrite(ledPin, LOW);
  } else{
  digitalWrite(ledPin, HIGH);
  }
}
