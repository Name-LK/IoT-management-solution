#include <Arduino.h>

#include <AwsIotWiFiClient.h>
#include <ESP8266WiFi.h>

#include <pgmspace.h>

const char endpoint[] = "endpoint.iot.us-east-1.amazonaws.com";
const char clientId[] = "client";
const char publishTopicName[] = "topic";
const char subscribeTopicFilter[] = "topic";

// Device certificate
// Copy and paste contents from *-certificate.pem.crt
static const char deviceCertificate[] PROGMEM = R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWTCCAkGgAwIBAgIUJCix3+MECbGe9r4+XwtyXCqaopUwDQYJKoZIhvcNAQEL
BQAwTTFLMEkGA1UECwxCQW1hem9uIFdlYiBTZXJ2aWNlcyBPPUFtYXpvbi5jb20g
SW5jLiBMPVNlYXR0bGUgU1Q9V2FzaGluZ3RvbiBDPVVTMB4XDTI0MTAxNzE0MDcw
NloXDTQ5MTIzMTIzNTk1OVowHjEcMBoGA1UEAwwTQVdTIElvVCBDZXJ0aWZpY2F0
ZTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBAK08bDD32hKpf231lCbT
oYFNBEZjAznmxKisBK6hQCdxiqCOlk9oJQEXVc2RUHNnGyJ4zry/iTvfDGVJWlYr
KxhgMbKr91kbTleryGfieGG98AB11b6Ald2P4BjWwvlxtFRfFARvzppCz0fYNVUJ
o0JkUoE78eo56jxPReqfxOoeQvkO2RJBm23DPzjc4+eaJKnSlvYCVhIsbvvVVmNc
JYeu0lpDKt2yP+fnMZpOKb42zKlE/D+IKD9abMYN1CsdnKocyuTDQhGPuNkJT28+
9woaxKvTafbjgSstPG7NOt2zy1vqyzja/Np8JvWAZ7KHyAxY6W4n5sYDZH049W74
GmECAwEAAaNgMF4wHwYDVR0jBBgwFoAUfwMzo+FPS9WOuvLhAWUEgDNk8+EwHQYD
VR0OBBYEFGBwYNFwWxVHAbNHRNg00+jLGIg8MAwGA1UdEwEB/wQCMAAwDgYDVR0P
AQH/BAQDAgeAMA0GCSqGSIb3DQEBCwUAA4IBAQCZUAQypMSfYAMLZJOaUVOFDeOG
k7CFITUsrpfxGvC88dC4zbLi6lN50I0fGSOBnL6Rwg4k9My48CXzozOZ8PPWvwTC
ZoaYHzhO6kSftXdb7GdoiS3b0gJQkc6Emc8inBAcr0xaDJ0CMiPjlbWdQ6URo2Y9
aWd7Bxv/ieHGeN6kY8FbWyUAZXL7wEHLaH20Pj5yhdxHfu77Fyn9O0mL3RsbkmMS
dw0+HIkvC+h+EwMTZxVCHZgcV+VMMBiUuyQe5UDZGbNmZRk9KDjpt6Rmz60tXZUH
aiVjO1KE4z3ahxUiXDLJD9eBvAmsqpdnK80UokfAdrbHqy+sYv4O5ulyQbIT
-----END CERTIFICATE-----
)KEY";

// Key files / Private key file
// Copy and paste contents from *-private.pem.key
static const char privateKeyFile[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEogIBAAKCAQEArTxsMPfaEql/bfWUJtOhgU0ERmMDOebEqKwErqFAJ3GKoI6W
T2glARdVzZFQc2cbInjOvL+JO98MZUlaVisrGGAxsqv3WRtOV6vIZ+J4Yb3wAHXV
voCV3Y/gGNbC+XG0VF8UBG/OmkLPR9g1VQmjQmRSgTvx6jnqPE9F6p/E6h5C+Q7Z
EkGbbcM/ONzj55okqdKW9gJWEixu+9VWY1wlh67SWkMq3bI/5+cxmk4pvjbMqUT8
P4goP1psxg3UKx2cqhzK5MNCEY+42QlPbz73ChrEq9Np9uOBKy08bs063bPLW+rL
ONr82nwm9YBnsofIDFjpbifmxgNkfTj1bvgaYQIDAQABAoIBACVIwNxVK7kig8uN
tEIbAEKM6eOgQbx/f0nyI9vV1rHmjPh7V7xlZLl0ZUImCVOdGEj2CUC0VHvvSIKw
I+IxzkejxG549nZWGylhTnOVWWVn9LSUp9klYZva8FdIOZRoNXCHOg6uZZ/ryyLi
VLAlr96FKwCnfyxzJIewZBLYKhBc8lD9zAg882m5DgZZa5DQV7JCpRZGxsU8jC0W
kjkVGDtSyKec2l8CrtrWCyEjFufBzySEeF/7AceWIPoRUP3VPE+Oh27MtslokbOQ
D1V29nAXFG0r+Hkt+4kdzFKeQRcvbEXBjYkbrjEo9Mmi2EBvro85OAffmdCS10cI
IoN1prECgYEA1oqRWI+8dyZPqGl+vyOrL3AhkPZKWMD8JWtWT41CqvcpgAZR31iF
kopIuOL1IuVKU1CdXlSGYG2KCNtkmRgGUm+qWcMuUMyTCoES5wSttHTobaEGHT11
spviWVL6w4t/SVqufU0/5cEReLsNCcxFMZ3K3SORkO594ZOZ5q7pAB0CgYEAzrZ4
Lwkiy5/QOqzkwrw0D/LiPSOHw+Q/vkqEFbUGc8mCniGtV91KMN5D4zb4wrLL7wES
ZJND0ChaWnBhxlVscfrMoJ9pByfZuRFPfOpxrnstyxFmtN0s5BVW4EUQ0LMrR49U
9uGMCezykUmA2p5MVYMx6oNm89ARe9YoUCaL+BUCgYB1dNeuTMM52Fe0NE2YB8co
6ZL28mAxxzZEpkdStlvRjg/Cb7NTO/ulBuBZmKfsuuu+QVxm0I/hKxdvUe35gxWy
/D+fdSReSCIYliHrKLyRF5BItOfMCmHtrzp0/OEdPboqRuNQ5oIdH16ZYo741kTr
rR/rghWn62ovncecbBZUYQKBgEVHCghZe3va16KShyhsgtZFhGURH1Mbj/Fqb9IG
lV8AzfHJP5sO4up10zK3Ha/C1G4JYeyrHjMJEx70QxCBWJLqhuHeCwkG5gUMNCOd
PvRG/BXugwlihsT6Ed+fxf7rzTlyY/tJo/E/pBvlEnYyT0rpaTU7TKmxtepW37Pc
91PxAoGAAcNPf02GNNa062AGeI2xIVreEy2h5/HZEUyAWbsh0yd5pdoWX+lYtl98
XBlGpgv4/XyX1Opgj9kgi7EsroCm0SCFNnt1e9O83JtnUcWinKhJyu99wdvuJqvC
G+8RBBVEmRUL5ZQWPPdHXI13K77D7j7stRvlU2EueCffK/2VQR8=
-----END RSA PRIVATE KEY-----
)KEY";

// RSA 2048 bit key: Amazon Root CA 1
// Copy and paste contents from AmazonRootCA1.cer
static const char rootCaCertificate[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";

const char ssid[] = "CLARO_009705";
const char password[] = "48UcnNRaxc";

AwsIotWiFiClient awsIotWiFiClient;

BearSSL::X509List trustAnchorCertificate(rootCaCertificate);
BearSSL::X509List clientCertificate(deviceCertificate);
BearSSL::PrivateKey clientPrivateKey(privateKeyFile);

void connectWiFi() {
  Serial.print("Connecting to Wi-Fi \"");
  Serial.print(ssid);
  Serial.print("\"...");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print('.');
  }

  Serial.println();
  Serial.print("Local IP address: ");
  Serial.println(WiFi.localIP());
  Serial.println("Wi-Fi connection was successful!");
}

void receiveMessage(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message received: ");
  Serial.write(payload, length);
  Serial.println();
}

void setupAwsIotWiFiClient() {
  Serial.println("Setting up AWS IoT Wi-Fi Client...");

  awsIotWiFiClient.setDebugOutput(true)
    .setCertificates(&trustAnchorCertificate, &clientCertificate, &clientPrivateKey)
    .setEndpoint(endpoint)
    .setReceiveMessageCallback(receiveMessage)
    .setClientId(clientId)
    .setSubscribeTopicFilter(subscribeTopicFilter)
    .connect();

  Serial.println("AWS IoT Wi-Fi Client setup was successful!");
}

void setup() {
  Serial.begin(9600);
  Serial.println("Setup...");

  connectWiFi();
  setupAwsIotWiFiClient();

  Serial.println("Setup was successful!");

  awsIotWiFiClient.publishMessage(publishTopicName, "Hello, world!");
}

void loop() {
  awsIotWiFiClient.loop();
}
