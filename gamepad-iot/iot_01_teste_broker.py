import sys
import paho.mqtt.client as mqtt 

#dados do meu broker
Broker = "192.168.25.233"
PortaBroker = 1883
username = ''
password = ''
KeepAliveBroker = 60
TopicoSubscribe = "mov/#"


#Callback - conexao ao broker realizada
def on_connect(client, userdata, flags, rc):
    print("[STATUS] Conectado ao Broker. Resultado de conexao: ", str(rc))
 
    #faz subscribe automatico no topico
    client.subscribe(TopicoSubscribe)
 
#Callback - mensagem recebida do broker
def on_message(client, userdata, msg):
    MensagemRecebida = str(msg.payload)
    
    print("[MSG RECEBIDA] Topico: ",msg.topic," / Mensagem: ",MensagemRecebida)
    
 
#programa principal:
try:
        print("[STATUS] Inicializando MQTT...")
        #inicializa MQTT:
        client = mqtt.Client()
        client.on_connect = on_connect
        client.on_message = on_message
        client.username_pw_set(username, password)
        client.connect(Broker, PortaBroker, KeepAliveBroker)
        client.publish("house/bulb1","on") 
        client.loop_forever()
except KeyboardInterrupt:
        print ("\nCtrl+C pressionado, encerrando aplicacao e saindo...")
        sys.exit(0)
