#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Defina suas credenciais Wi-Fi
const char* ssid = "Brito-2.4";
const char* password = "famili@1949";

// Defina o endereço do servidor MQTT (por exemplo, test.mosquitto.org)
const char* mqtt_server = "test.mosquitto.org";

WiFiClient espClient;
PubSubClient client(espClient);

// Variáveis
int PulseSensorPurplePin = 0; // Pulse Sensor PURPLE WIRE conectado ao pino ANALÓGICO 0
int LED13 = 2; // O LED embutido no Arduino (pino 2)

int Signal; // Armazena os dados brutos recebidos. O valor do sinal pode variar de 0 a 1024
int Threshold = 550; // Determina qual sinal considerar como "batida" e qual ignorar

// Média móvel
const int numReadings = 10;
int readings[numReadings]; // Armazena as últimas leituras
int readIndex = 0; // Índice atual de leitura

// Configuração inicial
void setup() {
  pinMode(LED13, OUTPUT); // Define o pino do LED como saída
  Serial.begin(9600); // Inicializa a comunicação serial
  setup_wifi(); // Configura a conexão Wi-Fi
  client.setServer(mqtt_server, 1883); // Configura o servidor MQTT

  // Inicializa as leituras com zeros
  for (int i = 0; i < numReadings; i++) {
    readings[i] = 0;
  }
}

// Função para reconectar ao servidor MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT...");
    String clientId = "ESP8266Client-" + String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) {
      Serial.println("conectado");
    } else {
      Serial.print("falha, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 5 segundos");
      delay(5000);
    }
  }
}

// Função para configurar a conexão Wi-Fi
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Wi-Fi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

// Função para calcular a média móvel
int calculateMovingAverage() {
  int total = 0;
  for (int i = 0; i < numReadings; i++) {
    total += readings[i];
  }
  return total / numReadings;
}

// Loop principal
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  Signal = analogRead(PulseSensorPurplePin);
  Serial.println(Signal);

  // Armazena a leitura atual
  readings[readIndex] = Signal;
  readIndex = (readIndex + 1) % numReadings;

  // Calcula a média móvel
  int movingAverage = calculateMovingAverage();

  // Publica o valor da média móvel no tópico MQTT
  char msg[10];
  snprintf(msg, sizeof(msg), "%d", movingAverage);
  client.publish("device/pulsesensor", msg);

  // Verifica se o sinal está acima do limite
  if (movingAverage > Threshold) {
    digitalWrite(LED13, LOW);
  } else {
    digitalWrite(LED13, HIGH);
  }

  delay(10);
}
