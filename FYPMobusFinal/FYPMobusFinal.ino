// Include the library:
#include <SPI.h>
#include <Eth2ernet.h>
#include "MgsModbus.h" //library in the directory

MgsModbus Mb;
int inByte=0;
byte mac[] = {0x90 , 0xA2 , 0xDA , 0x0E ,0x94 , 0xB5};
IPAddress ip(192,168,2,12);
IPAddress gateway(192,168,2,1);
IPAddress subnet(255,255,255,0);

bool F1; //for Codesys
bool F5;
bool F9;
const int F1butt = A4;//was A5 //was10
const int F5butt = 5;
const int F9butt = 7;
bool ICB1; //for Codesys
bool GCB; //for Codesys
bool FCB3; //for Codesys
const int led1 = A3; //was 12
const int led5 = 6; //was 12
const int led9 = 8; //was 12
int reading;
int writing;
int reading2;
int writing2;
int reading3;
int writing3;
int LDR= 4; //was4
int LightSensor;
bool G;
const int ledG = 4; //was5
int buzzer=12;
const int Relay=A0; //wasA0
const int Relay2=A1; 

const int house1=2;
const int house2=3;
const int house3=9;

bool S1; //Generator 


const int voltageinputPIN = A2; //select analog input pin for voltage sensor
const int baudRate = 9600; //sets baud rate in bits per second for serial monitor
const int sensorreadDelay = 250; //sensor read delay in milliseconds
const int maxanalogValue = 1010; //highest integer given at max input voltage
const int sensormaxVoltage = 25; //highest input voltage of sensor being used

float analogVoltage = 0; //to store voltage value at analog pin

void setup() 

{

Serial.begin(9600);
Serial.println("Serial interface started");

Ethernet.begin(mac , ip , gateway ,subnet);
Serial.println("Ethernet interface started");

//-----------CodeSys Read
Mb.MbData[0]=0;

//-----------CodeSys write
Mb.MbData[1]=0;
Mb.MbData[2]=0;
Mb.MbData[3]=0;


 
Serial.begin(9600);

pinMode(F1butt,INPUT);
pinMode(led1,OUTPUT);
pinMode(F5butt,INPUT);
pinMode(led5,OUTPUT);
pinMode(F9butt,INPUT);
pinMode(led9,OUTPUT);
pinMode(Relay,OUTPUT);
pinMode(Relay2,OUTPUT);
pinMode(house1,OUTPUT);
pinMode(house2,OUTPUT);
pinMode(house3,OUTPUT);
pinMode(12,OUTPUT);



 for( int i =0; i<8;i++)
{Serial.print("Adress");Serial.print(i);Serial.print("Data: ");Serial.println(Mb.MbData[i]);}

Serial.println("0 - Print the Holding registers");
Serial.println("1- Fill holding registers 0x5555");

     
} 

void loop() 
{
if (Serial.available() >0){
    inByte =  Serial.read();
    if (inByte=='0'){
      for (int i=0; i<0; i++){
      Serial.print("Address");Serial.print(i);Serial.print("Data: ");Serial.print(Mb.MbData[i]);}
    }
    if (inByte=='1'){
      for (int i=1; i<8; i++)
      Mb.MbData[1]=0x555;
    }
     if (inByte=='0'){
      for (int i=1; i<8; i++)
      Mb.MbData[1]=0;
    }
  }
  Mb.MbsRun();

 //**********************************************************Fault Detection*************************************************************//
 //--Zone1-----------------
 
  F1=(Mb.MbData[1]>>0)&1;
  ICB1=(Mb.MbData[0]>>2)&1;

  writing = digitalRead (F1butt);
    {
  }
  if (writing==1)
  {
    digitalWrite(F1,HIGH);
    Mb.MbData[1]|=(1<< 0);
      delay(50);
       Serial.println("Fault detected at Zone 1");
  }
  else
  {
    digitalWrite(F1,LOW);
    delay(50);
    Mb.MbData[1]=0;
     Serial.println("No Fault detected at Zone 1");
    
    
  }

  reading = digitalRead (ICB1);
  {
  }
  if (reading==0)
  {
    digitalWrite (led1, HIGH);
    delay(500);
    digitalWrite (led1,LOW);
    delay(500);
    tone(buzzer,4000,5000);
    Serial.println("Zone 1: Fault");
  }
  if (reading==1){
    Serial.println("Zone 1: No Fault");
    
  }

//--Zone5-----------------
 
  F5=(Mb.MbData[3]>>0)&1;
  GCB=(Mb.MbData[0]>>7)&1;

  writing2 = digitalRead (F5butt);
    {
  }
  if (writing2==1)
  {
    digitalWrite(F5,HIGH);
    Mb.MbData[3]|=(1<< 0);
      delay(50);
       Serial.println("Fault detected at Zone 5");
  }
  else
  {
    digitalWrite(F5,LOW);
    delay(50);
    Mb.MbData[3]=0;
     Serial.println("No Fault detected at Zone 5");
    
    
  }

  reading2 = digitalRead (GCB);
  {
  }
  if (reading2==0)
  {
    digitalWrite (led5, HIGH);
    delay(500);
    digitalWrite (led5,LOW);
    delay(500);
    tone(buzzer,4000,5000);
    Serial.println("Zone 5: Fault");
  }
  if (reading2==1){
    Serial.println("Zone 5: No Fault");
    
  }

  //--Zone9-----------------
 
  F9=(Mb.MbData[4]>>0)&1;
  FCB3=(Mb.MbData[0]>>10)&1;

  writing3 = digitalRead (F9butt);
    {
  }
  if (writing3==1)
  {
    digitalWrite(F9,HIGH);
    Mb.MbData[4]|=(1<< 0);
      delay(50);
       Serial.println("Fault detected at Zone 9");
  }
  else
  {
    digitalWrite(F9,LOW);
    delay(50);
    Mb.MbData[4]=0;
     Serial.println("No Fault detected at Zone 9");
    
    
  }

  reading3 = digitalRead (FCB3);
  {
  }
  if (reading3==0)
  {
    digitalWrite (led9, HIGH);
    delay(500);
    digitalWrite (led9,LOW);
    delay(500);
    tone(buzzer,4000,5000);
    Serial.println("Zone 9: Fault at Zone 9");
  }
  if (reading3==1){
    Serial.println("Zone 9: No Fault");
    
  }

 //******************************************** Light Sensor Solar Module****************************************************************//
 // reads the input on analog pin A0 (value between 0 and 1023)

 LDR= Mb.MbData[2];
 G=(Mb.MbData[0]>>11)&1;
  
 int sensor = analogRead(A5); //Read

  Serial.print('\n');
  Serial.print("Light Intensity = ");
  Serial.print(sensor);   // the raw analog reading
  Mb.MbData[2] = sensor;
  

  // We'll have a few threshholds, qualitatively determined
  if (sensor < 100) {
    Serial.println(" - Very bright");
    digitalWrite( ledG,HIGH);
    Mb.MbData[0]|=(1<< 11);
  } else if (sensor < 200) {
    Serial.println(" - Bright");
     digitalWrite( ledG,HIGH);
     Mb.MbData[0]|=(1<< 11);
  } else if (sensor < 500) {
    Serial.println(" - Light");
    digitalWrite( ledG,HIGH);
    Mb.MbData[0]|=(1<< 11);
  } else if (sensor < 800) {
    Serial.println(" - Dim");
    digitalWrite( ledG,LOW);
    Mb.MbData[0]|=(0<< 11);
  
  } else {
    Serial.println(" - Dark");
    digitalWrite( ledG,LOW);
    Mb.MbData[0]|=(0<< 11);

  delay(500);
  
 
  

  } 
  
//******************************************** Relay for Generator****************************************************************//

 S1=(Mb.MbData[0]>>0)&1;


 
 if (((Mb.MbData[0]>>0)&1)==1)
  {
    digitalWrite(Relay,HIGH);
    Serial.println("Power Relay is functioning");
  }
  else
  {
    digitalWrite(Relay,LOW);
    Serial.println("Power Relay is not functioning");
  }
  //Solar Relay
  if (((Mb.MbData[0]>>11)&1==1))
  {
    digitalWrite(Relay2,LOW);
    Serial.println("Solar Relay is functioning");
  }
  else
  {
    digitalWrite(Relay2,HIGH);
    Serial.println("Solar Relay is not functioning");
  }
  //******************************************** lIGHT Houses****************************************************************//
  //HOUSE1
  if (((Mb.MbData[0]>>13)&1)==1)
  {
    digitalWrite(house1,HIGH);
    Serial.println("house 1 is functioning");
  }
  else
  {
    digitalWrite(house1,LOW);
    Serial.println("house1 is not functioning");
  }
  //HOUSE2
  if (((Mb.MbData[0]>>14)&1)==1)
  {
    digitalWrite(house2,HIGH);
    Serial.println("house 2 is functioning");
  }
  else
  {
    digitalWrite(house2,LOW);
    Serial.println("house 2 is not functioning");
  }
  //HOUSE3
  if (((Mb.MbData[0]>>15)&1)==1)
  {
    digitalWrite(house3,HIGH);
    Serial.println("house 3 is functioning");
  }
  else
  {
    digitalWrite(house3,LOW);
    Serial.println("house3 is not functioning");
  }

//**********************************************************************Voltage Reader****************************************************************//
  Mb.MbData[5] = analogVoltage;
  analogVoltage = analogRead(voltageinputPIN); //reads analog voltage of incoming sensor
  analogVoltage = (analogVoltage/maxanalogValue)*sensormaxVoltage; //conversion equation
  Serial.print(analogVoltage); //prints value to serial monitor
  Serial.println("V"); //prints label
  delay(sensorreadDelay); //delay in milliseconds between read values
  
  
  }
