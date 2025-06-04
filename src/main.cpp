#include <Arduino.h>
#include <WiFi.h>
#include <ArduinoJson.h>
#include <DHTesp.h>
#include <PubSubClient.h>

// Configurações de WiFi
const char *SSID = "Wokwi-GUEST";
const char *PASSWORD = "";  

// Configurações de MQTT
const char *BROKER_MQTT = "broker.hivemq.com";
const int BROKER_PORT = 1883;
const char *ID_MQTT = "esp32_mqtt";
const char *TOPIC_SUBSCRIBE_LED = "fiap/iot/led";
const char *TOPIC_PUBLISH_TEMP_HUMI = "fiap/iot/temphumi";

// Pinos de hardware
#define PIN_DHT 12
#define PIN_LED 15
#define PIN_RAIN_SENSOR 13
#define PUBLISH_DELAY 2000

// Objetos e variáveis globais
WiFiClient espClient;
PubSubClient MQTT(espClient);
DHTesp dht;

TempAndHumidity sensorValues;
unsigned long publishUpdate = 0;
const int TAMANHO = 200;

float lastTemperature = NAN;
float lastHumidity = NAN;
bool lastLedState = LOW;
int lastRainState = -1; // estado inicial indefinido

// Funções auxiliares
void updateSensorValues() {
  sensorValues = dht.getTempAndHumidity();
}

void initWiFi() {
  Serial.print("Conectando com a rede: ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println("\nConectado com sucesso!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void initMQTT() {
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback([](char *topic, byte *payload, unsigned int length) {
    String msg = String((char*)payload).substring(0, length);

    StaticJsonDocument<TAMANHO> json;
    DeserializationError error = deserializeJson(json, msg);
    if (error) {
      Serial.println("Erro ao ler JSON recebido");
      return;
    }

    if (json.containsKey("led")) {
      int valor = json["led"];
      digitalWrite(PIN_LED, valor == 1 ? HIGH : LOW);
      Serial.printf("LED %s via MQTT\n", valor == 1 ? "ligado" : "desligado");
    }
  });
}

void reconnectWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }
  Serial.println("\nWiFi reconectado com sucesso!");
}

void reconnectMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao broker MQTT...");
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("conectado!");
      MQTT.subscribe(TOPIC_SUBSCRIBE_LED);
    } else {
      Serial.print("falha, rc=");
      Serial.print(MQTT.state());
      Serial.println(" tentando novamente em 2s...");
      delay(2000);
    }
  }
}

void checkWiFiAndMQTT() {
  if (WiFi.status() != WL_CONNECTED) reconnectWiFi();
  if (!MQTT.connected()) reconnectMQTT();
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_LED, OUTPUT);
  digitalWrite(PIN_LED, LOW);

  pinMode(PIN_RAIN_SENSOR, INPUT);
  dht.setup(PIN_DHT, DHTesp::DHT22);

  initWiFi();
  initMQTT();
}

void loop() {
  checkWiFiAndMQTT();
  MQTT.loop();

  if ((millis() - publishUpdate) >= PUBLISH_DELAY) {
    publishUpdate = millis();
    updateSensorValues();

    int rainState = digitalRead(PIN_RAIN_SENSOR); // 0 = molhado, 1 = seco

    bool changed =
      sensorValues.temperature != lastTemperature ||
      sensorValues.humidity != lastHumidity ||
      digitalRead(PIN_LED) != lastLedState ||
      rainState != lastRainState;

    if (!isnan(sensorValues.temperature) && !isnan(sensorValues.humidity) && changed) {
      lastTemperature = sensorValues.temperature;
      lastHumidity = sensorValues.humidity;
      lastLedState = digitalRead(PIN_LED);
      lastRainState = rainState;

      StaticJsonDocument<TAMANHO> doc;
      doc["temperatura"] = sensorValues.temperature;
      doc["umidade"] = sensorValues.humidity;
      doc["status_led"] = lastLedState ? "on" : "off";
      doc["chuva"] = rainState == 0 ? "Chovendo" : "Seco";

      char buffer[TAMANHO];
      serializeJson(doc, buffer);
      MQTT.publish(TOPIC_PUBLISH_TEMP_HUMI, buffer);
      Serial.println(buffer);
    }
  }
}
