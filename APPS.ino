const int button = 2;
const int driveEnable = 3;
const int RTDS = 4;
const int sensor_break = A0;
const int sensor_ac_1 = A1;
const int sensor_ac_2 = A2;

int state = 0;
int s1=0;
int s2=0;
int br=0;
int bt = 0;
int diff = 0;
int base_s1=0;
int base_s2=0;
int base_b=0;

void setup() {
  // put your setup code here, to run once:
  
  pinMode(driveEnable,OUTPUT);
  pinMode(RTDS,OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
  //calibrate();

}



void loop() {
  // put your main code here, to run repeatedly:


switch (state){
  
  case 0: //engine off
  
    read_sensors(); 
    print_values();
    if (diff<10&&bt==HIGH&&br>50) {
      digitalWrite(RTDS,HIGH);
      delay(3000);
      digitalWrite(RTDS,LOW);
      state = 1;
      break;      
    }   
    else if(diff>10) state = 2;
       
  case 1: // engine on
    read_sensors();
    print_values();
    if(diff>10){
      state = 2;
      break;    
    }      
    
  case 2: //fail
    read_sensors();
    print_values();
    if(diff<10) state = 0;
    
    
  default:
     Serial.println("ERROR");
     
  }
}


void read_sensors () {

    br = analogRead(sensor_break);
    br = (br/1023)*100;        
    s1 = analogRead(sensor_ac_1);
    s1 = (s1/1023)*100;
    s2 = analogRead(sensor_ac_2);
    s2 = (s2/1023)*100;
    bt = digitalRead(button);
    diff = abs(s1 - s2); 
}

void calibrate (){

Serial.print("Dont press the gas pedal\n");
delay(5000);
int temp1 = analogRead(sensor_ac_1);
Serial.print("Press the gas pedal to the max\n");
delay(5000);
int temp2 = analogRead(sensor_ac_1);
base_s1 = abs(temp1 - temp2);

Serial.print("Dont press the gas pedal again\n");
delay(5000);
temp1 = analogRead(sensor_ac_2);
Serial.print("Pres it to the max\n");
delay(5000);
temp2 = analogRead(sensor_ac_2);
base_s2 = abs(temp1 - temp2);

Serial.print("Dont press the break pedal\n");
delay(5000);
temp1 = analogRead(sensor_break);
Serial.print("Press to the max\n");
delay(5000);
temp2 = analogRead(sensor_break);
base_b = abs(temp1 - temp2);

}

void print_values() {
  if(bt == HIGH) Serial.print("ON"); 
  else Serial.print("OFF");
  Serial.print("Sensor 1: ");
  Serial.print(s1);
  Serial.print("%\n");
  Serial.print("Sensor 2: ");
  Serial.print(s2);
  Serial.print("%\n");
  Serial.print("break: ");
  Serial.print(br);
  Serial.print("%\n\n\n");
  
  delay(2000);

}
