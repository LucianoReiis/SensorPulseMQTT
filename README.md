# Sensor de Pulso com Comunicação MQTT
Este sketch Arduino lê dados de um Sensor de Pulso (conectado ao pino analógico 0) e se comunica com um servidor usando MQTT (Message Queuing Telemetry Transport). Os dados são então publicados em um tópico específico no broker MQTT.

## Requisitos
* Arduino IDE
* Placa ESP8266 (por exemplo, NodeMCU)
* Sensor de Pulso (para monitoramento da frequência cardíaca)
* Credenciais da rede Wi-Fi
* Broker MQTT (por exemplo, test.mosquitto.org)
##   Configuração
1. Conecte o Sensor de Pulso à placa ESP8266:<br>
Fio roxo (Sensor de Pulso) → Pino analógico 0<br>
LED13 (LED embutido) → Pino 2<br>
2. Instale as bibliotecas necessárias:<br>
PubSubClient (para comunicação MQTT)<br>
3. Atualize as seguintes variáveis no sketch:<br>
ssid: SSID da sua rede Wi-Fi<br>
password: Senha da sua rede Wi-Fi<br>
mqtt_server: Endereço do broker MQTT (por exemplo, “test.mosquitto.org”)<br>
Threshold: Defina o valor de limite para detecção de pulso<br>
Faça o upload do sketch para a sua placa ESP8266.<br>
## Comunicação MQTT
* O código se conecta ao broker MQTT especificado.
* Publica a média móvel das leituras do sensor de pulso no tópico “device/pulsesensor”.
* O LED13 é controlado com base no sinal de pulso (acima ou abaixo do limite).
## Licença
Este projeto está licenciado sob a Licença MIT - consulte o arquivo LICENSE para obter mais detalhes.
