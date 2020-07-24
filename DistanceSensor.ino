#include <Servo.h>
#include <math.h>

Servo left_servo, right_servo, roto_servo1;
const int trigPin = 9;
const int echoPin = 10;
int mapping = 0;
int pos1 = 0;    // variable to store the servo position
/*States:
 * Std dev ! < 1 or (std dev <1 and dist > 5cm) drive forward
 * Found std dev < 1 and dist < 5cm -> Mark, turn record
 */
float duration, distance;
int state = 0;//Not at wall and not 
// Helper function to calculate mean
float averageValues(float * distances, int dist_len)
{
  float sum = 0;
  for(int i = 0; i < dist_len; i++)
  {
    sum += distances[i];
  }
  return sum/dist_len;
}

// Calculate standard deviation
float standard_Deviation(float * distances, int dist_len)
{
  float average = averageValues(distances,dist_len);
  //calculate summation
  float summation = 0;
  for (int i = 0; i < dist_len; i++){
    summation = summation + (distances[i] - average)*(distances[i] - average);
  }
  return sqrt(abs(summation/dist_len));
  
}
//Sample 5 scans and calculate std dev
void set_Std_Dev( float& std_dev, int cycles )
{
  float values[cycles];
  for(int i =0; i < cycles; i++){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    // sends out an 8 cycle sonic burst from the transmitter, 
    // which then bounces of an object and hits the receiver
    // Which is connected to the Echo Pin
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
  
    duration = pulseIn(echoPin, HIGH);
    distance = (duration*.0343)/2;
    delay(5);
    values[i] = distance;
    Serial.println(distance);
  }
  std_dev = standard_Deviation(values, cycles);
}


//Simple driver method to, well...Drive.
void Drive(int lSpeed, int rSpeed)
{
  left_servo.write(lSpeed);
  right_servo.write(rSpeed);
}
void FoundWallTurn()
{
  
}
void FoundCornerTurn()
{
  
}
void GetDistance()
{
  
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  roto_servo1.attach(5);  // attaches the servo on pin 9 to the servo object
  left_servo.attach(6); // left wheel the servo on pin 7 
  right_servo.attach(7); // right wheel the servo on pin 8 
  Serial.begin(9600);
}


void loop() {
  float standardDevSide = 0;
  float standardDevFront = 0;
  set_Std_Dev(standardDevFront,20);
  Serial.print("Standard Deviation is: ");
  Serial.println(standardDevFront);
  
  // if the standard deviation is less than 1 then object found, make a dot 
  if(standardDevFront >= 1)
  {
     Drive(20,20);
  }
  //Found a wall, turn
  else if(standardDevFront < 1 && mapping == 0)
  {
    FoundWallTurn();
    mapping = 1; //Set mapping to 1
  }
  //Found a corner (Theoretically, should be mapping but, you know, be weary of theory.)
  else if(standardDevFront < 1 && standardDevSide < 1 && mapping)
  {
    FoundCornerTurn();
    //turning logic... Coming soon to a theater near you
    //if side std dev is less than 1 and front is also.  
    //If side is at std dev < 1 and at a distance of 5cm then stop
  }
  else{
    //Error handling here
    
  }

  for (pos1 = 0; pos1 <= 180; pos1 += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    roto_servo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos1 = 180; pos1 >= 0; pos1 -= 1) { // goes from 180 degrees to 0 degrees
    roto_servo1.write(pos1);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }

  
}
