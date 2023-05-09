import paho.mqtt.client as mqtt
import json
import base64
import pandas as pd
import csv
#import DataPlot and RealtimePlot from the file plot_data.py





def on_connect(mqttc, obj, flags, rc):
    print("rc: "+str(rc))
    initCsv()

def on_message(mqttc, obj, msg):
    writeDatatoCsv(msg.payload)
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
mqttc.subscribe('temp')

def initCsv():
    fieldnames = ['Time', 'Energy', 'AccEnergy','Current','Voltage']
    with open('./data.csv',mode='w',encoding='UTF8', newline='') as file:
        csvwritter = csv.DictWriter(file,fieldnames=fieldnames)
        csvwritter.writeheader()


def writeDatatoCsv(data):
    jsonData = json.loads(data)
    fieldnames = ['Time', 'Energy', 'AccEnergy','Current','Voltage']
    with open('./data.csv',mode='a',encoding='UTF8', newline='') as file:
        csvwritter = csv.DictWriter(file,fieldnames=fieldnames)
        csvwritter.writerow(jsonData)

# converted a file to csv


mqttc.loop_forever()