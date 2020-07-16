#include <OneWire.h>
#include <DallasTemperature.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#include "tracao.h"
#include "broker.h"

void pub_temperaturas(long delayT);
void pub_extremes(long delayT);
int  satura(float Kp, float erro, float omin, float omax);


#define ID "Aq-01"
char *id = ID;
int offsetId = 20;

// Hardware
// barramento onewire no D4 ok 
// roxo D5  amarelo D6   verde D7   azul D8
int driver[4] = {D5,D6,D7,D8};

void setup() 
{
      delay(1000);
      pinMode(BUILTIN_LED, OUTPUT);     // Inicializa pino BUILTIN_LED como output
      Serial.begin(115200);
      Serial.println("\r\n\r\n***************************\r\nRobo MQTT\r\nBom dia!\r\n");
      setup_wifi();
      setup_broker();
      setupTracao(0);
      //pwmTeste();
}

long iteration = 1;

void loop()    /// loop faz muito pouco: reconecta se necessario e chama o loop() do cliente mqtt
{
        if (!client.connected()) {
             conectaBroker( mqttUser, mqttPassword , 1);
             client.subscribe("#");  //  "#" significa subscribe todas as mensagens

        }
        client.loop();   
        iteration++;
}
