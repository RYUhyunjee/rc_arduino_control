#include <stdio.h>
#include <math.h>

//#define DEBUG
#define DEF_STEER 200
#define DEF_SPEED 200
#define encoder_pin A0 
/*
 #Steps:
 1.Connect the M1_PWM & M2_PWM to UNO digital 5 & 6
 2.Connect the M1_EN & M2_EN to UNO digital 4 & 7
 3.Connect +5V & GND to UNO 5V & GND
 
 # Function for current sense and diagnosis,if you want to use
 please connect the IS pins to Arduino
 Connect LA_IS & RA_IS to UNO digital 2
 Connect LB_IS & RB_IS to UNO digital 3
 */
#define steer_speed 5     //M1 Speed Control
#define throttle2_speed 6     //M2 Speed Control
#define steer_di 4     //M1 Direction Control
#define throttle2_di 7     //M2 Direction Control
int steer_val = DEF_STEER;
int counter=0;
String cmd_str = "";
int encoder_val = 0;

void stop(void)                    //Stop
{
  digitalWrite(steer_speed,0); 
  digitalWrite(steer_di,LOW);    
  digitalWrite(throttle2_speed,0);   
  digitalWrite(throttle2_di,LOW);    
}   
//void forward(char a,char b)
void forward(char a)//Move forward
{   
  analogWrite (throttle2_speed,a);    
  digitalWrite(throttle2_di,HIGH);
}  
//void backward (char a,char b)
void backward (char a)//Move backward
{ 
  analogWrite (throttle2_speed,a);    
  digitalWrite(throttle2_di,LOW);
}
void turn_L (char a)             //Turn Left
{
  analogWrite (steer_speed,a);      //PWM Speed Control
  digitalWrite(steer_di,HIGH);  
}
void turn_R (char a)             //Turn Right
{
  analogWrite (steer_speed,a);
  digitalWrite(steer_di,LOW);  
}


void current_sense()                  // current sense and diagnosis
{
  int val1=digitalRead(2);
  int val2=digitalRead(3);
  if(val1==HIGH || val2==HIGH){
    counter++;
    if(counter==3){
      counter=0;
      Serial.println("Warning");
    }  
  } 
}

void setup(void) 
{ 
  int i;
  for(i=4;i<=7;i++)
    pinMode(i, OUTPUT); 
  Serial.begin(19200);      //Set Baud Rate
  //Serial.println("Run keyboard control");
  digitalWrite(steer_speed,LOW);   
  digitalWrite(throttle2_speed,LOW); 
  pinMode(2,INPUT);
  pinMode(3,INPUT);
} 

void loop(void) 
{
  encoder_val = analogRead(encoder_pin);
  if(Serial.available()){
    cmd_str = Serial.readString();
    //char val = Serial.read();
/*    
    int first = cmd_str.indexOf(",");
    int second = cmd_str.indexOf(",",first+1);
    int strlength = cmd_str.length();
    String speed_val_s = cmd_str.substring(0,first);
    String steer_val_s = cmd_str.substring(first+1,second);

    int speed_val = speed_val_s.toInt(); // speed_val : -255 ~ 255
    int steer_val = steer_val_s.toInt(); // steer_val : -255 ~ 255
*/
    int steer_val = cmd_str.toInt(); // steer_val : -255 ~ 255
/*    
    if(speed_val >= 0){
      analogWrite (throttle2_speed,speed_val);
      digitalWrite(throttle2_di,HIGH);
    }
    else{
      speed_val = speed_val*(-1);
      analogWrite (throttle2_speed,speed_val);
      digitalWrite(throttle2_di,LOW);
    }
  */ 
    if(steer_val >= 0){
      analogWrite (steer_speed,steer_val);
      digitalWrite(steer_di,LOW);
    }
    else{
      steer_val = steer_val*(-1);
      analogWrite (steer_speed,steer_val);
      digitalWrite(steer_di,HIGH);
    }
    Serial.println(steer_val);
    Serial.println(encoder_val);
  }

  delay(5);
}
