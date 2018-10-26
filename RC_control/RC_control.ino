#include <stdio.h>
#include <Servo.h>
#include <math.h>
#include <RPLidar.h>

//#define DEBUG
#define pin_throttle 8
#define pin_steer 7
#define RPLIDAR_MOTOR 3
#define ZERO_SPEED 1450
#define ZERO_STEER 1450
#define KP 1.5


Servo steer;
Servo throttle;
RPLidar lidar;

int steer_val = ZERO_STEER;
int throttle_val = ZERO_SPEED;
int tx2_steer_val_old, tx2_steer_val_new = ZERO_STEER;
int tx2_throttle_val = ZERO_SPEED;
int lidar_steer_val = ZERO_STEER;
int lidar_throttle_val = ZERO_SPEED;


void setup() {
  Serial.begin(115200);
  lidar.begin(Serial3);

  
  steer.attach(pin_steer);
  throttle.attach(pin_throttle);
  
  pinMode(RPLIDAR_MOTOR, OUTPUT);


}

void loop() {
  if(Serial.available()){
    tx2_steer_val_old = tx2_steer_val_new;
    tx2_steer_val_new = Serial.parseInt();
    tx2_throttle_val = Serial.parseInt();
    Serial.read();
  }

   if( abs(tx2_steer_val_new - tx2_steer_val_old) < 5000){
    // use vision(tx2) control
    steer_val = tx2_steer_val_new;
    throttle_val = tx2_throttle_val;
  }
  else{
    //do lidar control   
    lidar_control();
  }

  throttle.writeMicroseconds(throttle_val);
  steer.writeMicroseconds(steer_val);


  Serial.print(steer_val);
  Serial.println(throttle_val);

}

float minDistance = 100000;
float maxDistance = 0;
float angleAtMinDist = 0;
int angleAtMaxDist = 0;
int decideDistance = 0;
float lidarDistance[360] = {0,};
float rightDistance[90] = {300,};
float leftDistance[89] = {300,};
int steerAngle = 0;
int throttleSpeed = 1515;

void lidar_control(){
  if(throttleSpeed > 1450){
    throttleSpeed -= 1;
  }
  throttle.write(throttleSpeed);
  
  if (IS_OK(lidar.waitPoint())) {
    //perform data processing here...
    float distance = lidar.getCurrentPoint().distance;
    float angle = lidar.getCurrentPoint().angle;
    if (lidar.getCurrentPoint().startBit) {
      for (int i = 0; i < 90; i++) {
        if (rightDistance[i] >= 1000
        && !decideDistance) {
          maxDistance = rightDistance[i];
          angleAtMaxDist = i;
          decideDistance = 1;
        }
#ifdef DEBUG
        Serial.print(rightDistance[i]);
        Serial.print(' ');
#endif
      }
#ifdef DEBUG
      Serial.println(' ');
      Serial.println(angleAtMaxDist);
#endif
      steerAngle = 90 + angleAtMaxDist * KP;
      steer.write(steerAngle);
      minDistance = 100000;
      maxDistance = 0;
      angleAtMinDist = 0;
      angleAtMaxDist = 0;
      decideDistance = 0;
    }
    else {
      int index = (int)angle;
      lidarDistance[index] = distance;
      if (index < 90) {
        rightDistance[index] = distance;
      }
      else if (index >= 270) {
        leftDistance[-(index - 359)] = distance;
      }
    }
  }
  else {
    analogWrite(RPLIDAR_MOTOR, 0); //stop the rplidar motor

    // try to detect RPLIDAR...
    rplidar_response_device_info_t info;
    if (IS_OK(lidar.getDeviceInfo(info, 100))) {
      //detected...
      lidar.startScan();
      analogWrite(RPLIDAR_MOTOR, 255);
      delay(1000);
    }
  }
  
}
