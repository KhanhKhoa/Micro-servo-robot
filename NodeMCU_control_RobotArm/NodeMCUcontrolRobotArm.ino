#include <ESP8266WiFi.h>
#include <string.h>
IPAddress ip      (192,168,1  ,150);
IPAddress gateway (192,168,1  ,1  );
IPAddress subnet  (255,255,255,0  );
IPAddress dns     (192,168,1  ,1  );
const char* ssid = "Dang Khoa";
const char* password = "9704180042754432";
char gocquay[13];
char nhan[18];
WiFiServer server(80);
void setup() {
  Serial.begin(9600);
  delay(10);
  WiFi.config(ip,dns,gateway,subnet);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }

  String request = client.readStringUntil('\r');
  //Serial.println(request);
  request.toCharArray(nhan,18);
  strncpy(gocquay, nhan + 5, 12);
  Serial.println(gocquay);
  client.flush();
}

