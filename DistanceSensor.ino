/*
 * HC-SR04 example sketch
 *
 * https://create.arduino.cc/projecthub/Isaac100/getting-started-with-the-hc-sr04-ultrasonic-sensor-036380
 *
 * by Isaac100
 */
 //
#include <math.h>
const int trigPin = 9;
const int echoPin = 10;

float duration, distance;

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
  }
  std_dev = standard_Deviation(values, cycles);
}

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}


void loop() {
  float standardDev = 0;
  set_Std_Dev(standardDev,20);
  Serial.print("Standard Deviation is: ");
  Serial.println(standardDev);

  // if the standard deviation is less than 1 
  
}
