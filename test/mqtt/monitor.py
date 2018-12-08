import sys
import configparser
import argparse
import paho.mqtt.client as mqtt

parser = argparse.ArgumentParser(description='Process some integers.')
parser.add_argument('-t', help='The topic')
parser.add_argument('-c', help='The Config')

args = parser.parse_args()

# Read INI file for all the configuration
configParser = configparser.ConfigParser()
# Change where you want the location of the config to be
configParser.read(args.c)
mqtt_broker_ip = configParser.get('mqtt_broker', 'ip')
mqtt_broker_port = configParser.getint('mqtt_broker', 'port')
mqtt_broker_username = configParser.get('mqtt_broker', 'username')
mqtt_broker_password = configParser.get('mqtt_broker', 'password')

# The callback for when the client receives a CONNACK response from the server.
def on_connect(self, client, userdata, rc):

    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.
    print "connected"

def on_message(client, userdata, message):
    print("%s %s" % (message.topic, message.payload))

client = mqtt.Client()
client.username_pw_set(mqtt_broker_username,mqtt_broker_password )
client.on_connect = on_connect
client.on_message = on_message;

client.connect(mqtt_broker_ip, mqtt_broker_port, 60)
client.subscribe(args.t)

print("Use Control-C on the keyboard to exit")

client.loop_forever()
