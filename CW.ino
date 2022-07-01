int line[4];//store black light numbers
float s;//speed or duty cycle
int black0=1016;//Values for black on sensors 0-3
int black1=1010;
int black2=1010;
int black3=1016;
int white0=1000;//Values for white on sensors 0-3
int white1=980;
int white2=980;
int white3=1000;
int turncount=0;//Count number of turns in challenge 3
int turn = 1;//Decide turn type in challenge 4
unsigned char LEDno=48;
void setup() {//Open the serial monitors for
  Serial.begin(9600);
  pinMode(37,INPUT);
  pinMode(36,INPUT);
  pinMode(35,INPUT);
  pinMode(34,INPUT);
}

void loop() {// Main Loop Function
  if (SelectorPosition()==0){ //Check selector is at position 0
    forward(0);//Set wheel speeds to 0
    greenLEDon(LEDno);//Make LED blink
    delay(100);
    greenLEDoff(LEDno);
    delay(100);
    readGroundSensors();//Update ground sensor readings
    for(int i=0;i<4;i++){//For loop to output updated readings to allow for debugging and sensitivity adjustments
      Serial.print("Light sensor: ");
      Serial.print(i);
      Serial.print(" light level ");
      Serial.println(line[i]);
    }
  }
  else if(SelectorPosition()==1){//Check selector is at position 1
    s=30;//Set Custom speed for challenge 1 and execute challenge 1
    Challenge_1();
  }
  else if(SelectorPosition()==2){//Check selector is at position 2
    s=28;//Set Custom speed for challenge 2 and execute challenge 2
    Challenge_2();
  }
  else if(SelectorPosition()==3){//Check selector is at position 3
    if (turncount<15){//Check maximum turns haven't been taken for challenge 3
      s=30;//Set Custom speed for challenge 1 and execute challenge 3
      Challenge_3();
    }
    else if(turncount>=15){//If maximum turns have been taken for challenge 3 stop robot
      forward(s);
      delay(1000);
      s=0;
    }
     
  }
  else if(SelectorPosition()==4){//Check selector is at position 4
     s=30;//Set Custom speed for challenge 1 and execute challenge 4
     Challenge_4();
  }

 else if (SelectorPosition()==5){//Check selector is at position 5
   //Get distance for distance sensors 0-2 & 6-7 and output them for debugging
   Serial.print("Distance of sensor 0 is: ");
   Serial.println(getDistance(0));
   Serial.print("Distance of sensor 6 is: ");
   Serial.println(getDistance(6));
   Serial.print("Distance of sensor 2 is: ");
   Serial.println(getDistance(2));
   Serial.print("Distance of sensor 7 is: ");
   Serial.println(getDistance(7));
   Serial.print("Distance of sensor 1 is: ");
   Serial.println(getDistance(1));
 }
}

//Selector Function
unsigned char SelectorPosition(){
  //Initialise Each bit of the selector
  int sel0=digitalRead(37);
  int sel1=digitalRead(36);
  int sel2=digitalRead(35);
  int sel3=digitalRead(34);
  //iterate through the possible options
  if(sel0==LOW && sel1==LOW && sel2==LOW && sel3==LOW){
    return 0;
  }
  else if(sel0==HIGH && sel1==LOW && sel2==LOW && sel3==LOW){
    return 1;
  }
  else if(sel0==LOW && sel1==HIGH && sel2==LOW && sel3==LOW){
    return 2;
  }
  else if(sel0==HIGH && sel1==HIGH && sel2==LOW && sel3==LOW){
    return 3;
  }
  else if(sel0==LOW && sel1==LOW && sel2==HIGH && sel3==LOW){
    return 4;
  }
  return 5;
}

//Challenge functions
void Challenge_1(){
  readGroundSensors();//Update Ground Sensors
  //ground sensors 1&2> average of black and white values; must be on black line so go forwards
  if ((line[1]>avr(black1,white1))&&(line[2]>avr(black2,white2))){
    forward(s);
  }
  //ground sensor 1<average of black and white values sensor 1 is off the line so turn right to compensate
  if (line[1]<avr(black1,white1)){
    rightTurn(s);
  }
  //ground sensor 2<average of black and white values sensor 1 is off the line so turn left to compensate
  if(line[2]<avr(black2,white2)){
    leftTurn(s);
  }

}

void Challenge_2(){
    readGroundSensors();//Update Ground Sensors
    //ground sensors 1&2< average of black and white values; must be either side of black line so go forwards
    if (line[1]<avr(black1,white1) && line[2]<avr(black2,white2)){
      forward(s);
    }
    //ground sensor 1>average of black and white values sensor 1 is on the line so turn left to compensate
    if (line[1]>avr(black1,white1)){
      leftTurn(s);
    }
    //ground sensor 2>average of black and white values sensor 2 is on the line so turn right to compensate    
    if (line[2]>avr(black2,white2)){
      rightTurn(s);
    }
    //ground sensor 0>average of black and white values sensor 0 is on the line so turn right alot to compensate
    if (line[0]>avr(black0,white0)){
      leftTurn(s*1.1);
    }
    //ground sensor 3>average of black and white values sensor 3 is on the line so turn right alot to compensate
    if (line[3]>avr(black3,white3)){
      rightTurn(s*1.1);
    }
}

void Challenge_3(){
  readGroundSensors();//Update Ground Sensors
  //ground sensor 1 or 2> average of black and white values; a sensor either sensor must be on the black line so is at the border
  if(line[1]>avr(black1,white1) || line[2]>avr(white2,black2)){
    //Reverse from edge, spin randomly and iterate turncount
    backward(s);
    delay(200);
    tightrightTurn(s);
    delay(random(300,1000));
    turncount++;
  }
  else{
    //if not at edge keep going forwards
    forward(s);
  }
  if (getDistance(0)<15){//If sensor 0 is within 15mm of an object
    spin(s);//Execute random spin function
    turncount++;//iterate turncount
  }
  else if (getDistance(7)<15){
    spin(s);//Execute random spin function
    turncount++;//iterate turncount
  }
  else if (getDistance(1)<15){
    spin(s);//Execute random spin function
    turncount++;//iterate turncount
  }
  else{//No object so keep moving forward
    forward(s);
  }
  
}

void Challenge_4(){
    readGroundSensors();//Update ground sensors
    Challenge_2();//Use challenge 2's line following so not to repeat code
    if (turn == 1){//Left Turn Next
      if(getDistance(0)<25 || getDistance(1)<30 ||getDistance(7)<30){//if sensors 0, 1 or 7 are within 30mm of an object
        //Stop, turn left tightly, set turn to 0 and start moving forward
        forward(0);
        delay(900);
        tightleftTurn(s);
        delay(600);
        turn = 0;
        forward(s);
        delay(100);
      }
     }
     if (turn == 0){//Right Turn Next
      if(getDistance(0)<25 || getDistance(1)<30 ||getDistance(7)<30){//if sensors 0, 1 or 7 are within 30mm of an object
        //Stop, turn right tightly, set turn to 1 and start moving forward
        forward(0);
        delay(900);
        tightrightTurn(s);
        delay(500); 
        turn = 1; 
        forward(s);
        delay(100);
      }
     }
     // use side sensors to detect walls and griund desnosrs to check line is gone
     if (getDistance(6)<20 && getDistance(2)<20 && line[1]<avr(black1,white1) && line[2]<avr(white2,black2)){
      int tunnel=1;
      while(tunnel==1){//put bot in tunnel mode (loop)
        if(line[1]<avr(black1,white1) || line[2]<avr(white2,black2)){//If ground sensors detect the box turn off tunnel mode and stop 
          tunnel=0;
          forward(0);
          delay(1000);
        }
        else if(getDistance(6)>100 && getDistance(2)<20){//If left sensors detect no tunnel but right does turn left
          leftTurn(s);
          forward(s);
        }
        else if(getDistance(6)<20 && getDistance(2)>100){//If right sensors detect no tunnel but left does turn right
          rightTurn(s);
          forward(s);
        }
        else{//otherwise go forwards
          forward(s);
        }
      }
     }
    if(line[1]>avr(black1,white1)&&line[0]>avr(black0,white0)){//If ground sensors detect a line go forwards
      s = 25;
      forward(s);
      delay(500);
      s = 30;
    }
}


//Prox Sensor Functions
int readProximitySensor(unsigned char proxIndex){//return value of proximity sensor at given pin
  return analogRead(proxIndex);
}
void proxLEDon(unsigned char proxIndex){//Turn on the LED of a proximity sensor at given pin
  PORTA |= (1<<proxIndex);
}
void proxLEDoff(unsigned char proxIndex){//Turn off the LED of a proximity sensor at given pin
   PORTA &= ~(1<<proxIndex);
}

//Pin Values(6 left, 2 right, 0 straight ahead)
int getDistance(unsigned char sensor){
  //Iterate through Przimity Sensor indexes, turn on their LEDs, read the value and turn the LEDs off again
  for (unsigned char i=0;i<8;i++){
    proxLEDon(sensor);
    delay(10);
    int x=(readProximitySensor(sensor));
    proxLEDoff(sensor);
    delay(10);
    return (4.158e-10*exp(0.02773*x)+3.568*exp(0.002083*x));//Substitute value in Matlab equation to get distance in mm
  }
}

//Ground Sensor Functions
void readGroundSensors(){//Read ground sensors 0,1,2,3 by turing their LEDs on, reading values and turning them off again
  for (int i=0; i<4;i++){
    groundLEDon(i);
    delay(1);
    line[i] = analogRead(8+i);
    groundLEDoff(i);
    }
}
void groundLEDon(unsigned char lineIndex){//Turn a ground sensor LED on with pin
  PORTJ |=(1<<lineIndex);//Set index to ON
}
void groundLEDoff(unsigned char lineIndex){//Turn a ground sensor LED on with pin
  PORTJ &= ~(1<<lineIndex);//Set index to OFF
}
void greenLEDon(unsigned char LEDno){//Turn a green LED on with pin
  digitalWrite(LEDno, LOW);
}
void greenLEDoff(unsigned char LEDno){//Turn a green LED on with pin
  digitalWrite(LEDno, HIGH);
}


//Movement functions
void tightrightTurn(float s){//Left Motor Forward & Right Motor Backward at same speed; turn on spot
  rightMotorBackward(s);
  leftMotorForward(s);
  
}
void tightleftTurn(float s){//Right Motor Forward & Left Motor Backward at same speed; turn on spot
  rightMotorForward(s);
  leftMotorBackward(s);
  
}

void leftTurn(float s){//Right Motor Forward & Left Motor Forward at a reduced speed
  rightMotorForward(s);
  leftMotorForward(s*0.6);
}
void rightTurn(float s){//Left Motor Forward & Right Motor Forward at a reduced speed
  rightMotorForward(s*0.6);
  leftMotorForward(s);
}

void forward(float s){//Right Motor Forward & Left Motor Forward
  rightMotorForward(s);
  leftMotorForward(s);
}

void rightMotorForward(float s){
  analogWrite(2,0);//Stop any backwards rotation
  analogWrite(5,s*0.76);//Rotate Forwards with a compenstaion for broken motor
} 

void leftMotorForward(float s){
  analogWrite(7,0);//Stop any backwards rotation
  analogWrite(6,s);//Rotate Forwards
} 

void backward(float s){//Right Motor Backwards & Left Motor Backwards
  rightMotorBackward(s);
  leftMotorBackward(s);
}

void leftMotorBackward(float s){
  analogWrite(6,0);//Stop any Forwards rotation
  analogWrite(7,s);//Rotate Backwards
}

void rightMotorBackward(float s){
  analogWrite(5,0);//Stop any Forwards rotation
  analogWrite(2,s);//Rotate Backwards
}

void spin(float s){
  //Reverse from object and spin a random amount
  backward(s);
  delay(200);
  tightrightTurn(s);
  delay(random(300,1000));
}

//Other functions
int avr(int B, int W){//Fucntion to take to values and give the average
  return (B+W)/2;
} 
