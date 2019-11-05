/*
Developed by: Hrishikesh M. B.
For A revelotionary dustbin concept
*/
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
 
// Replace with your network credentials
const char* ssid = "Test";
const char* password = "Test";
int dept = 10; //enter the dept of the dustbin in cm
int top_thres = 9; //enter the full threshold for upper bin 


ESP8266WebServer server(80);   //instantiate server at port 80 (http port)

const int trigP = 2;  //D4 Or GPIO-2 of nodemcu
const int echoP = 0;  //D3 Or GPIO-0 of nodemcu
const int trigP1 = 14;  //D5 Or GPIO-14 of nodemcu
const int echoP1 = 12;  //D6 Or GPIO-12 of nodemcu
const int crusher = 13; //D7 or GPIO-13 of nodemcu
 
long duration =0;
int distance =0;
long duration1 =0;
int distance1 =0;

String page;
float data;
float datat; 
void setup(void){
 
  pinMode(trigP, OUTPUT);
  pinMode(echoP, INPUT); 
  pinMode(trigP1, OUTPUT);
  pinMode(echoP1, INPUT); 
  pinMode(crusher, OUTPUT); 
  
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
  digitalWrite(trigP1, LOW);
  delayMicroseconds(2);       // 2 micro second delay 
  digitalWrite(trigP, HIGH);  // tigPin high
  digitalWrite(trigP1, HIGH);
  delayMicroseconds(10);      // trigPin high for 10 micro seconds
  digitalWrite(trigP, LOW);   // trigPin low
  digitalWrite(trigP1, LOW);
  duration = pulseIn(echoP, HIGH);   //Read echo pin, time in microseconds
  duration1 = pulseIn(echoP1, HIGH); 
  distance= duration*0.0343/2;        //Calculating actual/real distance
  distance1= duration1*0.0343/2;  
  if(distance1 <= top_thres){
    digitalWrite(crusher, HIGH);
  }
  else{
    digitalWrite(crusher, LOW);
  }
  datat = map(distance, 2, dept, 30, 0);
  data = (datat/30)*100;
  Serial.print("Distance = ");        //Output distance on arduino serial monitor 
  Serial.println(distance);
  Serial.println(distance1);
  delay(1000);
  server.handleClient();
}
