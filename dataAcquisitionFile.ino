// variables are declared here:

float speed = 0;
unsigned long timep, time, etime;

long last_angle = 0;
float angle_moved = 0;

const int select1 = 34; //Controls Reading from Output Buffer
const int select2 = 35; // (select1=Pin4 & select2=Pin17)

//const int reset = 5; //Active-Low Reset (Asynchronous) --> MAY NOT NEED CONNECTED
const int outputEnable = 41; //Active-Low Enable

const int d0 = 22; //Tri-State Output Bus
const int d1 = 23;
const int d2 = 24;
const int d3 = 25;
const int d4 = 26;
const int d5 = 27;
const int d6 = 28;
const int d7 = 29;

int i = 0;

byte MSB = 0;
byte SecondByte = 0;
byte ThirdByte = 0;
byte LSB = 0;

byte thirdNew = 0;
byte thirdOld = 0;
byte lsbNew = 0;
byte lsbOld = 0;
byte secondOld = 0;
byte secondNew = 0;
byte msbOld = 0;
byte msbNew = 0;

double multiplier = 0;
double tempValue = 0;
double result = 0;

double timeOne = 0;
double timeTwo = 0;
double timeLoop = 0;

float derivativeSpeed = 0;
float lagAngle = 0;
float derivativeError = 0;
float summedChangeError = 0;
float angleDiff = 0;
float lastAngle = 0;
float lastError = 0;
float error = 0;
float changeError = 0;
float totalError = 0;
float pidTerm = 0;
float scaledPidTerm = 0; // If value given is > 255, scale to proper range.

long  count = 0; // Count of Encoder (400CPR)
long  angle = 0; // Angle of Rotation

double    pwm = 11; // this is the PWM pin for the motor for how much we move it to correct for its error
const int dir1 = 3; // V+ and V- of Motor. Controlling Clockwise and Counter-Clockwise Rotation
const int dir2 = 4;

void setup()
{
  Serial.begin(115200);

  pinMode(d0, INPUT);
  pinMode(d1, INPUT);
  pinMode(d2, INPUT);
  pinMode(d3, INPUT);
  pinMode(d4, INPUT);
  pinMode(d5, INPUT);
  pinMode(d6, INPUT);
  pinMode(d7, INPUT);
  
  pinMode(pwm, OUTPUT);
  pinMode(dir1, OUTPUT);
  pinMode(dir2, OUTPUT);

  pinMode(select1, OUTPUT);
  pinMode(select2, OUTPUT);
//  pinMode(reset, OUTPUT);
  pinMode(outputEnable, OUTPUT);
}

void loop()
{
  time = micros();// this timer is used for the angle to be refreshed every 1s
  angle = count * 0.9;//. count to angle conversion
  etime = time - timep;
  
   digitalWrite(outputEnable, LOW);
  //delay (25);

  digitalWrite(select1, LOW); //Reset SEL1
  digitalWrite(select2, HIGH); //Set SEL2
  //digitalWrite(outputEnable, LOW); //Reset OE
  
  MSB = Get_MSB(); //Get MostSignificantByte
  
  digitalWrite(select1, HIGH);
  digitalWrite(select2, HIGH);
  
  SecondByte = Get_2nd();

  digitalWrite(select1, LOW);
  digitalWrite(select2, LOW);
  //digitalWrite(outputEnable, LOW); //Reset OE

  ThirdByte = Get_3rd();

  digitalWrite(select1, HIGH);
  digitalWrite(select2, LOW);

  LSB = Get_LSB();

  digitalWrite(outputEnable, HIGH);
  //delay(25);

  multiplier = 1;
  tempValue = LSB * multiplier;  //Assign LSB
  count = tempValue;

  multiplier = multiplier * 256;
  tempValue = ThirdByte * multiplier;  //Assign ThirdByte
  count = count + tempValue;

  multiplier = multiplier * 256;
  tempValue = SecondByte * multiplier;  //Assign SecondByte
  count = count + tempValue;
  
  multiplier = multiplier * 256;
  tempValue = MSB * multiplier;  //Assign MSB
  count = count + tempValue;

  //count == 32-BIT COUNT DATA i.e WHAT IS READ TO ARDUINO AND USED

  // count = (MSB<<24) | (SecondByte<<16) | (ThirdByte<<8) | LSB;
  count = double (count);
  
  digitalWrite(dir1,LOW) ;
  digitalWrite(dir2,HIGH) ;
  analogWrite(pwm,255) ;
  
  if (etime > 5)
   {
    //angle = angle%360;
    //count = count%400;
    
    angle_moved = (angle - last_angle)*(3.14159/180);
    speed = abs(angle_moved/etime)*1000000; //0.001784
    
    // Serial.print(" ");
    Serial.print(time);
    Serial.print("     ");
    // Serial.print(angle);
    // Serial.print("    ");
    Serial.print(speed);
    Serial.print("\n");
  
    timep = time;  
    last_angle = angle;
  }
  
}

byte Get_MSB() {
    msbOld = Read_Byte();  //Get Current Data --> WHAT IS PINA
    msbNew = Read_Byte();  //Get 2nd Data

  if (msbNew == msbOld) {
    MSB = msbNew;  //Get Stable Data
    return MSB;
  }
  else {
    Get_MSB();
  }
}
//
byte Get_2nd() {
    secondOld = Read_Byte();  //Get Current Data --> WHAT IS PINA
    secondNew = Read_Byte();  //Get 2nd Data

  if (secondNew == secondOld) {
    SecondByte = secondNew;  //Get Stable Data
    return SecondByte;
  }
  else {
    Get_2nd();
  }
}


byte Get_3rd() {
    thirdOld = Read_Byte();  //Get Current Data --> WHAT IS PINA
    thirdNew = Read_Byte();  //Get 2nd Data

  if (thirdNew == thirdOld) {
    ThirdByte = thirdNew;  //Get Stable Data
    return ThirdByte;
  }
  else {
    Get_3rd();
  }
}

byte Get_LSB() {
  lsbOld = Read_Byte();  //Get Current Data --> WHAT IS PINA
  lsbNew = Read_Byte();  //Get 2nd Data

  if (lsbNew == lsbOld) {
    LSB = lsbNew;  //Get Stable Data
    return LSB;
  }
  else {
    Get_LSB();
  }
}


byte Read_Byte() {
  byte requestedByte = 0;

  bitWrite(requestedByte, 0, digitalRead(22));
  bitWrite(requestedByte, 1, digitalRead(23));
  bitWrite(requestedByte, 2, digitalRead(24));
  bitWrite(requestedByte, 3, digitalRead(25));
  bitWrite(requestedByte, 4, digitalRead(26));
  bitWrite(requestedByte, 5, digitalRead(27));
  bitWrite(requestedByte, 6, digitalRead(28));
  bitWrite(requestedByte, 7, digitalRead(29));
  

  return requestedByte;
}

