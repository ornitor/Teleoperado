import sys
import paho.mqtt.client as mqtt 
import pygame
import time

Broker = "192.168.25.233"
PortaBroker = 1883
username = ''
password = ''
KeepAliveBroker = 60
topicoRaiz = "Top100" 

fgConnect = False
 
#Callback - conexao ao broker realizada
def on_connect(client, userdata, flags, rc):
    print("[STATUS] Conectado ao Broker. Resultado de conexao: ", str(rc))
    fgConnect = True
    #faz subscribe automatico no topico
    client.subscribe(topicoRaiz + "/#")

def on_disconnect(client, userdata, flags, rc):
    print("[STATUS] Caiu")
    fgConnect = False
    #faz subscribe automatico no topico
    client.loop_stop()

#Callback - mensagem recebida do broker
def on_message(client, userdata, msg):
    MensagemRecebida = str(msg.payload)
    
    print("[MSG RECEBIDA] Topico: ",msg.topic," / Mensagem: ",MensagemRecebida)

def inicializaMQTT():
    global client
    client = mqtt.Client()
    client.on_connect = on_connect
    client.on_disconnect = on_disconnect
    client.on_message = on_message
    client.username_pw_set(username, password)
    client.connect(Broker, PortaBroker, KeepAliveBroker)
    client.publish("Saudacao","Hi") 
    client.loop_start()
    print("Acompanhe no terminal:\n\n\tmosquitto_sub -h {} -k keepalive -p 1883 -t '#' -v".format(Broker))

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


last = 0
vlast = 0

def axisMotion():
    global last, vlast
    s = ""
    #print(time.time())
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(joy.get_numaxes()):
            s = s + encodeAxis(joy.get_axis(i))
    if time.time() < 0.2 and vlast == s: return
    client.publish(topicoRaiz + "/joy/axis",s)
    last = time.time()  
    vlast
    print("joy/axis",s)

def hatMotion():
    s = ""
    print("Joystick axis moved.")
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(joy.get_numhats(),):
            s = s + "{}".format(joy.get_hat(i))
    client.publish(topicoRaiz + "/joy/hat",s)
    print("joy/hat",s)

def ballMotion():
    print("Joystick ball moved.")
    client.publish(topicoRaiz + "/joy/ball","on") 

def buttonPressed():
    s = ""
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(12):
            s = s + "{}".format(joy.get_button(i))
    client.publish(topicoRaiz + "/joy/but",s) 
    print("buttom",s)

def buttonReleased():
    s = ""
    for  j in range(joystick_count):
        joy = pygame.joystick.Joystick(j)
        joy.init()
        for i in range(12):
            s = s + "{}".format(joy.get_button(i))
    client.publish(topicoRaiz + "/joy/but",s) 
    print("buttom",s)



# pygame.display.set_caption('Matemática está em tudo')
clock = pygame.time.Clock()   # relogio pra temporizar a animaçao 
#programa principal:
try:
        print("[STATUS] Inicializando MQTT...")
        inicializaMQTT()
        inicializaJoystick()
        while True:
            if not fgConnect: 
                client.reconnect()
                fgConnect = True
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
