// ==================================================
// High-Level Robot Programming Example
// Behavior-Tree Style Logic on Arduino
// ==================================================

// ===== Motor Pins (L298N) =====
const int ENA = 5;
const int IN1 = 8;
const int IN2 = 9;

const int ENB = 6;
const int IN3 = 10;
const int IN4 = 11;

// ===== Ultrasonic Sensor Pins =====
const int TRIG_PIN = 2;
const int ECHO_PIN = 3;

// ==================================================
// Setup
// ==================================================
void setup() {
  initMotors();
  initUltrasonic();
}

// ==================================================
// Main Loop (High-Level Only)
// ==================================================
void loop() {
  robotBehavior();
}

// ==================================================
// Behavior Layer (Decision Logic)
// ==================================================
void robotBehavior() {
  if (obstacleDetected()) {
    avoidObstacle();
  } else {
    moveForward();
  }
}

// ==================================================
// Condition
// ==================================================
bool obstacleDetected() {
  int distance = readDistanceCM();
  return distance < 20;
}

// ==================================================
// Actions
// ==================================================
void moveForward() {
  setMotorSpeed(120, 120);
}

void avoidObstacle() {
  setMotorSpeed(-100, 100);
  delay(300);
}

// ==================================================
// Hardware Layer
// ==================================================

// ----- Motor Control -----
void initMotors() {
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
}

void setMotorSpeed(int left, int right) {
  // Left motor
  digitalWrite(IN1, left >= 0 ? HIGH : LOW);
  digitalWrite(IN2, left >= 0 ? LOW : HIGH);
  analogWrite(ENA, abs(left));

  // Right motor
  digitalWrite(IN3, right >= 0 ? HIGH : LOW);
  digitalWrite(IN4, right >= 0 ? LOW : HIGH);
  analogWrite(ENB, abs(right));
}

// ----- Ultrasonic Sensor -----
void initUltrasonic() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
}

int readDistanceCM() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  long duration = pulseIn(ECHO_PIN, HIGH, 30000);
  int distance = duration * 0.034 / 2;

  return distance;
}
