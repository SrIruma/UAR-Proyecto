#include <Servo.h>  // Incluye la librería Servo para controlar el servo motor

// Definición de los pines
const int trigPin = 10;   // Pin para el Trigger del sensor ultrasónico
const int echoPin = 11;   // Pin para el Echo del sensor ultrasónico
const int ledVerde = 3;   // Pin para el LED verde
const int ledRojo = 4;    // Pin para el LED rojo
const int buzzer = 6;     // Pin para el buzzer

long duration;            // Variable para almacenar la duración de la señal de eco
int distance;             // Variable para almacenar la distancia calculada
Servo myServo;            // Objeto Servo para controlar el servo motor

// Ángulos en los que sonará el buzzer para indicar mediciones (en 15, 90 y 165 grados)
const int buzzerAngles[] = {15, 90, 165}; 
const int buzzerAnglesSize = sizeof(buzzerAngles) / sizeof(buzzerAngles[0]);  // Calcula el tamaño del array

int maxDistance = 50;  // Distancia máxima configurable

// Variables para el control no bloqueante
unsigned long previousMillisServo = 0;
unsigned long previousMillisLED = 0;
unsigned long previousMillisBuzzer = 0;

const long servoInterval = 30;    // Intervalo de movimiento del servo (ms)
const long ledInterval = 5;       // Intervalo para el efecto fade del LED (ms)
const long buzzerDuration = 200;  // Duración del buzzer (ms)

int currentAngle = 15;            // Ángulo actual del servo
bool forward = true;              // Dirección del movimiento del servo
int fadeValue = 0;                // Valor de brillo para el efecto fade
bool fadeDirection = true;        // Dirección del efecto fade

bool ledRojoOn = false;           // Estado del LED rojo
unsigned long redLEDDelay = 0;    // Almacena el tiempo del último cambio del LED rojo

void setup() {
  // Configuración de los pines
  pinMode(trigPin, OUTPUT);  // Configura el pin Trigger como salida
  pinMode(echoPin, INPUT);   // Configura el pin Echo como entrada
  pinMode(ledVerde, OUTPUT); // Configura el pin del LED verde como salida
  pinMode(ledRojo, OUTPUT);  // Configura el pin del LED rojo como salida
  pinMode(buzzer, OUTPUT);   // Configura el pin del buzzer como salida

  // Inicializa la comunicación serial
  Serial.begin(9600);
  myServo.attach(12);        // Conecta el servo motor al pin 12
  myServo.write(currentAngle);  // Inicializa el servo en el ángulo 15
}

void loop() {
  unsigned long currentMillis = millis();

  // Control del servo para un movimiento suave
  if (currentMillis - previousMillisServo >= servoInterval) {
    previousMillisServo = currentMillis;
    moveServo();
  }

  // Control del efecto fade del LED verde
  if (currentMillis - previousMillisLED >= ledInterval) {
    previousMillisLED = currentMillis;
    fadeLED();
  }

  // Control del buzzer en los ángulos específicos
  checkBuzzer(currentMillis);

  // Control de los LEDs
  controlLEDs();
}

// Función para mover el servo en un barrido suave de 15 a 165 grados y viceversa
void moveServo() {
  if (forward) {
    currentAngle++;
    if (currentAngle >= 165) {
      forward = false;
    }
  } else {
    currentAngle--;
    if (currentAngle <= 15) {
      forward = true;
    }
  }
  myServo.write(currentAngle);  // Mueve el servo al ángulo actual

  // Calcula la distancia
  distance = calculateDistance();
  printData(currentAngle, distance);
}

// Función para calcular la distancia usando el sensor ultrasónico
int calculateDistance() {
  digitalWrite(trigPin, LOW);  // Asegura que el Trigger esté apagado
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH); // Activa el Trigger con un pulso de 10 microsegundos
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Mide el tiempo que tarda el eco en volver, con un límite de 30 ms
  duration = pulseIn(echoPin, HIGH, 30000);

  // Si no se detecta eco, retorna la distancia máxima
  if (duration == 0) {
    return maxDistance;
  }

  // Calcula la distancia en centímetros (duración * velocidad del sonido en el aire / 2)
  return duration * 0.034 / 2;
}

// Función para controlar los LEDs
void controlLEDs() {
  // El LED verde se enciende en los ángulos deseados (15, 90, 165) y se apaga inmediatamente después
  if (currentAngle == 15 || currentAngle == 90 || currentAngle == 165) {
    digitalWrite(ledVerde, HIGH); // Enciende el LED verde
  } else {
    digitalWrite(ledVerde, LOW);  // Apaga el LED verde
  }
  
  // El LED rojo parpadea si la distancia está entre 3 cm y 40 cm
  if (distance > 3 && distance < 40) {
    if (!ledRojoOn) {
      ledRojoOn = true; // Enciende el LED rojo
      digitalWrite(ledRojo, HIGH);
      redLEDDelay = millis(); // Reinicia el temporizador
    }
    // Controla el tiempo de parpadeo
    if (millis() - redLEDDelay >= 100) { // Cambia cada 100 ms
      redLEDDelay = millis(); // Reinicia el temporizador
      digitalWrite(ledRojo, !digitalRead(ledRojo)); // Cambia el estado del LED rojo
    }
  } else {
    // Si la distancia no está en el rango, apaga el LED rojo
    ledRojoOn = false;
    digitalWrite(ledRojo, LOW);
  }
}

// Función para controlar el efecto fade del LED verde
void fadeLED() {
  if (currentAngle % 15 == 0) {
    if (fadeDirection) {
      fadeValue += 5;
    } else {
      fadeValue -= 5;
    }

    if (fadeValue <= 0) {
      fadeValue = 0;
      fadeDirection = true;
    } else if (fadeValue >= 255) {
      fadeValue = 255;
      fadeDirection = false;
    }

    analogWrite(ledVerde, fadeValue);
  }
}

// Función para activar el buzzer en los ángulos específicos
void checkBuzzer(unsigned long currentMillis) {
  static bool buzzerOn = false;
  static unsigned long buzzerStartTime = 0;

  for (int j = 0; j < buzzerAnglesSize; j++) {
    if (currentAngle == buzzerAngles[j] && !buzzerOn) {
      tone(buzzer, 1000);  // Activa el buzzer a 1000 Hz
      buzzerOn = true;
      buzzerStartTime = currentMillis;
    }
  }

  // Apaga el buzzer después de la duración especificada
  if (buzzerOn && (currentMillis - buzzerStartTime >= buzzerDuration)) {
    noTone(buzzer);
    buzzerOn = false;
  }
}

// Función para imprimir los datos en el monitor serie
void printData(int angle, int distance) {
  Serial.print(angle);  // Imprime el ángulo
  Serial.print(",");
  Serial.print(distance);  // Imprime la distancia
  Serial.println(".");
}
