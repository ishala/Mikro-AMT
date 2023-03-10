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
//idBot
const int idbot = 934984961;
//botFather
String token = "5577615040:AAHA5F4qMV-4dsrYcV16RnoUtfjtqWD0Qpw";
//WIFI yang dituju
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
bool inState = true;
int detik = 0;
int kapasitas = 0;


//variabel thingspeak
WiFiClient client;
unsigned long idthingspeak = 1999667;
const char * APIKey = "LPC7EBNRTJ54ONV7";

void cekKoneksi(){
  if(mybot.testConnection()){
    Serial.println("Koneksi Bagus");
  } else {
    Serial.println("Koneksi Buruk");
  }
}

void setupData(){
  WiFi.begin(ssid, pass);
  WiFi.mode(WIFI_STA);
  //Memulai ThingSpeak dengan client
  ThingSpeak.begin(client);
}

void setupServo(){
  myservo.attach(SERVO_PIN);
  //Setting Servo dari 0 derajat
  myservo.write(0);
}

void getRange(){
  //Menyimpan nilai jarak ultrasonik luar
  rangeOut = sonarOut.ping_cm();
  //Menyimpan nilai jarak ultrasonik dalam
  rangeIn = sonarIn.ping_cm();
}


void setup(){
  //Mengkoneksikan bot dengan wifi
  mybot.wifiConnect(ssid, pass);
  //memasukkan token bot Tele
  mybot.setTelegramToken(token);

  //Memulai Serial Monitor
  Serial.begin(9600);
  
  //Cek Koneksi ke bot
  cekKoneksi();

  //Setup Wifi
  setupData();

  //Mengirim pesan ke bot telegram
  mybot.sendMessage(idbot, "Wifi Connected");
  //Setup Servo
  setupServo();
}

void loop(){
  getRange();
  kapasitas = map(rangeIn, 0, 15, 100, 0);
  
  Serial.println(rangeIn);
  Serial.println(kapasitas);
  
  //Kondisi Jarak Ultrasonik Luar
  if(rangeOut <= 10){
     myservo.write(180);
     delay(3500);
  } 
  else {
    myservo.write(0);
  }

  //Kondisi jarak ultrasonik dalam
  if(rangeIn > 0 && rangeIn <= 3) {
    if(inState){
      mybot.sendMessage(idbot, "Tempat Sampah Penuh. Segera Bersihkan Tempat Sampah!");
      inState = false;
    }
  } else if (rangeIn > 3 && rangeIn <= 5) {
     if(inState){
      mybot.sendMessage(idbot, "Tempat Sampah Hampir Penuh.");
      inState = false;
     }
  } else {
    inState = true;
  }

  
  //khususon bot dan thinkspeak
  String txtKapasitas = "Kapasitas terisi = " + String(kapasitas) + "%";
  yield();
  int x = ThingSpeak.writeField(idthingspeak, 1, kapasitas, APIKey);
  if(mybot.getNewMessage(msg)){
    if(msg.text == "/kapasitas"){
      mybot.sendMessage(idbot, txtKapasitas);
    } 
  }

  delay(1);
}
