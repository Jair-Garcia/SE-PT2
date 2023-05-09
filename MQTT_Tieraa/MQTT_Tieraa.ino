#include <WiFi.h>
#include <PubSubClient.h>

const char* ssid = "KratosDKill"; // Nombre de la red WiFi
const char* password = "Ruffles_Qu3so"; // Contraseña de la red WiFi

const char* mqtt_server = "test.mosquitto.org"; // Dirección IP del servidor MQTT
const int mqtt_port = 1883; // Puerto de comunicación MQTT
//const char* mqtt_user = "jair"; // Usuario MQTT (opcional)
//const char* mqtt_password = "garcia"; // Contraseña del usuario MQTT (opcional)
const char* mqtt_topic = "kratoskk"; // Tema MQTT para publicar los datos del sensor

const int sensor_pin = 34; // Pin del sensor YL-69

WiFiClient wifiClient; //se utiliza para crear un objeto de tipo WiFiClient que será utilizado para establecer una conexión WiFi
PubSubClient mqttClient(wifiClient);  crea un objeto de tipo PubSubClient que utiliza el objeto wifiClient para conectarse a un servidor MQTT

void setup() {
  Serial.begin(115200);
  pinMode(sensor_pin, INPUT);
//Establece una conexión WiFi a la red especificada mediante el uso del nombre de la red y la contraseña
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a la red WiFi...");
  }
  Serial.println("Conexión WiFi establecida");

/*Establece la conexión del cliente MQTT con el servidor MQTT especificado por mqtt_server y mqtt_port. 
También configuran la función de devolución de llamada callback y se aseguran de que el cliente MQTT 
esté conectado al servidor antes de continuar con la ejecución del programa*/
  mqttClient.setServer(mqtt_server, mqtt_port);
  mqttClient.setCallback(mqtt_callback);

  while (!mqttClient.connected()) {
    if (mqttClient.connect("ESP32Client")) {
      Serial.println("Conexión MQTT establecida");
      mqttClient.subscribe(mqtt_topic);
    } else {
      Serial.print("Fallo en la conexión MQTT, código de error: ");
      Serial.println(mqttClient.state());
      delay(2000);
    }
  }
}

void loop() {
  float lectura = analogRead(sensor_pin);
  float humedad_suelo = map(lectura,0, 4096,100, 0);
  Serial.print("Humedad del suelo: ");
  Serial.print(humedad_suelo);
  Serial.println("%");

  char payload[10];
  sprintf(payload, "%.2f", humedad_suelo);

  mqttClient.publish(mqtt_topic, payload); //Publica el valor de humedad en el tema especificado
  mqttClient.loop();//Mantiene la conexión con el servidor MQTT y procesa cualquier mensaje entrante.

  delay(1000); // Esperar un segundo antes de tomar una nueva medición
}


/*Se ejecuta cuando se recibe un mensaje en un tema MQTT suscrito por el cliente y lo muestra en consola*/
void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Mensaje recibido en el tema [");
  Serial.print(topic);
  Serial.print("]: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
}
