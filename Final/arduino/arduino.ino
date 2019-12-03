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
#define LID_SERVO_CLOSED 90
#define LID_SERVO_OPEN 180
#define LID_SERVO_SPEED 270 // degrees per second

#define ARM_SERVO_PIN 10
#define ARM_SERVO_CLOSED 45
#define ARM_SERVO_OPEN 140
#define ARM_SERVO_SPEED_FAST 270 //degrees per second
#define ARM_SERVO_SPEED_SLOW 50  //degrees per second

// WARNING: Max servo speed is approx. 270 deg per second

Servo lidServo;
Servo armServo;

#define TOGGLE_BUTTON_PIN 5
#define TOGGLE_BUTTON_DEBOUNCE 500

#define BASIC_STAMP_PIN 2

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

  bool needButtonPrev;
  unsigned long buttonEdgeTime;

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
    if (lidState != OPENING)
    {
      lidState = OPENING;
      lidTarget = LID_SERVO_OPEN;
      lidTimer = millis();
    }
  }

  void closeLid()
  {
    if (lidState != CLOSING)
    {
      lidState = CLOSING;
      lidTarget = LID_SERVO_CLOSED;
      lidTimer = millis();
    }
  }

  void triggerArmSlow()
  {
    if (armState != SLOW_TRIGGER)
    {
      armState = SLOW_TRIGGER;
      armTarget = ARM_SERVO_OPEN;
      armTimer = millis();
    }
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

  bool isLidClosed()
  {
    return lidState == CLOSED;
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
    bool pinState = digitalRead(TOGGLE_BUTTON_PIN) == HIGH;
    if (pinState != needButtonPrev)
    {
      if (millis() - buttonEdgeTime > TOGGLE_BUTTON_DEBOUNCE) {
        needButtonPrev = pinState;
        buttonEdgeTime = millis();
        return !needButtonPrev;
      } else {
        buttonEdgeTime = 0;
        return pinState;
      }
    }
    else if (millis() - buttonEdgeTime > TOGGLE_BUTTON_DEBOUNCE) {
      // Still waiting for debounce to end
      return pinState;
    }
    else
    {
      // Still waiting for debounce to end
      return !pinState;
    }
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
    //sonar.ping_timer(echoCheck);
  }
  updateMotion();
  updateServos();
  updateBasicStamp();

  if (state.needsButtonPush()) // Need to push button
  {
    Serial.println("1");
    if (!state.isLidOpen() && state.canOpenLid())
    {
      state.openLid();
      Serial.println("2");
    }
    else if (state.isLidOpen())
    {
      Serial.println("3");
      if (state.seenMovement())
      {
        Serial.println("4");
        if (state.getSonarDistance() < SONAR_TRIGGER_STOP_DISTANCE)
        {
          state.stopArm();
          Serial.println("5");
        }
        else if (state.getSonarDistance() < SONAR_TRIGGER_RETRACT_DISTANCE)
        {
          state.retractArmSlow();
          Serial.println("6");
        }
        else
        {
          state.triggerArmSlow();
          Serial.println("7");
        }
      }
      else
      {
        state.triggerArmFast();
        Serial.println("8");
      }
    }
  }
  else if (!state.isArmRetracted()) // Button has been pushed and arm is still out
  {
    Serial.println("9");
    if (state.getSonarDistance() > SONAR_TRIGGER_RETRACT_DISTANCE)
    {
      Serial.println("10");
      if (state.getSonarDistance() < SONAR_TRIGGER_STOP_DISTANCE)
      {
        Serial.println("11");
        state.retractArmSlow();
      }
      else
      {
        Serial.println("12");
        state.retractArmFast();
      }
    }
    else
    {
      Serial.println("13");
      state.stopArm();
    }
  }
  else if (state.isArmRetracted() && !state.isLidClosed())
  {
    Serial.println("14");
    state.closeLid();
  }
  Serial.println("here");
  //delay(500);
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
  int lidTimeDiff = now - state.lidTimer;
  int armTimeDiff = now - state.armTimer;

  // Lid Servo Position Calculation
  if (lidPosition != state.lidTarget)
  {
    int sign = (lidPosition < state.lidTarget) ? 1 : -1;
    int target = lidPosition + (lidTimeDiff * sign * LID_SERVO_SPEED / 1000); // div by 1000 because LID_SERVO_SPEED in deg per seconds

    #ifdef LIDDEBUG
    Serial.print("Lid Sign: ");
    Serial.println(sign);
    #endif

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

    #ifdef ARMDEBUG
    Serial.print("Sign: ");
    Serial.println(sign);
    Serial.print("Arm speed: ");
    Serial.println(armSpeed);
    #endif

    int armTarget = armPosition + (armTimeDiff * sign * armSpeed / 1000); // div by 1000 because LID_SERVO_SPEED in deg per seconds
    if (sign == 1 && armTarget > state.armTarget)
    {
      armTarget = state.armTarget;
    }
    else if (sign == -1 && armTarget < state.armTarget)
    {
      armTarget = state.armTarget;
    }

    #ifdef ARMDEBUG
    Serial.print("Arm position: ");
    Serial.println(armPosition);
    Serial.print("Arm target: ");
    Serial.println(state.armTarget);
    Serial.print("Next arm target: ");
    Serial.println(armTarget);
    #endif

    if (armTarget != armPosition)
    {
      state.armTimer = now;
      armServo.write(armTarget);
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

  #ifdef ARMDEBUG
  Serial.print("Arm state: ");
  Serial.println(state.armState);
  #endif
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
