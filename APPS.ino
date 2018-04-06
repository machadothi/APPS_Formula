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
  pinMode(sensor_break,INPUT);
  pinMode(sensor_ac_1, INPUT);
  pinMode(sensor_ac_2, INPUT);
  Serial.begin(9600);

}

void loop() {
  // put your main code here, to run repeatedly:

calibrate();
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

    br = (analogRead(sensor_break)/base_b)*100;
    s1 = (analogRead(sensor_ac_1)/base_s1)*100;
    s2 = (analogRead(sensor_ac_2)/base_s2)*100;
    bt = digitalRead(button);
    diff = abs(s1 - s2); 
}

void calibrate (){

Serial.println("Dont press the gas pedal");
int temp1 = analogRead(sensor_ac_1);
Serial.println("Press the gas pedal to the max");
int temp2 = analogRead(sensor_ac_1);
base_s1 = abs(temp1 - temp2);

Serial.println("Dont press the gas pedal again");
int temp1 = analogRead(sensor_ac_2);
Serial.println("Pres it to the max");
int temp2 = analogRead(sensor_ac_2);
base_s2 = abs(temp1 - temp2);

Serial.println("Dont press the break pedal");
int temp1 = analogRead(sensor_break);
Serial.println("Press to the max");
int temp2 = analogRead(sensor_break);
base_b = abs(temp1 - temp2);

}

void print_values() {

  Serial.println("Sensor 1: %d", s1);
  Serial.println("Sensor 2: %d", s2);
  Serial.println("Break: %d", br);

}
