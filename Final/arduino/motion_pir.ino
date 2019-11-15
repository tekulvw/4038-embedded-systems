#define MOTION_OUT 6

void Motion_Initialize() {
  pinMode(MOTION_OUT, OUTPUT);
}

bool Motion_Seen() {
  return digitalRead(MOTION_OUT) == HIGH;
}
