/* 
 *  usa funçoes de traçao para ocontrolar o movimento do partir de um comandos recebido via MQTT
 *  
 */
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define MASTER_BROKER
#include "tracao.h"
#include "broker.h"

void pub_temperaturas(long delayT);
void pub_extremes(long delayT);


void setup_wifi() 
{
        WiFi.begin(ssid, password);
        Serial.println("Conectando WiFi:");
        for(int i=1; WiFi.status() != WL_CONNECTED; i++) {
                  delay(500);
                  if(i%80==0)
                          Serial.println(i);
                  Serial.print(".");
                  }
        Serial.print("\nWiFi conectado IP: ");
        Serial.println(WiFi.localIP());
}


void setup_broker() 
{
        delay(100);
        client.setServer(mqttServer, mqttPort);
        delay(100);
        client.setCallback(minhaFuncaoSubscritora);
        Serial.print("Conectando ");
        Serial.print(mqttServer);
        Serial.print(":");
        Serial.println(mqttPort);
        conectaBroker(mqttUser, mqttPassword , 5) ;
        client.subscribe("#");  //  "#" significa subscribe todas as mensagens
}


void minhaFuncaoSubscritora(char* topic, byte* payload, unsigned int length) 
{
        if(fgVerbose){
              Serial.print("Chegou mensagem [");  // avisando recebimento de qualquer mensagem
              Serial.print(topic);
              Serial.print("] ");
              for (int i = 0; i < length; i++) {
                    Serial.print((char)payload[i]);
                    }
              Serial.println();
        }
        if(strcmp(topic,"joy/but" )==0){   // so exemplo  LED poisser usado para fins de sinalizaação
                if ((char)payload[0] == '1') {
                        digitalWrite(BUILTIN_LED, LOW);   // Liga o LED da placa (no NodeMCU nivel Low liga o led))
                }
                else {
                        digitalWrite(BUILTIN_LED, HIGH);  // Desiga o LED da placa (no NodeMCU nivel High liga o led))
                }
        } 
        if(strcmp(topic,"joy/axis" )==0){   //  muda o set point 21<>25
                if ((char)payload[5] != '0') {
                        if ((char)payload[4] == '-') 
                            esquerda(((payload[5]-0x30)*1023)/9);
                        else 
                            direita( ((payload[5]-0x30)*1023)/9);
                }
                else if ((char)payload[7] == '0') {
                        brake();
                }
                else if ((char)payload[6] == '-') {
                        frente(((payload[7]-0x30)*1023)/9);
                }
                else {
                        tras( ((payload[7]-0x30)*1023)/9);
                }
        } 
        if(strcmp(topic,"pub" )==0){   // interpretando a mensagem especifica para o LED
                if ((char)payload[0] == '1') {
                        fgPub = 1;
                }
                else {
                         fgPub = 0;
                }
        } 
}



void conectaBroker(char* user, char* password, int n) 
{
     for(int i=0;  i<n && !client.connected();  i++) {   // fica em loop ate conectar
            Serial.print("... ");
            String clientId = "ETH0";
            clientId += String(random(0xffff), HEX);
            if (client.connect(clientId.c_str(), user,password)) {
                Serial.println("Conectado.");
            } else {
                Serial.print("Falhou, rc=");
                Serial.print(client.state());
                Serial.println(" retentando em 5 seconds");
                delay(5000);  // espere 5 segundos antes de retentar
            } 
        }
}
