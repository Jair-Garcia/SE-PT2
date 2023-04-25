#include <Wire.h>               /*~ Librería I2C ~*/
#include <LiquidCrystal_I2C.h>  /*~ Librería LCD ~*/

// Definir objeto para la pantalla LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Definir pines de los LEDs
const int led0 = 2;
const int led25 = 3;
const int led50 = 4;
const int led75 = 5;
const int led100 = 6;

// Definir pin del sensor de humedad
const int sensorHumedad = A0;

void setup() {
  // Inicializar los pines de los LEDs como salidas
  pinMode(led0, OUTPUT);
  pinMode(led25, OUTPUT);
  pinMode(led50, OUTPUT);
  pinMode(led75, OUTPUT);
  pinMode(led100, OUTPUT);
  
  // Inicializar la comunicación serial
  Serial.begin(9600);
  
  // Inicializar la pantalla LCD
  lcd.init();
  lcd.backlight();
}

void loop() {
  // Leer la humedad del suelo
  int valor = analogRead(sensorHumedad);
  
  // Convertir la lectura analógica a porcentaje de humedad
int porcentaje = map(valor, 0, 1023, 100, 0);
  
  // Mostrar el porcentaje de humedad en los LEDs utilizando la estructura switch
  switch (porcentaje / 25) {
    case 0:
      digitalWrite(led0, HIGH);
      digitalWrite(led25, LOW);
      digitalWrite(led50, LOW);
      digitalWrite(led75, LOW);
      digitalWrite(led100, LOW);
      break;
    case 1:
      digitalWrite(led0, LOW);
      digitalWrite(led25, HIGH);
      digitalWrite(led50, LOW);
      digitalWrite(led75, LOW);
      digitalWrite(led100, LOW);
      break;
    case 2:
      digitalWrite(led0, LOW);
      digitalWrite(led25, LOW);
      digitalWrite(led50, HIGH);
      digitalWrite(led75, LOW);
      digitalWrite(led100, LOW);
      break;
    case 3:
      digitalWrite(led0, LOW);
      digitalWrite(led25, LOW);
      digitalWrite(led50, LOW);
      digitalWrite(led75, HIGH);
      digitalWrite(led100, LOW);
      break;
    default:
      digitalWrite(led0, LOW);
      digitalWrite(led25, LOW);
      digitalWrite(led50, LOW);
      digitalWrite(led75, LOW);
      digitalWrite(led100, HIGH);
      break;
  }
  
  // Imprimir el valor de humedad en serie
  Serial.print("Humedad: ");
  Serial.print(porcentaje);
  Serial.println("%");
  
  // Mostrar el valor de humedad en la pantalla LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Humedad:");
  lcd.setCursor(0, 1);
  lcd.print(porcentaje);
  lcd.print("%");
  
  // Esperar 1 segundo antes de volver a leer la humedad
  delay(1000);
}
