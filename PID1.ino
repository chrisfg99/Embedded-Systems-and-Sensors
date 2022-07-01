int RawSensorValue;
int ground[2]={};
int Threshold = 1005;
enum  WhereRobotNeedsToGo {Forward, Stop, Left, Right};
enum  WhereRobotNeedsToGo CurrentState = Forward;
bool sensor1;
bool sensor2;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  UpdateGroundSensors();
  sensor1 = ground[0];
  sensor2 = ground[1];
  ProcessRobotState(sensor1, sensor2);
  ActOnRobotState();
}

void UpdateGroundSensors(){
  for (int i=1;i<3;i++){
    ReadGroundSensor(i);
    ConvertSensorToBinary(i);
  }
}

void GroundLEDoff(unsigned char lineIndex){
  if ((lineIndex==1)|(lineIndex==2)){
    PORTJ |=(1<<lineIndex);//Set index to ON
  }
}
void GroundLEDon(unsigned char lineIndex){
  if ((lineIndex==1)|(lineIndex==2)){
    PORTJ &= ~(1<<lineIndex);//Set index to OFF
}
}

void ReadGroundSensor(int sensornum){
  GroundLEDon(sensornum);
  delay(10);
  RawSensorValue = analogRead(sensornum+8);
  GroundLEDoff(sensornum);
}

void ConvertSensorToBinary(int sensornum){
  if (RawSensorValue < Threshold){
    ground[sensornum-1] = 1;
  }
  else {
    ground[sensornum-1] = 0;
  }
}

void ProcessRobotState(bool sensor1, bool sensor2){
  if ((sensor1 == 0) && (sensor2 == 0)){
    CurrentState = Forward;
  }
  else if((sensor1 == 1) && (sensor2 == 0)){
    CurrentState = Left;
  }
  else if((sensor1 == 0) && (sensor2 == 1)){
    CurrentState = Right;
  }
  else{
    CurrentState = Stop;
  }  
}
void ActOnRobotState(){
  if (CurrentState == Forward) {
    MotorsForward();
  }
  else if (CurrentState == Left) {
    RightTurn();
  }
  else if (CurrentState == Right) {
    LeftTurn();
  }
  else if (CurrentState == Stop) {
    MotorsStop();
  }  
}

void LeftTurn(){
  LeftMotorForward(0);
  RightMotorForward(20);
}
void RightTurn(){
  LeftMotorForward(20);
  RightMotorForward(0);
}
void MotorsStop(){
  analogWrite(2,1);
  analogWrite(5,1);
  analogWrite(6,1);
  analogWrite(7,1);
}
//

void Backward(float duty){
  RightMotorBackward(duty *2.55);
  LeftMotorBackward(duty *2.55);
}
void LeftMotorForward(float duty){
  analogWrite(6, duty *2.55);
  analogWrite(7,0);  
}

void LeftMotorBackward(float duty){
  analogWrite(6,0);
  analogWrite(7,duty *2.55);
}

void RightMotorForward(float duty){
  analogWrite(6, duty *2.55);
  analogWrite(7,0);
}

void RightMotorBackward(float duty){
  analogWrite(5,duty *2.55);
  analogWrite(2,0);
}

void MotorsForward(){
  LeftMotorForward(10);
  RightMotorForward(10);
}
