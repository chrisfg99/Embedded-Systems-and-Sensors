int line[4];//store black light numbers
float s;//speed or duty cycle
int black0=1020;//change this to change black light sensativity
int black1=1011;
int black2=1015;
int black3=1020;
int white0=1000;//change this to change black light sensativity
int white1=976;
int white2=985;
int white3=1000;
int turncount=0;
unsigned char LEDno=48;
void setup() {
  Serial.begin(9600);
  pinMode(37,INPUT);
  pinMode(36,INPUT);
  pinMode(35,INPUT);
  pinMode(34,INPUT);
}

void loop() {
  if (SelectorPosition()==0){ //reset (1 light will blink)
    //light check when position is 0
    forward(0);
    greenLEDon(LEDno);
    delay(100);
    greenLEDoff(LEDno);
    delay(100);
    readGroundSensors();
    for(int i=0;i<4;i++){
      Serial.print("Light sensor: ");
      Serial.print(i);
      Serial.print(" light level ");
      Serial.println(line[i]);
    }
  }
  else if(SelectorPosition()==1){//challange 1
    //challenge 1 in here
    s=25;
    Challenge_1();
  }
  else if(SelectorPosition()==2){//challange 2
    //challenge 2
    s=25;
    Challenge_2();
  }
  else if(SelectorPosition()==3){//challange 3
    //challange 3
    if (turncount<5){
      s=23;
      Challenge_3();
    }
    else if(turncount>=5){
      forward(s);
      delay(1000);
      s=0;
    }
     
  }
  else if(SelectorPosition()==4){//challange 4
    //chalange 4
    
  }
}

//Selector Function
unsigned char SelectorPosition(){
  int sel0=digitalRead(37);
  int sel1=digitalRead(36);
  int sel2=digitalRead(35);
  int sel3=digitalRead(34);
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
void Challenge_1(){//DONE DON'T EDIT
  s=27;
  readGroundSensors();
    if((line[1]>avr(black1,white1))&&(line[2]>avr(black2,white2))){
      forward(s);
    }
    else if (line[1]<avr(black1,white1)){
      rightTurn(s);
    }
    else if(line[2]<avr(black2,white2)){
      leftTurn(s);
    }
}

void Challenge_2(){//DONE DON'T EDIT
    readGroundSensors();    
    if (line[2]>white2){
      rightTurn(s);
    }
    
    if (line[1]>white1){
      leftTurn(s);
    }
}

void Challenge_3(){
  //Serial.println(getDistance());
  readGroundSensors();
  if(line[1]>avr(black1,white1) || line[2]>avr(white2,black2)){
    tightrightTurn(s);
    delay(random(500,1200));
    turncount++;
  }
  else{
    forward(s);
  }
  if (getDistance()<15){
    tightrightTurn(s);
    delay(random(500,1200));
    turncount++;
  }
  else{
    forward(s);
  }
  
}


//Prox Sensor Functions
int readProximitySensor(unsigned char proxIndex){
  return analogRead(proxIndex);
}
void proxLEDon(unsigned char proxIndex){
  PORTA |= (1<<proxIndex);
}
void proxLEDoff(unsigned char proxIndex){
   PORTA &= ~(1<<proxIndex);
}
int getDistance(){
  for (unsigned char i=0;i<1;i++){
    proxLEDon(i);
    delay(10);
    int x=(readProximitySensor(i));
    proxLEDoff(i);
    delay(10);
    return (4.158e-10*exp(0.02773*x)+3.568*exp(0.002083*x));
  }
}

//Ground Sensor Functions
void readGroundSensors(){//scan ground sensors 0,1,2,3
  for (int i=0; i<4;i++){
    groundLEDon(i);
    delay(1);
    line[i] = analogRead(8+i);
    groundLEDoff(i);
    }
}
void groundLEDon(unsigned char lineIndex){
  PORTJ |=(1<<lineIndex);//Set index to ON
}
void groundLEDoff(unsigned char lineIndex){
  PORTJ &= ~(1<<lineIndex);//Set index to OFF
}
void greenLEDon(unsigned char LEDno){
  digitalWrite(LEDno, LOW);
}
void greenLEDoff(unsigned char LEDno){
  digitalWrite(LEDno, HIGH);
}


//Movement functions
void tightrightTurn(float s){
  leftMotorForward(s);
  rightMotorBackward(s);
}

void leftTurn(float s){//left turn 
  rightMotorForward(s);
  leftMotorForward(s*0.6);
}
void rightTurn(float s){//right turn
  rightMotorForward(s*0.6);
  leftMotorForward(s);
}

void forward(float s){//forward
  rightMotorForward(s);
  leftMotorForward(s);
}

void rightMotorForward(float s){
  analogWrite(2,0);
  analogWrite(5,s);
} 

void leftMotorForward(float s){
  analogWrite(7,0);
  analogWrite(6,s);
} 

void backward(float s){
  rightMotorBackward(s);
  leftMotorBackward(s);
}

void leftMotorBackward(float s){
  analogWrite(6,0);
  analogWrite(7,s);
}

void rightMotorBackward(float s){
  analogWrite(5,0);
  analogWrite(2,s);
}

//Other functions
int avr(int B, int W){
  return (B+W)/2;
}
