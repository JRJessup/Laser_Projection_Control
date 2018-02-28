// variables are declared here:
long count = 0;
long angle = 0;
unsigned long programTime, loopTime, refreshTime;

byte state, prevState;

volatile int A,B;
int last_angle = 0;
int angle_moved = 0;

void setup()
{
  Serial.begin(115200); //faster bit rate for debugging

  pinMode(2, INPUT);
  pinMode(3, INPUT);
//  attachInterrupt(digitalPinToInterrupt(2), Achange, CHANGE); //alternative method of defining interrupts
//  attachInterrupt(digitalPinToInterrupt(3), Bchange, CHANGE);
  attachInterrupt(0,Achange,CHANGE); //interrupt pins declared here
  attachInterrupt(1,Bchange,CHANGE);
  programTime = millis();
}

void loop()
{
  loopTime = millis();// this timer is used for the angle to be refreshed every'n' seconds (dictated by refreshTime)
  angle = count * 0.9;//. count to angle conversion
  refreshTime = loopTime - programTime;
  if (refreshTime > 1000)
  {
    angle = angle%360;
    count = count%400;

    Serial.print("Count: ");
    Serial.print(count);
    Serial.print("  Angle: ");
    Serial.print(angle);
    Serial.print("\n");

    programTime = loopTime; //Equalizes program and loopTime, in order to restart Refresh Timer
  }

}

void Achange()
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;// switch...case method used here
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  switch (state)
  {
    case 1:
    {
      if (prevState == 2) count++;
      if (prevState == 4) count--;
      break;
    }
    case 2:
    {
      if (prevState == 1) count--;
      if (prevState == 3) count++;
      break;
    }
    case 3:
    {
      if (prevState == 2) count --;
      if (prevState == 4) count ++;
      break;
    }
    default:
    {
      if (prevState == 1) count++;
      if (prevState == 3) count--;
    }
  }
  prevState = state;
}

void Bchange()
{
  A = digitalRead(2);
  B = digitalRead(3);

  if ((A==HIGH)&&(B==HIGH)) state = 1;
  if ((A==HIGH)&&(B==LOW)) state = 2;
  if ((A==LOW)&&(B==LOW)) state = 3;
  if((A==LOW)&&(B==HIGH)) state = 4;
  switch (state)
  {
    case 1:
    {
      if (prevState == 2) count++;
      if (prevState == 4) count--;
      break;
    }
    case 2:
    {
      if (prevState == 1) count--;
      if (prevState == 3) count++;
      break;
    }
    case 3:
    {
      if (prevState == 2) count --;
      if (prevState == 4) count ++;
      break;
    }
    default:
    {
      if (prevState == 1) count++;
      if (prevState == 3) count--;
    }
  }
  prevState = state;
}
