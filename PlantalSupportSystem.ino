#include <dht.h>;
  
//Constants
#define DHTPIN A2     //pin we're connected to DHT11(Temp. Humidity Sensor)
dht DHT; // Initialize DHT sensor for normal 16mhz Arduino

//CO2 Sensor
#define co2sensor A0 //pin we're connected to CO2 sensor 

//Liquid Sensor
#define liquidsensor 6 //pin we're connected to liquid sensor 

//Variables
int SoilHumidity; //Stores humidity value in soil
int AirHumidity;  //Stores humidity value in air
int Temp; //Stores temperature value
int gas;//Stores whole gas level in air 
int CO2LvL; //Stores CO2 Level value in air
int Liquid; //Stores the data of water exist or not 

// put your setup code here, to run once:
void setup() {
//Soil Humidity
  pinMode(3,INPUT);
  pinMode(5,OUTPUT); //sent the signal to relay of pump

  //liquid Sensor
    pinMode ( 6, INPUT_PULLUP ) ;
    pinMode (7, OUTPUT); //sent the signal to relay of led to show no water in the tank

//Temp. Humidity
  Serial.begin(9600);
  Serial.println("Temperature and Humidity Sensor Test");
  pinMode(4,OUTPUT); //sent the signal to relay of led to show temperature exceeds the level
  pinMode(8, OUTPUT);//send the signal to relay of led to show air humidity exceeds the level
  pinMode(9,OUTPUT); //sent the signal to relay of heater
  pinMode(10,OUTPUT); //sent the signal to relay of humidifier  

//CO2
  pinMode (co2sensor, INPUT);
  pinMode(12, OUTPUT); //send the signal to relay of LED to show co2 exceeds tha level
  pinMode (11, OUTPUT);//sent the signal to relay of fan
  Serial.begin(9600);//(check whether needed or not)
}

// put your main code here, to run repeatedly:
void loop() {
    SoilHumidity();
    TempAndAirHumidity();
    CO2Level();

  delay(1000); //Delay 1 sec.
}

void SoilHumidity(){
    //SoilHumidity
    SoilHumidity = digitalRead(3); //Read the soil sensor 
  
    if(SoilHumidity == HIGH){
      digitalWrite(5, LOW); //pump off when water is not needed
      digitalWrite ( 7, LOW);//led OFF
    }else{
      //Liquid Sensor
      Liquid = digitalRead (liquidsensor); //Read the liquid sensor
      //If it is 0, off the pump
      if ( Liquid < = 0 ){
        digitalWrite ( 5, LOW ) ;//pump off when no water in the tank
        digitalWrite ( 7, HIGH);//led to show no water
      }else{
        digitalWrite ( 5, HIGH ); //pump on when water needed
        digitalWrite ( 7, LOW);//led OFF
      }
   }
}

void TempAndAirHumidity(){
  //Temperature and Air humidity
    //Read data and store it to variables AirHumidity (humidity) and Temp (temperature)
    // Reading temperature or humidity takes about 250 milliseconds!
    DHT.read11(DHTPIN);
    
    AirHumidity = DHT.humidity;
    Temp = DHT.temperature;
    
    //Print temp and humidity values to serial monitor
    Serial.print("Humidity: ");
    Serial.print(AirHumidity);
    Serial.print(" %, Temp: ");
    Serial.print(Temp);
    Serial.println(" ° Celsius");

    if(Temp < 21){
      digitalWrite(9,HIGH); //Heater on when temperature low
      digitalWrite(4, LOW);//Led OFF
    }else if(Temp >= 21 || Temp <= 27){
      digitalWrite(9,LOW); //Heater off when temperature is in appropriate level 
      digitalWrite(4, LOW);//Led OFF
    }else{
      digitalWrite(9,LOW); //Heater off when temperature exceeds the appropriate level
      digitalWrite(4, HIGH);//Led to show Temp. Exceeds the required level
    }

    if(AirHumidity < 40){
      digitalWrite(10, HIGH); //Humidifier on when air humidity low
      digitalWrite(8, LOW); //Led OFF
    }else if(AirHumidity >= 40 || AirHumidity <= 60){
      digitalWrite(10,LOW); //Humidifier off when air humidity is in appropriate level
      digitalWrite(8, LOW); //Led OFF
    }else{
      digitalWrite(10,LOW); //Humidifier off when air humidity exceeds appropriate level
      digitalWrite(8, HIGH); //Led to show air humidity exceeds appropriate level
    }
}

void CO2Level(){
  //CO2 Level
    gas = analogRead(co2sensor); //Read the co2 sensor 
    CO2LvL = gas - 255; //(need to check the minimum value of 255)
    CO2LvL = map(CO2LvL, 0, 1024, 400, 5000);
    Serial.print(" CO2LvL: ");
    Serial.print(CO2LvL);
    Serial.print(" ppm");
    Serial.println();

    if(CO2LvL <= 400){
      //(what to do when less co2)
      digitalWrite(12, LOW); //Led OFF 
    }else if(CO2LvL>=400 && CO2LvL<=1000){
      digitalWrite(11,LOW);//fan off when co2 is in appropriate level
      digitalWrite(12, LOW); //Led OFF 
    }else{
      digitalWrite(11, HIGH);//fan on when co2 level
      digitalWrite(12, HIGH); //Led to show co2 exceeds the appropriate level 
    }
}
