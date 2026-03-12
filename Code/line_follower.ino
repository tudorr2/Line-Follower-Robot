/*
  Line Follower with 5 sensors and PID Steering
  For STM32 + L298N Motor Driver
*/

#define S1_PIN PA0  
#define S2_PIN PA1  
#define S3_PIN PA2  
#define S4_PIN PA3  
#define S5_PIN PA4  

#define MOTOR_A_IN1 PB0
#define MOTOR_A_IN2 PB1
#define MOTOR_A_PWM PB6

#define MOTOR_B_IN3 PB10
#define MOTOR_B_IN4 PB11
#define MOTOR_B_PWM PB7

uint32_t threshold = 565;
int baseSpeed = 220;    
int adjustment;
int turnSpeed = 220;


float Kp = 32;   
float Ki = 0.05;     
float Kd = 35;    

// --- PID Variables ---
float error = 0;
float lastError = 0;
float integral = 0;
float derivative = 0;

void setup()
{
  Serial1.begin(9600);
\
 
  pinMode(S1_PIN, INPUT);
  pinMode(S2_PIN, INPUT);
  pinMode(S3_PIN, INPUT);
  pinMode(S4_PIN, INPUT);
  pinMode(S5_PIN, INPUT);

  
  pinMode(MOTOR_A_IN1, OUTPUT);
  pinMode(MOTOR_A_IN2, OUTPUT);
  pinMode(MOTOR_B_IN3, OUTPUT);
  pinMode(MOTOR_B_IN4, OUTPUT);
  pinMode(MOTOR_A_PWM, OUTPUT);
  pinMode(MOTOR_B_PWM, OUTPUT);

  stopMotors();
}

void loop()
{

  bool s1_on = analogRead(S1_PIN) < threshold;
  bool s2_on = analogRead(S2_PIN) < threshold;
  bool s3_on = analogRead(S3_PIN) < threshold;
  bool s4_on = analogRead(S4_PIN) < threshold;
  bool s5_on = analogRead(S5_PIN) < threshold;

  
  error = (s1_on * -4) + (s2_on * -2) + (s3_on * 0) + (s4_on * 2) + (s5_on * 4);
  int numOnLine = s1_on + s2_on + s3_on + s4_on + s5_on;

  if (numOnLine == 0)
  {
    
    integral = 0;
   
    if (lastError > 0) 
    {
      spinRight(turnSpeed);
    }
    else
    {
      spinLeft(turnSpeed);
    }
   
  }
  else
  {
  
    error = error / numOnLine; 
    integral = integral + error;
    derivative = error - lastError;
    lastError = error;
    
    int adjustment = (Kp * error) + (Ki * integral) + (Kd * derivative);

    int leftSpeed = baseSpeed + adjustment;
    int rightSpeed = baseSpeed - adjustment;

    setMotors(leftSpeed, rightSpeed);
  
    Serial1.print("Err:"); Serial1.print(error);
    Serial1.print(" P:"); Serial1.print(Kp * error);
    Serial1.print(" I:"); Serial1.print(Ki * integral);
    Serial1.print(" D:"); Serial1.print(Kd * derivative);
    Serial1.print(" Adj:"); Serial1.println(adjustment);
  }
  
  delayMicroseconds(500);
}


void setMotors(int leftSpeed, int rightSpeed)
{
  leftSpeed = constrain(leftSpeed, 0, 255);
  rightSpeed = constrain(rightSpeed, 0, 255);

  digitalWrite(MOTOR_A_IN1, HIGH); 
  digitalWrite(MOTOR_A_IN2, LOW);
  analogWrite(MOTOR_A_PWM, leftSpeed);

  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);
  analogWrite(MOTOR_B_PWM, rightSpeed);
}

void stopMotors()
{
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, LOW);
  analogWrite(MOTOR_A_PWM, 0);

  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, LOW);
  analogWrite(MOTOR_B_PWM, 0);
}



void spinLeft(int speed)
{
  digitalWrite(MOTOR_A_IN1, LOW);
  digitalWrite(MOTOR_A_IN2, HIGH); 
  analogWrite(MOTOR_A_PWM, speed);

  digitalWrite(MOTOR_B_IN3, HIGH);
  digitalWrite(MOTOR_B_IN4, LOW);  
  analogWrite(MOTOR_B_PWM, speed);
}


void spinRight(int speed)
{
  digitalWrite(MOTOR_A_IN1, HIGH);
  digitalWrite(MOTOR_A_IN2, LOW);  
  analogWrite(MOTOR_A_PWM, speed);

  digitalWrite(MOTOR_B_IN3, LOW);
  digitalWrite(MOTOR_B_IN4, HIGH); 
  analogWrite(MOTOR_B_PWM, speed);
}