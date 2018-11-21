//void handleGenericArgs(){
//  String message="Number of args received:";
//  message +=server.args();
//  message +="\n";
//  for(int i=0;i<server.args();i++){
//    message +="Arg n"
//    }
//  }

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "Charlesku";
const char* password = "charles84107";

String website;

WebServer server(80);

const int pinvr = 32;

int vr=0;
void handleSensorData(){
  int sensor= analogRead(32);
 
  String json="{\"vr\": ";
  json+=sensor;
  json+="}";
  server.send(200,"application/json",json);
  }
void createSite(){
  website+="<!DOCTYPE html><html lang=\"en\"><head> <meta charset=\"UTF-8\"> <script src=\"https://code.jquery.com/jquery-2.2.4.min.js\"></script> <script>setInterval(requestData, 500); function requestData(){$.get(\"/sensors\") .done(function(data){if(data){$(\"#resis\").text(data.vr);}else{$(\"#resis\").text(\"?\");}}) .fail(function(){console.log(\"fail\");});}</script> <title>VR Reader</title></head><body> <div class=\"container\"> <p>Variable Resistor=<span id=\"resis\"></span></p></div></body></html>";
}

void handleRoot() {
//  digitalWrite(led,1);
  server.send(200, "text/html",website); 
//  digitalWrite(led,0);
}
void ledDiming(){
//  int led_Diming=server.arg("ledval").toInt();
//  ledcWrite(0,led_Diming);
  server.send(200,"text/html",website);
}
void handleNotFound() {
//  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i = 0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
//  digitalWrite(led, 0);
}

void setup(void) {
  Serial.print("12345");
 // pinMode(led, OUTPUT);
  ledcSetup(0,5000,8);
 // ledcAttachPin(led,0);
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  Serial.println("");
  createSite();
  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  if (MDNS.begin("esp32")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/ledDiming",ledDiming);
  server.on("/inline", []() {
    server.send(200, "text/html", website);
  });

  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");
}

void loop(void) {
  server.handleClient();
  vr=analogRead(pinvr);
  handleSensorData();
  //Serial.println(vr);
}

