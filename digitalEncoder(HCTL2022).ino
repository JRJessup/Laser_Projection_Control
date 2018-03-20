double newResult = 0;

const int ChannelA = 1; //Channels of Encoders (A=Pin10 & B=Pin9)
const int ChannelB = 2; // --> MAY NOT BE CONNECTED

const int select1 = 3; //Controls Reading from Output Buffer
const int select2 = 4; // (select1=Pin4 & select2=Pin17)

const int reset = 5; //Active-Low Reset (Asynchronous) --> MAY NOT NEED CONNECTED
const int outputEnable = 6; //Active-Low Enable

const int d0 = 7; //Tri-State Output Bus
const int d1 = 8;
const int d2 = 9;
const int d3 = 10;
const int d4 = 11;
const int d5 = 12;
const int d6 = 13;
const int d7 = 14;

double MSB = 0;
double SecondByte = 0;
double ThirdByte = 0;
double LSB = 0;

double multiplier = 0;
double tempValue = 0;
double result = 0;


void setup() {
  pinMode(d0, INPUT);
  pinMode(d1, INPUT);
  pinMode(d2, INPUT);
  pinMode(d3, INPUT);
  pinMode(d4, INPUT);
  pinMode(d5, INPUT);
  pinMode(d6, INPUT);
  pinMode(d7, INPUT);

  pinMode(select1, OUTPUT);
  pinMode(select2, OUTPUT);
  pinMode(reset, OUTPUT);
  pinMode(outputEnable, OUTPUT);
}

void loop() {
  digitalWrite(outputEnable, HIGH);
  delay (25);

  digitalWrite(select1, LOW); //Reset SEL1
  digitalWrite(select2, HIGH); //Set SEL2
  digitalWrite(outputEnable, LOW); //Reset OE

  Get_MSB(); //Get MostSignificantByte

  digitalWrite(select1, HIGH);
  digitalWrite(select2, HIGH);

  Get_2nd();

  digitalWrite(select1, LOW);
  digitalWrite(select2, LOW);

  Get_3rd();

  digitalWrite(select1, HIGH);
  digitalWrite(select2, LOW);

  Get_LSB();

  digitalWrite(outputEnable, HIGH);
  delay(25);

  multiplier = 1;
  tempValue = LSB * multiplier;  //Assign LSB
  result = tempValue;

  multiplier = multiplier * 256;
  tempValue = ThirdByte * multiplier;  //Assign ThirdByte
  result = result + tempValue;

  multiplier = multiplier * 256;
  tempValue = SecondByte * multiplier;  //Assign SecondByte
  result = result + tempValue;

  multiplier = multiplier * 256;
  tempValue = MSB * multiplier;  //Assign MSB
  result = result + tempValue;

  // result == 32-BIT COUNT DATA i.e WHAT IS READ TO ARDUINO AND USED
}


double Get_MSB() {
  msbOld = PinA;  //Get Current Data --> WHAT IS PINA
  msbNew = PinA;  //Get 2nd Data

  if (msbNew == msbOld) {
    MSB = msbNew;  //Get Stable Data
  }
  else {
    Get_MSB();  //Says GoTo Get_2nd in Datasheet. Maybe Typo? Check w/ Tony
  }
}

double Get_2nd() {
  secondOld = PinA;  //Get Current Data --> WHAT IS PINA
  secondNew = PinA;  //Get 2nd Data

  if (secondNew == secondOld) {
    SecondByte = secondNew;  //Get Stable Data
  }
  else {
    Get_2nd();
  }
}

double Get_3rd() {
  thirdOld = PinA;  //Get Current Data --> WHAT IS PINA
  thirdNew = PinA;  //Get 2nd Data

  if (thirdNew == thirdOld) {
    ThirdByte = thirdNew;  //Get Stable Data
  }
  else {
    Get_3rd();
  }
}

double Get_LSB() {
  lsbOld = PinA;  //Get Current Data --> WHAT IS PINA
  lsbNew = PinA;  //Get 2nd Data

  if (lsbNew == lsbOld) {
    LSB = lsbNew;  //Get Stable Data
  }
  else {
    Get_LSB();
  }
}
