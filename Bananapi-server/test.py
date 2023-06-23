import paho.mqtt.client as mqtt
import time
import random 



def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))
    test()

def on_message(mqttc, obj, msg):
    pass
def on_publish(mqttc, obj, mid):
    print("mid: "+str(mid))

def on_subscribe(mqttc, obj, mid, granted_qos):
    print("Subscribed: "+str(mid)+" "+str(granted_qos))

def on_log(mqttc, obj, level, string):
    print(string)


mqttc = mqtt.Client(transport="websockets")
mqttc.ws_set_options("/mqtt",headers=None)
mqttc.on_message = on_message
mqttc.on_connect = on_connect
mqttc.on_publish = on_publish
mqttc.on_subscribe = on_subscribe
# Uncomment to enable debug messages
mqttc.on_log = on_log
mqttc.connect("192.168.118.229", 9001, 60)
print(f'trying to connect.....')

def test():
    for i in range(0,100):
        randm= random.randint(0,100)
        data = str('{"time": "33fadfaadaffdaf3","energy":50,"voltage":220,"current":10}')
        mqttc.publish('temp',data,qos=0)
        print(i)
        #time.sleep(1)
# converted a file to csv


mqttc.loop_forever()