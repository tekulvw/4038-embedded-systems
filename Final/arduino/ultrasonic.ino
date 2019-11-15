#define TRIG 8
#define ECHO 7

void US_Initialize(void) {
  pinMode(TRIG, OUTPUT); // Sets the trigPin as an Output
  pinMode(ECHO, INPUT); // Sets the echoPin as an Input
}

long US_GetDistance() {
  long duration;
  int distance;
  
  // Clears the trigPin
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO, HIGH);
  
  // Calculating the distance
  distance= duration*0.034/2;
  return distance;
}
