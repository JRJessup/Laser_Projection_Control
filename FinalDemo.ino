// Decoder
byte MSB_x = 0;
byte SecondByte_x = 0;
byte ThirdByte_x = 0;
byte LSB_x = 0;

byte secondOld_x = 0;
byte secondNew_x = 0;
byte thirdNew_x = 0;
byte thirdOld_x = 0;
byte msbOld_x = 0;
byte msbNew_x = 0;
byte lsbNew_x = 0;
byte lsbOld_x = 0;

double multiplier_x = 0;
double tempValue_x = 0;
double result_x = 0;

byte MSB_y = 0;
byte SecondByte_y = 0;
byte ThirdByte_y = 0;
byte LSB_y = 0;

byte secondOld_y = 0;
byte secondNew_y = 0;
byte thirdNew_y = 0;
byte thirdOld_y = 0;
byte msbOld_y = 0;
byte msbNew_y = 0;
byte lsbNew_y = 0;
byte lsbOld_y = 0;

double multiplier_y = 0;
double tempValue_y = 0;
double result_y = 0;


// Bottom Motor
const int d0 = 22; 
const int d1 = 23;
const int d2 = 24;
const int d3 = 25;
const int d4 = 26;
const int d5 = 27;
const int d6 = 28;
const int d7 = 29;

const int select1_x = 34; 
const int select2_x = 35; 
const int outputEnable_x = 41; 

const int pwm_x = 2;
const int dir1_x = 5;
const int dir2_x = 4;

double count_x = 0;
double angle_x = 0;
double lastAngle_x = 0;
double lastError_x = 0;
double setpoint_x = 0;
double sumError_x = 0;
double scaledPidTerm_x = 0;

double Kp_x = 14;
double Ki_x = 20;
double Kd_x = 2;


// Top Motor
const int c0 = 44;
const int c1 = 45;
const int c2 = 46;
const int c3 = 47;
const int c4 = 48;
const int c5 = 49;
const int c6 = 50;
const int c7 = 51;

const int select1_y = 52; 
const int select2_y = 53; 
const int outputEnable_y = 43; 

const int pwm_y = 10;
const int dir1_y = 8;
const int dir2_y = 9;

double count_y = 0;
double angle_y = 0;
double lastAngle_y = 0;
double lastError_y = 0;
double setpoint_y = 0;
double sumError_y = 0;
double scaledPidTerm_y = 0;

double Kp_y = 9.45;
double Ki_y = 29.9;
double Kd_y = 0.317;


const int laserPin = 13;


void setup() {
    Serial.begin(115200);

    // Bottom Motor
    pinMode(d0, INPUT);
    pinMode(d1, INPUT);
    pinMode(d2, INPUT);
    pinMode(d3, INPUT);
    pinMode(d4, INPUT);
    pinMode(d5, INPUT);
    pinMode(d6, INPUT);
    pinMode(d7, INPUT);

    pinMode(pwm_x, OUTPUT);
    pinMode(dir1_x, OUTPUT);
    pinMode(dir2_x, OUTPUT);
    pinMode(select1_x, OUTPUT);
    pinMode(select2_x, OUTPUT);
    pinMode(outputEnable_x, OUTPUT);


    // Top Motor
    pinMode(c0, INPUT);
    pinMode(c1, INPUT);
    pinMode(c2, INPUT);
    pinMode(c3, INPUT);
    pinMode(c4, INPUT);
    pinMode(c5, INPUT);
    pinMode(c6, INPUT);
    pinMode(c7, INPUT);

    pinMode(pwm_y, OUTPUT);
    pinMode(dir1_y, OUTPUT);
    pinMode(dir2_y, OUTPUT);
    
    pinMode(select1_y, OUTPUT);
    pinMode(select2_y, OUTPUT);
    pinMode(outputEnable_y, OUTPUT);

    pinMode(laserPin, OUTPUT);
    digitalWrite(laserPin, HIGH);

    
}

void loop() {

    getAllBytesForCount_x();
//    getCount_x();

    getAllBytesForCount_y();
//    getCount_y();

    count_x = (ThirdByte_x << 8) | LSB_x;
    angle_x = count_x * 0.9;

    count_y = (ThirdByte_y << 8) | LSB_y;
    angle_y = count_y * 0.9;
    
    findBottomPID();
    findTopPID(); 
    
    writeToPWM_Bottom();
    writeToPWM_Top();
    
    Serial.println(angle_y);
}


void getAllBytesForCount_x() {
    digitalWrite(outputEnable_x, LOW);

    digitalWrite(select1_x, LOW);
    digitalWrite(select2_x, HIGH);

    MSB_x = Get_MSB_x();

    digitalWrite(select1_x, HIGH);
    digitalWrite(select2_x, HIGH);

    SecondByte_x = Get_2nd_x();

    digitalWrite(select1_x, LOW);
    digitalWrite(select2_x, LOW);

    ThirdByte_x = Get_3rd_x();

    digitalWrite(select1_x, HIGH);
    digitalWrite(select2_x, LOW);

    LSB_x = Get_LSB_x();

    digitalWrite(outputEnable_x, HIGH);
}

void getAllBytesForCount_y() {
    digitalWrite(outputEnable_y, LOW);

    digitalWrite(select1_y, LOW);
    digitalWrite(select2_y, HIGH);

    MSB_y = Get_MSB_y();

    digitalWrite(select1_y, HIGH);
    digitalWrite(select2_y, HIGH);

    SecondByte_y = Get_2nd_y();

    digitalWrite(select1_y, LOW);
    digitalWrite(select2_y, LOW);

    ThirdByte_y = Get_3rd_y();

    digitalWrite(select1_y, HIGH);
    digitalWrite(select2_y, LOW);

    LSB_y = Get_LSB_y();

    digitalWrite(outputEnable_y, HIGH);
}


void getCount_x() {
    multiplier_x = 1;
    tempValue_x = LSB_x * multiplier_x; 
    count_x = tempValue_x;

    multiplier_x = multiplier_x * 256;
    tempValue_x = ThirdByte_x * multiplier_x; 
    count_x = count_x + tempValue_x;

    multiplier_x = multiplier_x * 256;
    tempValue_x = SecondByte_x * multiplier_x;  
    count_x = count_x + tempValue_x;

    multiplier_x = multiplier_x * 256;
    tempValue_x = MSB_x * multiplier_x; 
    count_x = count_x + tempValue_x;
}

void getCount_y() {
    multiplier_y = 1;
    tempValue_y = LSB_y * multiplier_y; 
    count_y = tempValue_y;

    multiplier_y = multiplier_y * 256;
    tempValue_y = ThirdByte_y * multiplier_y; 
    count_y = count_y + tempValue_y;

    multiplier_y = multiplier_y * 256;
    tempValue_y = SecondByte_y * multiplier_y;  
    count_y = count_y + tempValue_y;

    multiplier_y = multiplier_y * 256;
    tempValue_y = MSB_y * multiplier_y; 
    count_y = count_y + tempValue_y;
}

void findBottomPID() {    
    double dAngle = angle_x - lastAngle_x;
    double error = setpoint_x - angle_x;                                  
    double dError = error - lastError_x;
    sumError_x += error;  

    if (abs(error) < 1) {
        sumError_x = 0;
    }
    if (abs(error) > 5) {
        sumError_x = 0;
    }
    
    double pid = (Kp_x * error) + (Ki_x * sumError_x) + (Kd_x * dAngle);
    pid = constrain(pid, -255, 255);
    scaledPidTerm_x = abs(pid);

    lastAngle_y = angle_x;
    lastError_y = error;
}

void findTopPID() {
    double dAngle = angle_y - lastAngle_y;
    double error = setpoint_y - angle_y;
    double dError = error - lastError_y;
    sumError_y += error;

    if (abs(error) < 1) {
        sumError_y = 0;
    } else if (abs(error) > 5) {
        sumError_y = 0;
    }

    double pid = (Kp_y * error) + (Ki_y * sumError_y) + (Kd_y * dAngle);
    pid = constrain(pid, -255, 255);
    scaledPidTerm_y = abs(pid);

    lastAngle_y = angle_y;
    lastError_y = error;
}

void writeToPWM_Bottom() {
    if (angle_x >= setpoint_x) {
      digitalWrite(dir1_x, HIGH);  
      digitalWrite(dir2_x, LOW);
      setpoint_x = 0;
      
    } else if (angle_x <= setpoint_x) {
      digitalWrite(dir1_x, LOW); 
      digitalWrite(dir2_x, HIGH);
      setpoint_x = 10;
    }

    analogWrite(pwm_x, scaledPidTerm_x); 
}

void writeToPWM_Top() {
    if (angle_y >= setpoint_y) {
        digitalWrite(dir1_y, HIGH);  
        digitalWrite(dir2_y, LOW);
        setpoint_y = -15;
    
    } else if (angle_y <= setpoint_y) {
        digitalWrite(dir1_y, LOW); 
        digitalWrite(dir2_y, HIGH);
        setpoint_y = 0;
    }

    analogWrite(pwm_y, scaledPidTerm_y); 
}


byte Get_MSB_x() {
    msbOld_x = Read_Byte_x();  
    msbNew_x = Read_Byte_x(); 

    if (msbNew_x == msbOld_x) {
        MSB_x = msbNew_x;  
        return MSB_x;
    }
    else {
        Get_MSB_x(); 
    }
}

byte Get_2nd_x() {
    secondOld_x = Read_Byte_x();  
    secondNew_x = Read_Byte_x();  

    if (secondNew_x == secondOld_x) {
        SecondByte_x = secondNew_x;  
        return SecondByte_x;
    }
    else {
        Get_2nd_x();
    }
}

byte Get_3rd_x() {
    thirdOld_x = Read_Byte_x();  
    thirdNew_x = Read_Byte_x(); 

    if (thirdNew_x == thirdOld_x) {
        ThirdByte_x = thirdNew_x; 
        return ThirdByte_x;
    }
    else {
        Get_3rd_x();
    }
}

byte Get_LSB_x() {
    lsbOld_x = Read_Byte_x(); 
    lsbNew_x = Read_Byte_x();  

    if (lsbNew_x == lsbOld_x) {
        LSB_x = lsbNew_x;  
        return LSB_x;
    }
    else {
        Get_LSB_x();
    }
}

byte Read_Byte_x() {
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


byte Get_MSB_y() {
    msbOld_y = Read_Byte_y();  
    msbNew_y = Read_Byte_y(); 

    if (msbNew_y == msbOld_y) {
        MSB_y = msbNew_y;  
        return MSB_y;
    }
    else {
        Get_MSB_y(); 
    }
}

byte Get_2nd_y() {
    secondOld_y = Read_Byte_y();  
    secondNew_y = Read_Byte_y();  

    if (secondNew_y == secondOld_y) {
        SecondByte_y = secondNew_y;  
        return SecondByte_y;
    }
    else {
        Get_2nd_y();
    }
}

byte Get_3rd_y() {
    thirdOld_y = Read_Byte_y();  
    thirdNew_y = Read_Byte_y(); 

    if (thirdNew_y == thirdOld_y) {
        ThirdByte_y = thirdNew_y; 
        return ThirdByte_y;
    }
    else {
        Get_3rd_y();
    }
}

byte Get_LSB_y() {
    lsbOld_y = Read_Byte_y(); 
    lsbNew_y = Read_Byte_y();  

    if (lsbNew_y == lsbOld_y) {
        LSB_y = lsbNew_y;  
        return LSB_y;
    }
    else {
        Get_LSB_y();
    }
}

byte Read_Byte_y() {
    byte requestedByte = 0;

    bitWrite(requestedByte, 0, digitalRead(c0));
    bitWrite(requestedByte, 1, digitalRead(c1));
    bitWrite(requestedByte, 2, digitalRead(c2));
    bitWrite(requestedByte, 3, digitalRead(c3));
    bitWrite(requestedByte, 4, digitalRead(c4));
    bitWrite(requestedByte, 5, digitalRead(c5));
    bitWrite(requestedByte, 6, digitalRead(c6));
    bitWrite(requestedByte, 7, digitalRead(c7));

    return requestedByte;
}
