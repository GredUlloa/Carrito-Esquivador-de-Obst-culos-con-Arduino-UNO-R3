#include <Servo.h>
Servo MotorSensor;

// Sensor ultrasónico
const int Trigg = 12;
const int Eco = 8;
const float v = 0.0343; // Velocidad del sonido (cm/µs)

// Motor 1
const int IN1 = 2;
const int IN2 = 7;
const int ENA = 6;
// Motor 2
const int IN3 = 4;
const int IN4 = 3;
const int ENB = 5;

// Parámetros ajustables
const int VELOCIDAD = 150;
const float DIST_OBSTACULO = 20.0;  // cm: si hay algo más cerca, decide girar
const int ANG_IZQ = 5;              // ángulo del servo mirando a la izquierda
const int ANG_DER = 175;            // ángulo del servo mirando a la derecha
const int ANG_CENTRO = 90;          // alineado al frente
const int T_GIRO_90 = 600;          // ms para girar ~90°
const int T_GIRO_180 = 1200;        // ms para dar media vuelta

void setup() {
  Serial.begin(9600);
  pinMode(Trigg, OUTPUT);
  pinMode(Eco, INPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  MotorSensor.attach(9);
  MotorSensor.write(ANG_CENTRO);
  delay(500);
}

// _____________ Para el movimiento del carro _____________
void detener() {
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void avanzar() {
  analogWrite(ENA, VELOCIDAD);
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void retroceder() {
  analogWrite(ENA, VELOCIDAD);
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void giro_izquierda() {
  analogWrite(ENA, VELOCIDAD);
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}

void giro_derecha() {
  analogWrite(ENA, VELOCIDAD);
  analogWrite(ENB, VELOCIDAD);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

// _____________ Servo del sensor _____________
void GirarIzquierdaSensor() {
  MotorSensor.write(ANG_IZQ);
  delay(500);
}

void GirarDerechaSensor() {
  MotorSensor.write(ANG_DER);
  delay(500);
}

void CentrarSensor() {
  MotorSensor.write(ANG_CENTRO);
  delay(500);
}

// _____________ Medición de distancia _____________
float medirDistancia() {
  digitalWrite(Trigg, LOW);
  delayMicroseconds(2);
  digitalWrite(Trigg, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigg, LOW);

  unsigned long tiempo = pulseIn(Eco, HIGH, 30000UL); // timeout 30 ms
  if (tiempo == 0) return 400.0;  // sin eco = camino libre
  return tiempo * v / 2.0;
}

// Promedia 3 lecturas para reducir ruido
float medirPromedio() {
  float suma = 0;
  for (int i = 0; i < 3; i++) {
    suma += medirDistancia();
    delay(30);
  }
  return suma / 3.0;
}

// ---------- Lógica principal ----------
void loop() {
  float distFrente = medirDistancia();
  Serial.print("Frente: ");
  Serial.print(distFrente);
  Serial.println(" cm");

  if (distFrente >= DIST_OBSTACULO) {
    avanzar();
    delay(50);
    return;
  }

  // Obstáculo: detenerse y explorar
  detener();
  delay(300);

  GirarIzquierdaSensor();
  float distIzq = medirPromedio();

  GirarDerechaSensor();
  float distDer = medirPromedio();

  CentrarSensor();

  Serial.print("Izquierda: ");
  Serial.print(distIzq);
  Serial.print(" cm | Derecha: ");
  Serial.print(distDer);
  Serial.println(" cm");

  if (distIzq < DIST_OBSTACULO && distDer < DIST_OBSTACULO) {
    // Si no hay salida a los costados, se retrocede y da media vuelta
    Serial.println("Sin salida: media vuelta");
    retroceder();
    delay(500);
    giro_derecha();
    delay(T_GIRO_180);
  } else if (distIzq > distDer) {
    Serial.println("Girando a la izquierda");
    giro_izquierda();
    delay(T_GIRO_90);
  } else {
    Serial.println("Girando a la derecha");
    giro_derecha();
    delay(T_GIRO_90);
  }

  detener();
  delay(200);
}