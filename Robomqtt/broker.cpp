
/*
 *   Alterna estado do LED da placa do NodeMCU ESP8266
 *   atraves de um broker MQTT
 * 
 */
 
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#define MASTER_BROKER
#include "tracao.h"
#include "broker.h"

void pub_temperaturas(long delayT);
void pub_extremes(long delayT);
String rob_topico = "";

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


void setup_broker(String idRobo) 
{
        rob_topico = idRobo;
        delay(100);
        client.setServer(mqttServer, mqttPort);
        delay(100);
        client.setCallback(minhaFuncaoSubscritora);
        Serial.print("Conectando ");
        Serial.print(mqttServer);
        Serial.print(":");
        Serial.println(mqttPort);
        conectaBroker(mqttUser, mqttPassword , 5) ;
        String subtopico = rob_topico + (String)"/#";
        client.subscribe(subtopico.c_str());  //  "#" significa subscribe todas as mensagens
        subtopico = rob_topico + (String)"/status";
        client.publish(subtopico.c_str(), "O Robô está vivo!");
}


void minhaFuncaoSubscritora(char* qtopic, byte* payload, unsigned int length) 
{
        if(fgVerbose){
              Serial.print("==> [");  // avisando recebimento de qualquer mensagem
              Serial.print(qtopic);
              Serial.print("] ");
              for (int i = 0; i < length; i++) {
                    Serial.print((char)payload[i]);
                    }
              Serial.println();
        }
        String subtopico = rob_topico + (String)"/joy/but";
        if( subtopico == (String) qtopic){   // so exemplo  LED poisser usado para fins de sinalizaação
                if ((char)payload[0] == '1') {
                        blinka(8,75);   // Liga o LED da placa (no NodeMCU nivel Low liga o led))
                }
                else {
                        digitalWrite(BUILTIN_LED, HIGH);  // Desiga o LED da placa (no NodeMCU nivel High liga o led))
                }
                if ((char)payload[1] == '1') {
                        blinka(4,150);   // Pisca 5 vezes  LED da placa (no NodeMCU nivel Low liga o led))
                }
                else {
                        digitalWrite(BUILTIN_LED, HIGH);  // Desiga o LED da placa (no NodeMCU nivel High liga o led))
                }
        } 
        subtopico = rob_topico + (String)"/joy/axis";
        if( subtopico == (String) qtopic){   //  muda o set point 21<>25
                if ((char)payload[5] != '0') {
                        if ((char)payload[4] == '-') 
                            esquerda(((payload[5]-0x30)*1023)/9);
                        else 
                            direita( ((payload[5]-0x30)*1023)/9);
                }
                else if ((char)payload[7] == '0') {
                        breake();
                }
                else if ((char)payload[6] == '-') {
                        frente(((payload[7]-0x30)*1023)/9);
                }
                else {
                        tras( ((payload[7]-0x30)*1023)/9);
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
                blinka(8,100);
                delay(3000);  // espere 3 segundos antes de retentar
            } 
        }
}


void blinka(int n, int tdelay)
{
      for(int i=0; i<n; i++){
            digitalWrite(BUILTIN_LED,LOW);
            delay(tdelay);
            digitalWrite(BUILTIN_LED,HIGH);
            delay(tdelay);
      }
}
