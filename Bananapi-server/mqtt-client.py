import sqlite3
import paho.mqtt.client as mqtt
import socket
import json
#connect to database
 
def get_local_ip():
    s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    try:
        # doesn't even have to be reachable
        s.connect(('192.255.255.255', 1))
        IP = s.getsockname()[0]
    except:
        IP = '127.0.0.1'
    finally:
        s.close()
    return IP
 
local_ip = get_local_ip()
print(local_ip)




def on_connect(mqttc, obj, flags, rc):
    databaseInit()
    print("rc: "+str(rc))

def on_message(mqttc, obj, msg):
    storeData(msg.payload)
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
mqttc.on_log = on_log
mqttc.connect(local_ip, 9001, 60)
print(f'trying to connect.....')
mqttc.subscribe('data')


def databaseInit():
    connection = sqlite3.connect('Energy.db')
    cursor =connection.cursor()
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS energy (
        timestamp text,
        energy real,
        accEnergy real,
        current real,
        voltage real
        )""")
    connection.commit()
    connection.close()


def storeData(data):
    connection = sqlite3.connect('Energy.db')
    cursor =connection.cursor()
    data_dict = json.loads(data)
    print(data_dict)
    cursor.execute("INSERT INTO energy VALUES (:Time,:Energy,:AccEnergy,:Current,:Voltage)",data_dict)
    cursor.execute("SELECT * FROM energy ")
    print(cursor.fetchall())
    connection.commit()
    connection.close()










mqttc.loop_forever()