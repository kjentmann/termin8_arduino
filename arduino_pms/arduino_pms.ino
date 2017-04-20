//----------------------------------------//
//-----------Termin-8 Projects-----.------//
//----------------------------------------//
//----Hardcore plant management system----//
//----------------------------------------//
//------------------PMS-------------------//
//----------------------------------------//



#include <dht.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <SoftwareSerial.h>



#define DHT11_PIN 2
byte humiditySensorApin = A0;
byte humiditySensorDpin = 3;
byte waterPumpRelayPin=6;
byte bTx=10;
byte bRx=11;

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);  // Set the LCD I2C address
SoftwareSerial BT(bTx,bRx);
dht DHT;


int currentAirTemp;
int currentAirHumid;
int currentSoilHumid;
 

void setup() {

  //LCD Setup
  lcd.begin(16,2);
  lcd.backlight();
  lcd.clear();
  //AirSensor Setup
  pinMode(humiditySensorDpin, OUTPUT);
  pinMode(waterPumpRelayPin, OUTPUT);
  //WaterPump Setup
  digitalWrite(waterPumpRelayPin, HIGH);
  //SoilSensor setup
  digitalWrite(humiditySensorDpin, LOW);
 //Bluetooth tranceiver setup
  BT.begin(9600);
  Serial.begin(9600);
 
}
int read_humidity_sensor() {
  digitalWrite(humiditySensorDpin, HIGH);
  delay(500);
  int value = analogRead(humiditySensorApin);
  digitalWrite(humiditySensorDpin, LOW);
  return 1023 - value;
}

void updateLCD(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("AirT:" + String(currentAirTemp));
  lcd.print(" AirH:" + String(currentAirHumid));
  lcd.setCursor(0,1);
  lcd.print("SoilHumidity:" + String(currentSoilHumid));
  //lcd.print(String(currentAirTemp));

  
}

void waterPlant(double duration){ //sec
 digitalWrite(waterPumpRelayPin, LOW);
 delay( duration * 1000);
 digitalWrite(waterPumpRelayPin, HIGH);
}

String data ="";
void bluetoothSync(){
   while (BT.available())
    {
        char character = BT.read(); // Receive a single character from the software serial port
        data.concat(character); // Add the received character to the receive buffer
        if (character == '\n')
        {
          
  
              
        if (data[0]=='1'){
            digitalWrite(waterPumpRelayPin, LOW);
            BT.println("Water pump on");
        }
        else if (data[0]=='2'){
          digitalWrite(waterPumpRelayPin, HIGH);
          BT.println("Water pump off");
        }
        else if (data[0]=='3'){
          int duration = data[1]-48; //whooops max 9 sec
          BT.println("Watering for ");
          BT.println(duration);
          if (duration>0){
            waterPlant(duration);
          }
          else{
            BT.println("Error! non valid duration");  
          }
      }
    else if (data[0]=='?')
    {
      BT.println("Send '1' to turn water pump on");
      BT.println("Send '2' to turn water pump off");
    }
    else{
         BT.println("Unknown command. Received: ");
         BT.println(data);
    }

        
            // Clear buffer
            data = "";
        }
    }
  
}


void loop()
{
  int chk = DHT.read11(DHT11_PIN);
  currentAirTemp = DHT.temperature;
  currentAirHumid = DHT.humidity;
  currentSoilHumid =read_humidity_sensor();
  updateLCD();
  bluetoothSync();
}


  //Serial.print("Air temperature = ");
  //Serial.println(currentAirTemp);
  //Serial.print("Air humidity = ");
  //Serial.println(currentAirHumid);
   // Serial.print("Humidity Level (0-1023): ");
  // 1023 from specs, while highest walue from MADSOFT labs is measured to 750
  //Serial.println(read_humidity_sensor());

 
   


