#include <OneWire.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D4


const char* host = "api.thingspeak.com"; // Your domain  
String ApiKey = "9K109EPJFMOB69YF";
String path = "/update?api_key=" + ApiKey + "&field1=";  
const char* server = "54.175.21.22";
String serverPath = "/data?temperature=";

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

const char* ssid = "U+NetFC73";
const char* password = "4000011857";


char temperatureString[6];
void connect_ap() {
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP());  
}
void setup(void){
  Serial.begin(115200);
  Serial.println("");
  
  connect_ap();
  Serial.println("Dallas Temperature IC Control Library Demo");

  DS18B20.begin();
}


float getTemperature() {
  float temp;
  do {
    DS18B20.requestTemperatures(); 
    temp = DS18B20.getTempCByIndex(0);
    delay(100);
  } while (temp == 85.0 || temp == (-127.0));
  return temp;
}


void loop() {
  float temperature = getTemperature();

  dtostrf(temperature, 2, 2, temperatureString);
  // send temperature to the serial console
  Serial.println(temperatureString);
  WiFiClient client;
  WiFiClient client1;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  const int serverPort=3000;
  
  if (!client1.connect(server, serverPort)) {
    Serial.println("connection failed");
    return;
  }
  client1.print(String("GET ") + serverPath + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + server + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
  client.print(String("GET ") + path + temperatureString + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: keep-alive\r\n\r\n");
               
  delay(60000);

}
