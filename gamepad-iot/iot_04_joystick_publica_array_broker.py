import sys
import paho.mqtt.client as mqtt 
import pygame

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
    print("Acompanhe no terminal:\n\n\tmosquitto_sub -h 192.168.25.233 -k keepalive -p 1883 -t '#' -v")

def inicializaJoystick():
    global joystick_count
    pygame.init()
    pygame.joystick.quit()
    pygame.joystick.init()
    joystick_count = pygame.joystick.get_count()
    print("\n\n\nHá ",joystick_count, " joysticks conectados.")
    for i in range(0, joystick_count):
        j = pygame.joystick.Joystick(i)
        j.init()
        name = j.get_name()
        print("\nJoystick ",i,": {}".format(name))
        print (j.get_numaxes()," eixos"  )
        print (j.get_numballs() , " balls" )
        print (j.get_numbuttons(), " botoes"  )
        print (j.get_numhats(), " hats"  )

def encodeAxis(axis):
    if axis <0:
        s="-"
    else:
        s="+"
    if axis > 0.9: axis = 0.9 
    if axis < -0.9: axis = -0.9 
    return s + "{}".format(abs(int(axis*10)))

def axisMotion():
    s = ""
    print("Joystick axis moved.")
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(joy.get_numaxes()):
            s = s + encodeAxis(joy.get_axis(i))
    client.publish("joy/axis",s)
    print("joy/axis",s)

def hatMotion():
    s = ""
    print("Joystick axis moved.")
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(joy.get_numhats(),):
            s = s + "{}".format(joy.get_hat(i))
    client.publish("joy/hat",s)
    print("joy/hat",s)

def ballMotion():
    print("Joystick ball moved.")
    client.publish("joy/ball","on") 

def buttonPressed():
    s = ""
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(12):
            s = s + "{}".format(joy.get_button(i))
    client.publish("joy/but",s) 
    print("buttom",s)

def buttonReleased():
    s = ""
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(12):
            s = s + "{}".format(joy.get_button(i))
    client.publish("joy/but",s) 
    print("buttom",s)


    
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
