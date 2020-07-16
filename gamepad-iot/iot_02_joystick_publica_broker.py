import sys
import paho.mqtt.client as mqtt 
import pygame

#definicoes: 
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

def inicializaMQTT():
    global client
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_message = on_message
    client.username_pw_set(username, password)
    client.connect(Broker, PortaBroker, KeepAliveBroker)
    client.publish("house/bulb1","on") 
    client.loop_start()    

def inicializaJoystick():
    pygame.init()
    pygame.joystick.quit()
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
    print("\n\n\nBom dia! Há ",joystick_count, " joysticks conectados.")
    for i in range(0, joystick_count):
        j = pygame.joystick.Joystick(i)
        j.init()
        name = j.get_name()
        print("\nJoystick ",i,": {}".format(name))
        print (j.get_numaxes()," eixos"  )
        print (j.get_numballs() , " balls" )
        print (j.get_numbuttons(), " botoes"  )
        print (j.get_numhats(), " hats"  )

def axisMotion():
    print("Joystick axis moved.")
    client.publish("joy/axis","on") 

def hatMotion():
    print("Joystick hat moved.")
    client.publish("joy/hat","on")

def ballMotion():
    print("Joystick ball moved.")
    client.publish("joy/ball","on") 

def buttonPressed():
    print("Joystick button pressed.")
    client.publish("joy/but","1") 

def buttonReleased():
    print("Joystick button released.")
    client.publish("joy/but","0") 
    
    
#programa principal:
try:
        print("[STATUS] Inicializando MQTT...")
        inicializaMQTT()
        inicializaJoystick()
        while True:
            for event in pygame.event.get(): # User did something.
                if event.type == pygame.QUIT: # If user clicked close.
                    done = True # Flag that we are done so we exit this loop.
                elif event.type == pygame.JOYBUTTONDOWN:
                    buttonPressed()
                elif event.type == pygame.JOYBUTTONUP:
                    buttonReleased()
                elif event.type == pygame.JOYBALLMOTION:
                    ballMotion()
                elif event.type == pygame.JOYAXISMOTION:
                    axisMotion()
                elif event.type == pygame.JOYHATMOTION:
                     hatMotion()
except KeyboardInterrupt:
        print ("\nCtrl+C pressionado, encerrando aplicacao e saindo...")
        sys.exit(0)
