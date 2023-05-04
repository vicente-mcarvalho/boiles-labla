#include <Arduino.h>
#include <BlynkSimpleEsp32.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define BLYNK_PRINT Serial

// COPIAR DIRETAMENTE DO BLYNK.IO
#define BLYNK_TEMPLATE_ID "TMPLE2olxXrP"
#define BLYNK_TEMPLATE_NAME "Horta"
#define BLYNK_AUTH_TOKEN "92JKMuQ_mHNI-ynRtGK-ZdS0PDN8ijpl"

//CREDENCIAIS DE ACESSO À WIFI
#define WFC_SSID "BOILES"
#define WFC_PASS "e1cc72048901@"

#define LIGADA 1
#define DESLIGADA 0
#define PIN_BOMBA 22
#define V_PIN V1
#define LIM_BOMBA 60000

char auth[] = BLYNK_AUTH_TOKEN;

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = WFC_SSID;
char pass[] = WFC_PASS;

bool bomba_bt = false;
int  bomba_tempo; 

unsigned long tempo_exec;

BLYNK_WRITE(V_PIN) {
  bomba_bt = param.asInt();
}

void DesligarBomba(void){
  digitalWrite(PIN_BOMBA, LOW);
  Blynk.virtualWrite(V_PIN, DESLIGADA);
}

void LigarBomba(void){
  digitalWrite(PIN_BOMBA, HIGH);
  Blynk.virtualWrite(V_PIN, LIGADA);
}


void SyncAct(void){
  if(bomba_bt == LIGADA && Blynk.connected() == 1){
    if((tempo_exec + LIM_BOMBA) <= millis()){
      DesligarBomba();
      Blynk.syncVirtual(V_PIN);
    }
  }else if(digitalRead(PIN_BOMBA) == 1){
    if((tempo_exec + LIM_BOMBA) <= millis()){
      digitalWrite(PIN_BOMBA, HIGH);
      bomba_bt = DESLIGADA;
    }
  }
  if((tempo_exec) < millis() && bomba_bt == DESLIGADA) tempo_exec = millis();
}

void setup() {
  // Debug console
  Serial.begin(9600);

  Blynk.begin(auth, ssid, pass);
  pinMode(PIN_BOMBA, OUTPUT);

  digitalWrite(PIN_BOMBA, LOW);
  Blynk.syncVirtual(V_PIN);
  delayMicroseconds(1500);
  tempo_exec = 0;
}

void loop() {
  Blynk.run();

  if (bomba_bt == LIGADA) {
    LigarBomba();
  }else if(bomba_bt == DESLIGADA) {
    DesligarBomba();
  }
  SyncAct();
  Serial.print(bomba_bt);
  Serial.print('\t');
  Serial.print(digitalRead(PIN_BOMBA));
  Serial.print('\t');
  Serial.print(Blynk.connected());
  Serial.print('\t');
  Serial.print(millis());
  Serial.print('\t');
  Serial.print(tempo_exec + LIM_BOMBA);
  Serial.print('\t');
  Serial.println(tempo_exec + LIM_BOMBA - millis());
}


