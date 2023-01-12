                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                              #include <NewPing.h>
#include <Servo.h>
#include "CTBot.h"
#include "ThingSpeak.h"
#include <ESP8266WiFi.h>

// ultrasonik luar
#define TRIG_PIN1 D11
#define ECHO_PIN1 D12
//ultrasonik dalam
#define TRIG_PIN2 D9
#define ECHO_PIN2 D10
//servo 
#define SERVO_PIN D8

// sonar ultrasonik luar
NewPing sonarOut(TRIG_PIN1, ECHO_PIN1, 200);
// sonar ultrasonik dalam
NewPing sonarIn(TRIG_PIN2, ECHO_PIN2, 200);
//Servo
Servo myservo;

//setup wifi dan jaringan
const int idbot = 934984961;
String token = "5577615040:AAHA5F4qMV-4dsrYcV16RnoUtfjtqWD0Qpw";
String ssid = "haha";
String pass = "faisal123";

//inisialisasi bot
CTBot mybot;
TBMessage msg;

//variabel ultrasonik luar
long rangeOut = 0;
int jumlahBuka = 0;
//variable ultrasonik dalam
unsigned long milsIn;
long rangeIn = 0;
bool inState = false;
int detik = 0;
int kapasitas = 0;


//variabel thingspeak
WiFiClient client;
unsigned long idthingspeak = 1999667;
const char * APIKey = "LPC7EBNRTJ54ONV7";


void setup(){
  //Mengkoneksikan bot dengan wifi
  mybot.wifiConnect(ssid, pass);
  //memasukkan token bot Tele
  mybot.setTelegramToken(token);

  //Memulai Serial Monitor
  Serial.begin(9600);
  
  //Cek Koneksi ke bot
  if(mybot.testConnection()){
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Buruk");
  }

  //Setup Wifi
  WiFi.begin(ssid, pass);
  WiFi.mode(WIFI_STA);
  //Memulai ThingSpeak dengan client
  ThingSpeak.begin(client);

  //Mengirim pesan ke bot telegram
  mybot.sendMessage(idbot, "Wifi Connected");
  //Setup Servo
  myservo.attach(SERVO_PIN);
  //Setting Servo dari 0 derajat
  myservo.write(0);
}

void loop(){
  //Menyimpan nilai jarak ultrasonik luar
  rangeOut = sonarOut.ping_cm();
  //Menyimpan nilai jarak ultrasonik dalam
  rangeIn = sonarIn.ping_cm();

  kapasitas = map(rangeIn, 0, 14, 0, 100);
  int x = ThingSpeak.writeField(idthingspeak, 5, kapasitas, APIKey);

  Serial.println(rangeIn);
  Serial.println(kapasitas);
  //Kondisi Jarak Ultrasonik Luar
  if(rangeOut <= 10){
     myservo.write(180);
     delay(2000);
     jumlahBuka++;
  } 
  else {
    myservo.write(0);
  }

  //Kondisi jarak ultrasonik dalam
  if(rangeIn > 0 && rangeIn <= 5) {
    inState = true;
    if(inState){
      milsIn = millis() / 1000;
      if(milsIn > 0 && milsIn <= 1) {
        mybot.sendMessage(idbot, "Tempat Sampah Penuh!");
      } else if (milsIn % 10 == 0) {
        mybot.sendMessage(idbot, "Tempat Sampah Penuh!");
      }
    }
  } else {
      inState = false;
      milsIn -= milsIn;
  }

  
 
  //khususon bot
  String txtKapasitas = "Kapasitas = " + String(kapasitas) + "%";
  String txtTerbuka = "Banyak Kali Terbuka = " + String(jumlahBuka)
  yield();
  if(mybot.getNewMessage(msg)){
    if(msg.text == "/kapasitas"){
      mybot.sendMessage(idbot, txtKapasitas);
    } else if (msg.text == "/jumlah"){
      mybot.sendMessage(idbot, txtTerbuka);
    }
  }

  delay(1);
}
