#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN A0
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  //checkt de data van de sensor en zet deze in de serial monitor
  //dit doet hij elke 10 seconden door de timer die ik heb ingesteld.

  float h = dht.readHumidity();
  float t = dht.readTemperature();

  float hic = dht.computeHeatIndex(t, h, false);
  
  if (hic > 0 && hic < 51) {

  Serial.println(hic);

  } else{
    Serial.println("error");
  }

  delay (1000);
}
