#include <dht.h>

dht DHT;

#define DHT11_PIN 2
byte humidity_sensor_pin = A0;
byte humidity_sensor_vcc = 3;

int currentAirTemp;
int currentAirHumid;


void setup() {
  pinMode(humidity_sensor_vcc, OUTPUT);
  digitalWrite(humidity_sensor_vcc, LOW);
  Serial.begin(9600);
}
int read_humidity_sensor() {
  digitalWrite(humidity_sensor_vcc, HIGH);
  delay(500);
  int value = analogRead(humidity_sensor_pin);
  digitalWrite(humidity_sensor_vcc, LOW);
  return 1023 - value;
}

void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  currentAirTemp=DHT.temperature;
  currentAirHumid=DHT.humidity;
  Serial.print("Air temperature = ");
  Serial.println(currentAirTemp);
  Serial.print("Air humidity = ");
  Serial.println(currentAirHumid);

  Serial.print("Humidity Level (0-1023): ");
  Serial.println(read_humidity_sensor()); 
  delay(1000);
}

