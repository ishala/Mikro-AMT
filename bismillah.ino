#include <NewPing.h>
#include <Servo.h>
#include "CTBot.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

#define TRIG_PIN1 D11
#define ECHO_PIN1 D12
#define TRIG_PIN2 D9
#define ECHO_PIN2 D10
#define SERVO_PIN D8
NewPing sonar(TRIG_PIN, ECHO_PIN, 200);
Servo myservo;

//setup wifi dan jaringan
const int idbot = 934984961;
String token = "5577615040:AAHA5F4qMV-4dsrYcV16RnoUtfjtqWD0Qpw";
String ssid = "haha";
String pass = "faisal123";

//inisialisasi bot
CTBot mybot;
TBMessage msg;

//variabel ultrasonik 1
long jarak1 = 0;
//variable ultrasonik 2
long jarak2 = 0;
int jumlahBuka = 0;


//variabel thingspeak
WiFiClient client;
unsigned long idthingspeak = 1999667;
const char * APIKey = "LPC7EBNRTJ54ONV7";


void setup(){
  mybot.wifiConnect(ssid, pass);
  mybot.setTelegramToken(token);

  //Cek Koneksi ke bot
  if(mybot.testConnection()){
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Buruk");
  }

  WiFi.begin(ssid, pass);
  WiFi.mode(WIFI_STA);
  ThingSpeak.begin(client);
  
  mybot.sendMessage(idbot, "Wifi Connected");
  myservo.attach(SERVO_PIN);
  myservo.write(0);
  Serial.begin(9600);
}

void loop(){
  jarak1 = sonar.ping_cm();
  jarak2 = sonar.ping_cm();
  

  //Kondisi Jarak Ultrasonik Luar
  if(jarak1 <= 10){
     myservo.write(180);
     delay(3000);
     jumlahBuka++;
  } 
  else {
    myservo.write(0);
  }

  if(jarak2 < 7) {
    int x = ThingSpeak.writeField(idthingspeak, 5, jarak2, APIKey);

    mybot.sendMessage(idbot, "Tempat Sampah Penuh!");
  } 
  
  Serial.println(jarak);
  delay(1);
  


  //khususon bot
  String text = "Jumlah Buka = " + String(jumlahBuka);
  Serial.println(text);
  yield();
  if(mybot.getNewMessage(msg)){
  if(msg.text == "/laporan"){
     mybot.sendMessage(idbot, text);
    }
  }
}
