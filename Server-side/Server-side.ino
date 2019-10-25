/*
Developed by: Hrishikesh M. B.
For A revelotionary dustbin concept
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "<ssid>";
const char* password = "<psk>";
int dept = 50; //enter the dept of the dustbin in cm

ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

const int trigP = 2;  //D4 Or GPIO-2 of nodemcu
const int echoP = 0;  //D3 Or GPIO-0 of nodemcu

long duration =0;
int distance =0;

String page = "";
int data; 
void setup(void){
 
  pinMode(trigP, OUTPUT);
  pinMode(echoP, INPUT); 
  
  delay(1000);
  Serial.begin(115200);
  WiFi.begin(ssid, password); //begin WiFi connection
  Serial.println("");
  
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
  server.on("/", [](){
    page = " <html><meta http-equiv=\"refresh\" content=\"2\"><head><script type=\"text/javascript\" src=\"https://www.gstatic.com/charts/loader.js\"></script><script type=\"text/javascript\">google.charts.load('current', {'packages':['gauge']});google.charts.setOnLoadCallback(drawChart);function drawChart() {var data = google.visualization.arrayToDataTable([['Label', 'Value'],['Dustbin level', 10],]);var options = {width: 1200, height: 460,redFrom: 90, redTo: 100,yellowFrom:75, yellowTo: 90,minorTicks: 5};var chart = new google.visualization.Gauge(document.getElementById('chart_div'));chart.draw(data, options);setInterval(function() {data.setValue(0, 1, " +String(data)+ ");chart.draw(data, options);}, 100);}</script></head><body><div id=\"chart_div\" style=\"width: 1200px; height: 460px;\"></div></body></html> ";
    server.send(200, "text/html", page);
  });
  
  server.begin();
  Serial.println("Web server started!");
}
 
void loop(void){

  digitalWrite(trigP, LOW);   // Makes trigPin low
  delayMicroseconds(2);       // 2 micro second delay 

  digitalWrite(trigP, HIGH);  // tigPin high
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low

  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  distance= duration*0.034/2;        //Calculating actual/real distance
  data = map(distance, 0, dept, 100, 0);
  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
  Serial.println(distance);
  delay(1000);
  server.handleClient();
}
