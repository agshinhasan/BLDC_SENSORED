const int analogInPin1 = A5; // Analog input pin that the potentiometer is attached to

int sensorValue1 = 0; // value read from the pot
int outputValue1 = 0; // value output to the P

//hall input
int hall_sensor_a = A0;
int hall_sensor_b = A1;
int hall_sensor_c = A2;

//buzzer and LED
//int buzzer = 16;
//int LED = 17;
float throttle = 0.0;

boolean MotorOff = false;

float Cn = 0;

//Motor setup
const int a_coil_lout = 5;
const int a_coil_pwm_hout = 6;
const int b_coil_lout = 7;    
const int b_coil_pwm_hout = 9;
const int c_coil_lout = 8;
const int c_coil_pwm_hout = 10;

enum WheelDirection {
DIR_FORWARD,
DIR_BACKWARD,
DIR_STOP
};

// MOTOR DRIVE
void MoveWheel(WheelDirection (dir), float (speed)) {
if (MotorOff) return;

//empty all motor registers
//half bridge driver, hi part is active high
//lo part is active low
analogWrite(a_coil_pwm_hout, 0);//set motor to stop
analogWrite(b_coil_pwm_hout, 0);
analogWrite(c_coil_pwm_hout, 0);

digitalWrite(a_coil_lout, LOW);
digitalWrite(b_coil_lout, LOW);
digitalWrite(c_coil_lout, LOW);

int hall_a = digitalRead(hall_sensor_a);
int hall_b = digitalRead(hall_sensor_b);
int hall_c = digitalRead(hall_sensor_c);

Serial.print("\r\n");
Serial.print(hall_a);
Serial.print(hall_b);
Serial.print(hall_c);

if (dir == DIR_STOP) {
// do nothing
} else if (dir == DIR_FORWARD) { ///CounterClockwise
if (hall_a == 0 && hall_b == 0 && hall_c == 1) {//001
analogWrite(b_coil_pwm_hout, speed);
digitalWrite(c_coil_lout, HIGH);
} else if (hall_a == 0 && hall_b == 1 && hall_c == 1) {//011
analogWrite(a_coil_pwm_hout, speed);
digitalWrite(c_coil_lout, HIGH);
} else if (hall_a == 0 && hall_b == 1 && hall_c == 0) {//010
analogWrite(a_coil_pwm_hout, speed);
digitalWrite(b_coil_lout, HIGH);
}else if (hall_a == 1 && hall_b == 1 && hall_c == 0) {//110
analogWrite(c_coil_pwm_hout, speed);
digitalWrite(b_coil_lout, HIGH);
} else if (hall_a == 1 && hall_b == 0 && hall_c == 0) {//100
analogWrite(c_coil_pwm_hout, speed);
digitalWrite(a_coil_lout, HIGH);
}else if (hall_a == 1 && hall_b == 0 && hall_c == 1) {//101
analogWrite(b_coil_pwm_hout, speed);
digitalWrite(a_coil_lout, HIGH);
} 
}
else if (dir == DIR_BACKWARD) { //Clockwise
  if (hall_a == 1 && hall_b == 0 && hall_c == 1) {//101
analogWrite(a_coil_pwm_hout, speed);
digitalWrite(b_coil_lout, HIGH);
} else if (hall_a == 1 && hall_b == 0 && hall_c == 0) {//100
analogWrite(a_coil_pwm_hout, speed);
digitalWrite(c_coil_lout, HIGH);
} else if (hall_a == 1 && hall_b == 1 && hall_c == 0) {//110
analogWrite(b_coil_pwm_hout, speed);
digitalWrite(c_coil_lout, HIGH);
} else if (hall_a == 0 && hall_b == 1 && hall_c == 0) {//010
analogWrite(b_coil_pwm_hout, speed);
digitalWrite(a_coil_lout, HIGH);
} else if (hall_a == 0 && hall_b == 1 && hall_c == 1) {//011
analogWrite(c_coil_pwm_hout, speed);
digitalWrite(a_coil_lout, HIGH);
}else if (hall_a == 0 && hall_b == 0 && hall_c == 1) {//001
analogWrite(c_coil_pwm_hout, speed);
digitalWrite(b_coil_lout, HIGH);
}
}
delay(10);
}

void setup()
{
Serial.begin(115200);
pinMode(a_coil_lout, OUTPUT);
pinMode(a_coil_pwm_hout, OUTPUT);
pinMode(b_coil_lout, OUTPUT);
pinMode(b_coil_pwm_hout, OUTPUT);
pinMode(c_coil_lout, OUTPUT);
pinMode(c_coil_pwm_hout, OUTPUT);

pinMode(hall_sensor_a, INPUT);
pinMode(hall_sensor_b, INPUT);
pinMode(hall_sensor_c, INPUT);

//half bridge driver, hi part is active high
// lo part is active low
analogWrite(a_coil_pwm_hout, 0);//set motor to stop
analogWrite(b_coil_pwm_hout, 0);
analogWrite(c_coil_pwm_hout, 0);

digitalWrite(a_coil_lout, LOW);
digitalWrite(b_coil_lout, LOW);
digitalWrite(c_coil_lout, LOW);

}

void loop()
{
  
// read the tuning knob value:
for(int i=0;i<5;i++){ 
  sensorValue1 += analogRead (analogInPin1); 
} 
sensorValue1 = sensorValue1/5; // map tuning knob to the range of the analog out:
outputValue1 = map(sensorValue1, 0, 1023, -250, 250); //PID CALC
Cn = outputValue1; //MOTOR DRIVE
WheelDirection dir;
if (Cn > 0)
dir = DIR_FORWARD;
else if (Cn < -0)
dir = DIR_BACKWARD;
else
dir = DIR_STOP;
throttle = abs(Cn);
MoveWheel(dir, throttle);
Serial.println (throttle);

}
