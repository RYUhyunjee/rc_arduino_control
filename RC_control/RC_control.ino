#include <stdio.h>
#include <Servo.h>
#include <math.h>

//#define DEBUG
#define pin_throttle 8 // set pin number 8 of arduino mega as rc_throttle pin 
#define pin_steer 7 // set pin number 7 of arduino mega as rc_steer pin
#define LED 13
#define ZERO_STEER 1450
#define ZERO_SPEED 1500
void lidar_control();


Servo steer;
Servo throttle;

int steer_val = ZERO_STEER;
int throttle_val = ZERO_SPEED;
int lane_steer_val_old, lane_steer_val_new = ZERO_STEER;
int lane_throttle_val = ZERO_SPEED;


void setup() {
  Serial.begin(115200);
  
//  steer.attach(pin_steer);
//  throttle.attach(pin_throttle);
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
}

void loop() {
  
  if(Serial.available()){
    digitalWrite(LED, HIGH);
    lane_steer_val_old = lane_steer_val_new;
    lane_steer_val_new = Serial.parseInt();
    lane_throttle_val = Serial.parseInt();
    Serial.read();
  }
  else{
    digitalWrite(LED, LOW);
  }

  //if the diffrence between the pervious and persent steering values is less than 5000, the (lane)vision will control rc_car or the rplidar will control it.
  //if you want to change the Criteria then, change the value(now the Criteria is 5000). 
   if( abs(lane_steer_val_new - lane_steer_val_old) < 5000){
    // use vision(lane) control
    steer_val = lane_steer_val_new;
    throttle_val = lane_throttle_val;
  }
  else{
    //do lidar control   
    lidar_control();
  }

//  throttle.writeMicroseconds(throttle_val);
//  steer.writeMicroseconds(steer_val);


  Serial.print(steer_val);
  Serial.print(",");
  Serial.println(throttle_val);

}

void lidar_control()
{
  
}
