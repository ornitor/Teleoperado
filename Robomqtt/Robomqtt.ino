#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "tracao.h"
#include "broker.h"

void pub_temperaturas(long delayT);
void pub_extremes(long delayT);
int  satura(float Kp, float erro, float omin, float omax);


String idRobo = "Top100";
String stsTopico = idRobo + (String)"/status";
int offsetId = 20;


void setup() 
{
      delay(1000);
      pinMode(BUILTIN_LED, OUTPUT);     // Inicializa pino BUILTIN_LED como output
      Serial.begin(115200);
      Serial.println("\r\n\r\n***************************\r\nRobô IoT\r\nBom dia!\r\n");
      setup_wifi();
      setup_broker(idRobo);
      setupTracao(0);
}

long iteration = 1;

void loop()    /// loop faz muito pouco: reconecta se necessario e chama o loop() do cliente mqtt
{
        if (!client.connected()) {
             breake();
             conectaBroker( mqttUser, mqttPassword , 1);
             client.subscribe("#");  //  "#" significa subscribe todas as mensagens

        }
        client.loop();   
        iteration++;
        if(iteration%500000==0)
             client.publish(stsTopico.c_str(), "Vivo");
}
