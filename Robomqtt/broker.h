
/*
 *   Alterna estado do LED da placa do NodeMCU ESP8266
 *   atraves de um broker MQTT
 * 
 */
 
 
#include <ESP8266WiFi.h>
#include <PubSubClient.h>


#ifdef MASTER_BROKER
/*
 * // configure a tua rede wireless
const char* ssid =  "minha-rede"; 
const char* password = "minha-senha-de-rede"; 

// Configure o teu broker e a instancia
char* mqttServer = "url-do-broker";
const int mqttPort = 8300; //  "configuire porta do broker";
char* mqttUser = "user-do-broker";
char* mqttPassword = "senha-do-user-do-broker";
*/

/*
char* ssid =  "GVT-068C";   //  "dualhank"; // 
char* password =  "0159269240"; // "eu sou gordinho"; // 
char* mqttServer = "m16.cloudmqtt.com";
int mqttPort = 15871;
char* mqttUser = "kubhhucj" ; //Usuário
char* mqttPassword = "qEbxoHc1HBY7" ; //Senha
*/

char* ssid =  "GVT-068C";   //  "dualhank"; // 
char* password =  "0159269240"; // "eu sou gordinho"; // 
char* mqttServer = "192.168.25.233";
int mqttPort = 1883;
char* mqttUser = "" ; //Usuário
char* mqttPassword = "" ; //Senha


//Variáveis de wifi e broker mqtt
WiFiClient espClient;
PubSubClient client(espClient);

unsigned int fgPub = 0;
unsigned int fgVerbose = 1;

#else
// configure a tua rede wireless
extern const char* ssid ; 
extern const char* password ; 

// Configure o teu broker e a instancia
extern char* mqttServer;
extern const int mqttPort ;
extern char* mqttUser;
extern char* mqttPassword ;

//Variáveis de wifi e broker mqtt
extern WiFiClient espClient;
extern PubSubClient client;

extern unsigned int fgPub;
extern unsigned int fgVerbose;
#endif


void setup_wifi() ;
void setup_broker(String idRobo) ;
void minhaFuncaoSubscritora(char* topic, byte* payload, unsigned int length);
void conectaBroker(char* user, char* password, int n) ;
void blinka(int n, int tdelay);
