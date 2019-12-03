#include <Servo.h>
#include <NewPing.h>

#define MOTION_PIN 6

#define SONAR_TRIG 8
#define SONAR_ECHO 7
#define SONAR_MAX_DISTANCE 500            // cm
#define SONAR_TRIGGER_STOP_DISTANCE 30    // cm
#define SONAR_TRIGGER_RETRACT_DISTANCE 10 // cm

NewPing sonar(SONAR_TRIG, SONAR_ECHO);

unsigned int pingSpeed = 50; // How frequently are we going to send out a ping (in milliseconds). 50ms would be 20 times a second.
unsigned long pingTimer;     // Holds the next ping time.

#define LID_SERVO_PIN 9
#define LID_SERVO_CLOSED 0
#define LID_SERVO_OPEN 180
#define LID_SERVO_SPEED 270 // degrees per second

#define ARM_SERVO_PIN 10
#define ARM_SERVO_CLOSED 0
#define ARM_SERVO_OPEN 90
#define ARM_SERVO_SPEED_FAST 30 //degrees per second
#define ARM_SERVO_SPEED_SLOW 5  //degrees per second

// WARNING: Max servo speed is approx. 270 deg per second

Servo lidServo;
Servo armServo;

#define TOGGLE_BUTTON_PIN 5
#define BASIC_STAMP_PIN 3

enum ArmAction
{
  NO_ARM_ACTION,
  FAST_TRIGGER,
  SLOW_TRIGGER,
  TRIGGERED,
  FAST_WITHDRAW,
  SLOW_WITHDRAW,
  RETRACTED,
};

enum LidAction
{
  NO_LID_ACTION,
  OPENING,
  OPEN,
  CLOSING,
  CLOSED,
};

typedef struct BoxState
{
  bool sonarFoundObject;
  bool lastSonarFoundObject;
  int sonarDistance;

  bool motionFound;
  bool lastMotionFound;

  ArmAction lastArmState;
  ArmAction armState;
  int armTarget;
  unsigned long armTimer;

  LidAction lidState;
  int lidTarget;
  unsigned long lidTimer;

  void reset()
  {
    sonarFoundObject = false;
    lastSonarFoundObject = false;
    sonarDistance = SONAR_MAX_DISTANCE;

    motionFound = false;
    lastMotionFound = false;

    lastArmState = NO_ARM_ACTION;
    armState = FAST_WITHDRAW;
    armTarget = ARM_SERVO_CLOSED;

    lidState = CLOSING;
    lidTarget = LID_SERVO_CLOSED;
  }

  void openLid()
  {
    lidState = OPENING;
    lidTarget = LID_SERVO_OPEN;
    lidTimer = millis();
  }

  void closeLid()
  {
    lidState = CLOSING;
    lidTarget = LID_SERVO_CLOSED;
    lidTimer = millis();
  }

  void pushButtonFast()
  {
    armState = FAST_TRIGGER;
    armTarget = ARM_SERVO_OPEN;
    armTimer = millis();
  }

  void triggerArmSlow()
  {
    armState = SLOW_TRIGGER;
    armTarget = ARM_SERVO_OPEN;
    armTimer = millis();
  }

  void triggerArmFast()
  {
    if (armState != FAST_TRIGGER)
    {
      armState = FAST_TRIGGER;
      armTarget = ARM_SERVO_OPEN;
      armTimer = millis();
    }
  }

  void retractArmFast()
  {
    if (armState != FAST_WITHDRAW)
    {
      armState = FAST_WITHDRAW;
      armTarget = ARM_SERVO_CLOSED;
      armTimer = millis();
    }
  }

  void retractArmSlow()
  {
    if (armState != SLOW_WITHDRAW)
    {
      armState = SLOW_WITHDRAW;
      armTarget = ARM_SERVO_CLOSED;
      armTimer = millis();
    }
  }

  void stopArm()
  {
    if (isArmRetracting() || isArmTriggering())
    {
      lastArmState = armState;
      armState = NO_ARM_ACTION;
    }
  }

  bool isLidOpen()
  {
    return lidState == OPEN;
  }

  bool canOpenLid()
  {
    return !(lidState == OPENING || lidState == OPEN);
  }

  bool canCloseLid()
  {
    return armState == RETRACTED && !(lidState == CLOSING || lidState == CLOSED);
  }

  bool canPushButton()
  {
    return lidState == OPEN;
  }

  bool needsButtonPush()
  {
    return digitalRead(TOGGLE_BUTTON_PIN) == HIGH;
  }

  bool isArmRetractingFast()
  {
    return armState == FAST_WITHDRAW;
  }

  bool isArmRetractingSlow()
  {
    return armState == SLOW_WITHDRAW;
  }

  bool isArmRetracting()
  {
    return isArmRetractingFast() || isArmRetractingSlow();
  }

  bool isArmRetracted()
  {
    return armState == RETRACTED;
  }

  bool isArmFastTriggering()
  {
    return armState == FAST_TRIGGER;
  }

  bool isArmSlowTriggering()
  {
    return armState == SLOW_TRIGGER;
  }

  bool isArmTriggering()
  {
    return isArmSlowTriggering() || isArmFastTriggering();
  }

  bool isArmTriggered()
  {
    return armState == TRIGGERED;
  }

  bool isArmStopped()
  {
    return armState == NO_ARM_ACTION;
  }

  int getSonarDistance()
  {
    return sonarDistance;
  }

  bool seenMovement()
  {
    return motionFound;
  }

  bool seenMovementRising()
  {
    return motionFound && !lastMotionFound;
  }

  bool seenMovementFalling()
  {
    return !motionFound && lastMotionFound;
  }
};

BoxState state = {};

void setup()
{
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  pingTimer = millis(); // Start now.

  state.reset();
  lidServo.attach(LID_SERVO_PIN);
  lidServo.write(state.lidTarget);
  armServo.attach(ARM_SERVO_PIN);
  armServo.write(state.armTarget);

  pinMode(MOTION_PIN, INPUT);
  pinMode(TOGGLE_BUTTON_PIN, INPUT_PULLUP);
  pinMode(BASIC_STAMP_PIN, OUTPUT);
}

void loop()
{
  if (millis() >= pingTimer)
  {
    pingTimer += pingSpeed;
    sonar.ping_timer(echoCheck);
  }
  updateMotion();
  updateServos();
  updateBasicStamp();

  if (state.needsButtonPush()) // Need to push button
  {
    if (!state.isLidOpen() && state.canOpenLid())
    {
      state.openLid();
    }
    else if (state.isLidOpen())
    {
      if (state.seenMovement())
      {
        if (state.getSonarDistance() < SONAR_TRIGGER_STOP_DISTANCE)
        {
          state.stopArm();
        }
        else if (state.getSonarDistance() < SONAR_TRIGGER_RETRACT_DISTANCE)
        {
          state.retractArmSlow();
        }
        else
        {
          state.triggerArmSlow();
        }
      }
      else
      {
        state.triggerArmFast();
      }
    }
  }
  else if (!state.isArmRetracted()) // Button has been pushed and arm is still out
  {
    if (state.getSonarDistance() > SONAR_TRIGGER_RETRACT_DISTANCE)
    {
      if (state.getSonarDistance() < SONAR_TRIGGER_STOP_DISTANCE)
      {
        state.retractArmSlow();
      }
      else
      {
        state.retractArmFast();
      }
    }
    else
    {
      state.stopArm();
    }
  }
  else if (state.isArmRetracted() && !state.isLidOpen())
  {
    state.closeLid();
  }
}

void echoCheck()
{
  if (sonar.check_timer())
  {
    state.sonarDistance = sonar.ping_result / US_ROUNDTRIP_CM;
    state.lastSonarFoundObject = state.sonarFoundObject;
    if (state.sonarDistance < SONAR_MAX_DISTANCE - 100)
    {
      state.sonarFoundObject = true;
    }
    else
    {
      state.sonarFoundObject = false;
    }
    Serial.print("Ping: ");
    Serial.print(state.sonarDistance);
    Serial.println("cm");
  }
}

void updateMotion()
{
  state.lastMotionFound = state.motionFound;
  state.motionFound = digitalRead(MOTION_PIN);
}

void updateServos()
{
  unsigned lidPosition = lidServo.read();
  unsigned armPosition = armServo.read();
  unsigned long now = millis();
  int timeDiff = now - state.lidTimer;

  // Lid Servo Position Calculation
  if (lidPosition != state.lidTarget)
  {
    int sign = (lidPosition < state.lidTarget) ? 1 : -1;
    int target = lidPosition + (timeDiff * sign * LID_SERVO_SPEED / 1000); // div by 1000 because LID_SERVO_SPEED in deg per seconds
    if (sign == 1 && target > state.lidTarget)
    {
      target = state.lidTarget;
    }
    else if (sign == -1 && target < state.lidTarget)
    {
      target = state.lidTarget;
    }

    if (target != lidPosition)
    {
      state.lidTimer = now;
      lidServo.write(target);
    }
  }
  else if (state.lidState == OPENING)
  {
    state.lidState = OPEN;
  }
  else if (state.lidState == CLOSING)
  {
    state.lidState = CLOSED;
  }

  if (armPosition != state.armTarget)
  {
    int sign = (armPosition < state.armTarget) ? 1 : -1;
    int armSpeed;
    if (state.armState == FAST_TRIGGER || state.armState == FAST_WITHDRAW)
    {
      armSpeed = ARM_SERVO_SPEED_FAST;
    }
    else if (state.armState == SLOW_TRIGGER || state.armState == SLOW_WITHDRAW)
    {
      armSpeed = ARM_SERVO_SPEED_SLOW;
    }

    int target = armPosition + (timeDiff * sign * armSpeed / 1000); // div by 1000 because LID_SERVO_SPEED in deg per seconds
    if ((sign == 1 && target > state.armTarget) ||
        (sign == -1 && target < state.armTarget))
    {
      target = state.armTarget;
    }

    if (target != lidPosition)
    {
      state.armTimer = now;
      armServo.write(target);
    }
  }
  else if (state.armState == FAST_TRIGGER || state.armState == SLOW_TRIGGER)
  {
    state.armState = TRIGGERED;
  }
  else if (state.armState == FAST_WITHDRAW || state.armState == SLOW_WITHDRAW)
  {
    state.armState = RETRACTED;
  }
}

void updateBasicStamp()
{
  if (state.needsButtonPush())
  {
    digitalWrite(BASIC_STAMP_PIN, HIGH);
  }
  else
  {
    digitalWrite(BASIC_STAMP_PIN, LOW);
  }
}
