#include <Arduino.h>
#include <WiFi.h>
#include <WiFiMulti.h>

#include "settings.h"
#include "led.h"

// Objeto utilizado para conexão com a rede
WiFiMulti wiFiMulti;

// Objeto utilizado para conexão com o servidor (TCP)
WiFiClient client;

void setupWiFi()
{
  // Modo Access Point
  wiFiMulti.addAP(SSID, PASSWORD);

  Serial.println();
  Serial.print("Waiting for WiFi... ");

  while (wiFiMulti.run() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

// Função que conecta com o Servidor Go
bool connect()
{
  Serial.print("Connecting to ");
  Serial.println(SERVER_IP);

  return client.connect(SERVER_IP, SERVER_PORT);
}

void verifyConnection()
{
  if (!client.connected())
  {
    client.stop();
    Serial.println("Desconectado, reconectando...");
    setColor(0, 255, 0);
    if (connect())
    {
      Serial.println("Conectado com o servidor Go");
      setColor(0, 255, 0);
    }
    else
    {
      Serial.println("Erro ao conectar com o servidor Go");
      setColor(255, 0, 0);
    }
  }
}

void setup()
{
  Serial.begin(9600);

  setupWiFi();

  if (connect())
  {
    Serial.println("Conectado com o servidor Go");

    // pinMode(SENSOR_HUMIDITY_PIN, INPUT);
    setColor(0, 0, 255);
  }
  else
  {
    Serial.println("Erro ao conectar com o servidor Go");
    setColor(255, 0, 0);
  }

  client.print("GET /index.html HTTP/1.1\n\n");

  for (int maxloops = 0; !client.available() && maxloops < 1000; maxloops++)
    delay(1);

  if (client.available() > 0)
  {
    setColor(0, 255, 0);
  }

  delay(500);
}

void loop()
{

  int sensorValue = analogRead(SENSOR_HUMIDITY_PIN);
  int humidity = (100.00 - ((sensorValue / 4096.00) * 100.00));

  client.print("umd:" + String(humidity) + "\n\n");

  verifyConnection();

  delay(200);
}