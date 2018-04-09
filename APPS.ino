const int button = 2;
const int driveEnable = 3;
const int RTDS = 4;
const int sensor_break = A0;
const int sensor_ac_1 = A1;
const int sensor_ac_2 = A2;
const int accelerating = 5;
const int ERROR_SGN = 6;


int state = 0;
float s1=0;
float s2=0;
float br=0;
int bt = 0;
int diff = 0;
float base_s1=0;
float base_s2=0;
float base_b=0;
int first_init=0;
float s_output=0;

void setup() {
  // put your setup code here, to run once:
  pinMode(ERROR_SGN, OUTPUT);
  pinMode(accelerating, OUTPUT);
  pinMode(sensor_break, INPUT);
  pinMode(sensor_ac_1,INPUT);
  pinMode(sensor_ac_2,INPUT);
  pinMode(driveEnable,OUTPUT);
  pinMode(RTDS,OUTPUT);
  pinMode(button, INPUT);
  Serial.begin(9600);
  calibrate();

}



void loop() {
  // put your main code here, to run repeatedly:

switch (state){
  
  case 0: //engine off
  
    read_sensors(); 
    print_values();
    check_output();
    
    if (diff<10&&bt==LOW&&br>50&&s_output<5) {
      
      if(first_init == 0){
      Serial.print("\n\n Beepping RTDS");
      digitalWrite(RTDS,HIGH);
      delay(3000);
      digitalWrite(RTDS,LOW);
      first_init=1;
      }
     
      
      state = 1;
      break;      
    }   
    else if(diff>10) {
      
     state = 2;
     break;}
     
     break;
       
  case 1: // engine on
  
    check_output();
    digitalWrite(driveEnable,HIGH);
    read_sensors();
    print_values();
    if(diff>10){
      state = 2;
      digitalWrite(driveEnable,LOW);
      s_output=0;
    
      break;    
    }
    else {
    state = 1;
    break;
    }
    break;    
    
  case 2: //fail
    digitalWrite(ERROR_SGN,HIGH);
    read_sensors();
    print_values();
    check_output();
    if(diff<10) {
      digitalWrite(ERROR_SGN,LOW);
      state = 0;
      break;
    }
    else{
      
      state = 2;
      break;
    }
  default:
     Serial.println("ERROR");
     
  }
  
  
}


void read_sensors () {

    br = analogRead(sensor_break);
    br = (br/base_b)*100;
    s1 = analogRead(sensor_ac_1);
    s1=(s1/base_s1)*100;
    s2 = analogRead(sensor_ac_2);
    s2 = (s2/base_s2)*100;
    bt = digitalRead(button);
    diff = abs(s1 - s2); 
}

void calibrate (){
  
  
//calibrating sensor 1
Serial.print("Release the throttle pedal\n");
delay(5000);
int temp1 = analogRead(sensor_ac_1);


Serial.print("Press the throttle pedal to the max...");
delay(5000);
int temp2 = analogRead(sensor_ac_1);
base_s1 = abs(temp1 - temp2);
Serial.print("OK!\n\n");
//calibrating sensor 2

Serial.print("Release the throttle pedal again\n");
delay(5000);
temp1 = analogRead(sensor_ac_2);
Serial.print("Press it to the max...");
delay(5000);
temp2 = analogRead(sensor_ac_2);
base_s2 = abs(temp1 - temp2);
Serial.print("OK!\n\n");
//calibrating break pedal

Serial.print("Release the break pedal\n");
delay(5000);
temp1 = analogRead(sensor_break);
Serial.print("Press it to the max...");
delay(5000);
temp2 = analogRead(sensor_break);
base_b = abs(temp1 - temp2);
Serial.print("OK!\n\n");
Serial.print("\n\nLoading...\n");
delay(3000);


}

void print_values() {
  if(bt == HIGH) Serial.print("\nbutton pressed"); 
  else Serial.print("\n\nOFF");
  Serial.print("\nSensor 1: ");
  Serial.print(s1);
  Serial.print("%\n");
  Serial.print("Sensor 2: ");
  Serial.print(s2);
  Serial.print("%\n");
  Serial.print("break: ");
  Serial.print(br);
  
  Serial.print("%\n\n\nOutput: ");
  Serial.print(s_output);
  Serial.print("%\nstate: ");
  Serial.print(state);
  Serial.print("\n\n");
  
  delay(500);

}

void check_output(){
  
  if (state==0||state==2){
    s_output = 0;
    analogWrite(accelerating,int((255/100)*s_output));
  
  }

  else if(s1>s2){
    s_output = s2;
    analogWrite(accelerating,int((255/100)*s_output));        
  }
  else{
    s_output = s1;
    analogWrite(accelerating,int((255/100)*s_output));
  }
  
  
}
