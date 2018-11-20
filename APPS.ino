#include <PWM.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,2,1,0,4,5,6,7,3, POSITIVE);

const int button = 5;
const int driveEnable = 8;
const int RTDS = 4;
const int sensor_break = A0;
const int sensor_ac_1 = A2;
const int sensor_ac_2 = A1;
const int accelerating = 9;
const int frequency = 20000;

int state = 0;
float s1=0;
float s2=0;
float br=0;
int bt = 0;
float s_output=0;
int diff = 0;
float base_s1=0;
float base_s2=0;
float base_b=0;
float offset_s1 = 0;
float offset_s2 = 0;
float offset_b = 0;
int first_init=0;


void setup() {
  // put your setup code here, to run once:
  pinMode(accelerating, OUTPUT);
  pinMode(sensor_break, INPUT);
  pinMode(sensor_ac_1,INPUT);
  pinMode(sensor_ac_2,INPUT);
  pinMode(driveEnable,OUTPUT);
  pinMode(RTDS,OUTPUT);
  pinMode(button, INPUT);
//  Serial.begin(9600);
  lcd.begin (16,2);
  analogReference(EXTERNAL);
  InitTimersSafe();
  SetPinFrequency(accelerating,frequency);
  
  calibrate();

}



void loop() {
  // put your main code here, to run repeatedly:

switch (state){
//-------------------------ENGINE OFF STATE ------------------ 
  case 0: //engine off
  
    read_sensors(); 
    print_values();
    check_output();
    
    if (diff<10 && bt==HIGH && br>50 && s1<5 && s2<5) {
      
      if(first_init == 0){
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("Beepping RTDS");
        digitalWrite(RTDS,HIGH);
        delay(3000);
        digitalWrite(RTDS,LOW);
        first_init=1;
      }
     
      
      state = 1;
      break;      
    }   
    else if(diff>10) {
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.println( "ERRO DE LEITURA!");
        delay(2000);
        state = 2;
     break;}
     
     break;
 //-----------------------ENGINE ON STATE ------------------
  case 1: // engine on
  
    check_output();
    digitalWrite(driveEnable,LOW);
    read_sensors();
    print_values();
    if(diff>10){
      state = 2;
      digitalWrite(driveEnable,HIGH);
      s_output=0;   
      break;    
    }
    else {
    state = 1;
    break;
    }
    break;    
//--------------------------------ERRROR STATE----------------------
  case 2: 
    read_sensors();
    print_values();
    check_output();
    if(diff<10) {      
      state = 0;
      break;
    }
    else{
      
      state = 2;
      break;
    }
  default:
     lcd.clear();
     lcd.setCursor(0,0);
     Serial.println("ERROR");
     
  }
  
  
}
//---------------------- MAIN END----------------------------

void read_sensors () {
    br = analogRead(sensor_break);
    br = ((br-offset_b)/base_b)*100;
    if(br<0)br=0;
    else if(br>100)br=100;
    s1 = analogRead(sensor_ac_1);
    s1=((s1-offset_s1)/base_s1)*100;
    if(s1<0)s1=0;
    else if(s1>100)s1=100;
    s2 = analogRead(sensor_ac_2);
    s2 = ((s2-offset_s2)/base_s2)*100;
    if(s2<0)s2=0;
    else if(s2>100)s2=100;
    bt = digitalRead(button);
    diff = abs(s1 - s2);  
}

void calibrate (){
  lcd.setCursor(0,0);  
  //calibrating sensor 1
  lcd.print("SOLTE O");
  lcd.setCursor(0,1);
  lcd.print("ACELERADOR");
  delay(5000);
  int temp1 = analogRead(sensor_ac_1);

  lcd.clear();
  lcd.print("PISE AO MAXIMO");
  lcd.setCursor(0,1);
  lcd.print("NO ACELERADOR");
  delay(5000);
  int temp2 = analogRead(sensor_ac_1);
  offset_s1 = temp1;
  base_s1 = abs(temp1 - temp2);
  lcd.print(" OK");
  delay(1000);
  
  //calibrating sensor 2
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("SOLTE O");
  lcd.setCursor(0,1);
  lcd.print("ACELERADOR");
  delay(5000);
  temp1 = analogRead(sensor_ac_2);
  lcd.clear();
  lcd.print("PISE AO MAXIMO");
  lcd.setCursor(0,1);
  lcd.print("NO ACELERADOR");
  delay(5000);
  temp2 = analogRead(sensor_ac_2);
  offset_s2= temp1;
  base_s2 = abs(temp1 - temp2);
  lcd.print(" OK");
  delay(1000);
  
  //calibrating break pedal

  lcd.clear();
  lcd.print("SOLTE O FREIO");
  delay(5000);
  temp1 = analogRead(sensor_break);
  lcd.clear();
  lcd.print("PISE AO MAXIMO");
  lcd.setCursor(0,1);
  lcd.print("NO FREIO");
  delay(5000);
  temp2 = analogRead(sensor_break);
  offset_b = temp1;
  base_b = abs(temp1 - temp2);
  lcd.print(".. OK!");
  delay(1000);
  lcd.clear();
  lcd.print("Loading");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
  delay(1000);
  lcd.print(".");
}

void print_values(){
  if(bt==LOW&&state==0){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("PISE NO FREIO");
    lcd.setCursor(0,1);
    lcd.print("E APERTE O BOTAO");
    delay(40);
  }
  else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("ON");    
    lcd.print(" ST = ");
    lcd.print(state);
    lcd.print(" br=");
    lcd.print(int(br));
    lcd.setCursor(0,1);
    lcd.print("s1=");
    lcd.print(int(s1));
    lcd.print(" s2=");
    lcd.print(int(s2));
    lcd.print(" o=");
    lcd.print(int(s_output));
    delay(40);
    }
}

//void print_values() {
//  if(bt == HIGH) Serial.print("\nOFF"); 
//  else Serial.print("\n\nBUTTON PRESSED");
//  Serial.print("\nSensor 1: ");
//  Serial.print(s1);
//  Serial.print("%\n");
//  Serial.print("Sensor 2: ");
//  Serial.print(s2);
//  Serial.print("%\n");
//  Serial.print("break: ");
//  Serial.print(br);
//  
//  Serial.print("%\n\n\nOutput: ");
//  Serial.print(s_output);
//  Serial.print("%\nstate: ");
//  Serial.print(state);
//  Serial.print("\n\n");
//  
//  delay(500);
//
//}

void check_output(){
  
  if (state==0||state==2){
    s_output = 0;
    pwmWrite(accelerating,int((255/100)*s_output));
  
  }

  else if(s1>s2){
    s_output = s2;
    //pwmWrite(accelerating,int((255/100)*s_output));      
    pwmWrite(accelerating,map(s_output,0, 100, 0, 255));  
  }

  //place here the floating cable condition after knowing the  edges!!
  /*else if(br>br_low_edge||br<br_up_edge||s1>s1_low_edge||s1<s1_up_edge||s2>s2_low_edge||s2<s2_up_edge){
    state = 2;
    Serial.println("\nFLOATING CABLE!!\n");
    }*/
  else{
    s_output = s1;
    pwmWrite(accelerating,map(s_output,0, 100, 0, 255));  
  }
  
  
}
