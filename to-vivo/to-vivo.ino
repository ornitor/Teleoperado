#include <ESP8266WiFi.h>
#include <PubSubClient.h>


// minha funções de broker
void setup_wifi() ;
void setup_broker() ;
void minhaFuncaoSubscritora(char* topic, byte* payload, unsigned int length);
void conectaBroker(char* user, char* password, int n) ;


// configure a tua rede wireless
char* ssid =  "minha-rede";
char* password =   "minha-senha-de-rede"; 

//configure o teu broker mqtt
char* mqttServer = "192.168.25.233";
int mqttPort = 1883;
char* mqttUser = "" ; //Usuário
char* mqttPassword = "" ; //Senha


//Variáveis de wifi e broker mqtt
WiFiClient espClient;
PubSubClient client(espClient);

void setup() 
{
      Serial.begin(115200);
      Serial.println("\r\n\r\n***************************\n\n\Sou uma coisa  MQTT\nBom dia!\n");
      setup_wifi();
      setup_broker();
}

long iteration = 1;

void loop()   
{
        if (!client.connected()) {
             conectaBroker( mqttUser, mqttPassword , 1);
             client.subscribe("#");  //  "#" significa subscribe todas as mensagens
        }
        client.loop();     
        iteration++;
        if(iteration%200000==0)
             client.publish("status", "Vivo");

}


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
              Serial.print("Chegou mensagem [");  // avisando recebimento de qualquer mensagem
              Serial.print(topic);
              Serial.print("] ");
              for (int i = 0; i < length; i++) {
                    Serial.print((char)payload[i]);
                    }
              Serial.println();
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
