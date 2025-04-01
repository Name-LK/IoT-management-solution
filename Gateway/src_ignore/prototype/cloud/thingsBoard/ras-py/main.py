#Original Code by Thingsboard Team
#Modified by Adesola Samuel
import os
import time
import sys
import paho.mqtt.client as mqtt
import json
import random

THINGSBOARD_HOST = 'thingsboard.cloud'
ACCESS_TOKEN = '3MsTGi0kqvpi8T6cXWun'

# Data capture and upload interval in seconds. Less interval will eventually hang the DHT22.
INTERVAL=5

# Initial the dht device, with data pin connected to:

sensor_data = {'temperature': 0, 'humidity': 0}

next_reading = time.time() 

client = mqtt.Client()

# Set access token
client.username_pw_set(ACCESS_TOKEN)

# Connect to ThingsBoard using default MQTT port and 60 seconds keepalive interval
client.connect(THINGSBOARD_HOST, 1883, 60)

client.loop_start()

try:
    while True:
        temperature = random.randint(10, 50)
        temperature = round(temperature, 2)
        print(u"Temperature: {:g}\u00b0C".format(temperature))
        sensor_data['temperature'] = temperature

        # Sending humidity and temperature data to ThingsBoard
        client.publish('v1/devices/me/telemetry', json.dumps(sensor_data), 1)

        next_reading += INTERVAL
        sleep_time = next_reading-time.time()
        if sleep_time > 0:
            time.sleep(sleep_time)
except KeyboardInterrupt:
    pass

client.loop_stop()
client.disconnect()
