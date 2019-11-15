void setup() {
  US_Initialize();
  Motion_Initialize();
  Serial.begin(9600); // Starts the serial communication
}

void loop() {
  long distance = US_GetDistance();
  bool motion_seen = Motion_Seen();
  
  // Prints the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Motion: ");
  Serial.println(motion_seen);
}
