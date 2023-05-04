#include <Arduino.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define BLYNK_PRINT Serial

#include <BlynkCred.h>
#include <WifiCred.h>

#define LIGADA 0
#define DESLIGADA 1
#define PIN_LUZ1 22
#define V_PIN V0
#define LIM_LUZ1 60000

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = WFC_SSID;
char pass[] = WFC_PASS;

bool luz1_bt = false;

unsigned long tempo_exec;

BLYNK_WRITE(V_PIN) {
  luz1_bt = param.asInt();
}

void DesligarRele(int DEV){
  Blynk.virtualWrite(V_PIN, DESLIGADA);
  digitalWrite(DEV, LOW);
}

void LigarRele(int DEV){
  Blynk.virtualWrite(V_PIN, LIGADA);
  digitalWrite(DEV, HIGH);
}


void SyncAct(void){
  if(luz1_bt == LIGADA && Blynk.connected() == 1){
    if((tempo_exec + LIM_LUZ1) <= millis()){
      DesligarRele(PIN_LUZ1);
      Blynk.syncVirtual(V_PIN);
    }
  }else if(digitalRead(PIN_LUZ1) == 1){
    if((tempo_exec + LIM_LUZ1) <= millis()){
      digitalWrite(PIN_LUZ1, HIGH);
      luz1_bt = DESLIGADA;
    }
  }
  if((tempo_exec) < millis() && luz1_bt == DESLIGADA) tempo_exec = millis();
}

void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(PIN_LUZ1, OUTPUT);

  digitalWrite(PIN_LUZ1, LOW);
  Blynk.syncVirtual(V_PIN);
  delayMicroseconds(1500);
  tempo_exec = 0;
}

void loop() {
  Blynk.run();

  if (luz1_bt == LIGADA) {
    LigarRele(PIN_LUZ1);
  }else if(luz1_bt == DESLIGADA) {
    DesligarRele(PIN_LUZ1);
  }

  Serial.print(luz1_bt);
  Serial.print('\t');
  Serial.print(digitalRead(PIN_LUZ1));
  Serial.print('\t');
  Serial.print(Blynk.connected());
  Serial.print('\t');
  Serial.println(millis());
}


