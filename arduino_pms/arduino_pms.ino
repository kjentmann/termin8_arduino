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
 
# define MAX_WATER_DURATION 60

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
 
   while (BT.available()){
     
        char character = BT.read(); // Receive a single character from the software serial port
           int charr= character;
           BT.println(charr); //RAW data control
          data.concat(character); // Add the received character to the receive buffer
        if (character == '\n')
        {
         int lenght = data.length()-3;  
  
        if (lenght <=2){  
        switch (data[0]){
        
        case '1':  
       
            //digitalWrite(waterPumpRelayPin, LOW);  // Disable for safety reasons
            BT.println("Water pump on");
        //}
        break;
        case '2':
        
          //digitalWrite(waterPumpRelayPin, HIGH);  // Disable for safety reasons
          BT.println("Water pump off");
        break;
        
        case '?': 
          BT.println("Send '1' to turn water pump on");
          BT.println("Send '2' to turn water pump off");
        
        break;
        
        case '3':
          int duration; 
          char buffer[2];
          BT.println("lenght of data:");
          BT.println(lenght);
          if (lenght ==1){  
            duration = data[1]-48;
            BT.println("a quick one!");  
          }
          else if (lenght ==2){
            buffer[0]=data[1];
            buffer[1]=data[2];
            buffer[2]= '\0';
            duration = atoi(buffer);
            BT.println("wow, thirsty one!");
            BT.println(duration);
          }
                    
        if (duration>0 && duration < MAX_WATER_DURATION){
          BT.println("Watering for num sec:");
          BT.println(duration);
          waterPlant(duration);
        }
        else{
          BT.println("Error! non valid duration");  
        }
      break; 
      }
    }
    else{
      BT.println("Unknown command. Received: ");
      BT.println(data);
    }
    data = "";// Clear buffer
    BT.println("\nTermin8\n");
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

  //Serial prints if no lcd in the hood.
  
  //Serial.print("Air temperature = ");
  //Serial.println(currentAirTemp);
  //Serial.print("Air humidity = ");
  //Serial.println(currentAirHumid);
  // Serial.print("Humidity Level (0-1023): ");
  // 1023 from specs, while highest walue from MADSOFT labs is measured to 750
  //Serial.println(read_humidity_sensor());
