```python
# src/config/settings.py
THINGSBOARD_HOST = 'thingsboard.cloud'
ACCESS_TOKENS = ['3MsTGi0kqvpi8T6cXWun', 'AnotherAccessToken']  # Lista de tokens para múltiplos dispositivos
INTERVAL = 5
```

```python
# src/utils/logger.py
import logging

def setup_logger():
    logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
    return logging.getLogger(__name__)
```

```python
# src/models/sensor.py
import random

class Sensor:
    def __init__(self, name: str):
        self.name = name

    def read_temperature(self):
        return round(random.randint(10, 50), 2)
```

```python
# src/services/thingsboard_client.py
import paho.mqtt.client as mqtt
import json

class ThingsBoardClient:
    def __init__(self, host: str, access_token: str):
        self.host = host
        self.access_token = access_token
        self.client = mqtt.Client()
        self.client.username_pw_set(self.access_token)
        self.client.connect(self.host, 1883, 60)
        self.client.loop_start()
    
    def send_telemetry(self, data: dict):
        self.client.publish('v1/devices/me/telemetry', json.dumps(data), 1)
    
    def disconnect(self):
        self.client.loop_stop()
        self.client.disconnect()
```

```python
# src/controllers/main.py
import time
from models.sensor import Sensor
from services.thingsboard_client import ThingsBoardClient
from config import settings
from utils.logger import setup_logger

logger = setup_logger()

# Criar múltiplos sensores e clientes ThingsBoard
devices = []
for i, token in enumerate(settings.ACCESS_TOKENS):
    sensor = Sensor(f"Temperature Sensor {i+1}")
    client = ThingsBoardClient(settings.THINGSBOARD_HOST, token)
    devices.append((sensor, client))

try:
    while True:
        for sensor, client in devices:
            temperature = sensor.read_temperature()
            logger.info(f"{sensor.name} - Temperature: {temperature}°C")
            client.send_telemetry({'temperature': temperature})
        time.sleep(settings.INTERVAL)
except KeyboardInterrupt:
    for _, client in devices:
        client.disconnect()
    logger.info("Disconnected all devices from ThingsBoard.")
```
