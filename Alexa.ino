/****************************************************************************
 *Permission is hereby granted, free of charge, to any person
 *obtaining a copy of this software and associated documentation
 *files (the "Software"), to deal in the Software without
 *restriction, including without limitation the rights to use,
 *copy, modify, merge, publish, distribute, sublicense, and/or sell
 *copies of the Software, and to permit persons to whom the
 *Software is furnished to do so, subject to the following
 *conditions:
 *
 *The above copyright notice and this permission notice shall be
 *included in all copies or substantial portions of the Software.
 *
 *THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *OTHER DEALINGS IN THE SOFTWARE.
 ***************************************************************************/


#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ESP8266WiFi.h>
#include "RemoteDebug.h"
RemoteDebug Debug;
#include "fauxmoESP.h"
#include <Ticker.h>

// commentare la riga successiva se non si vuole eseguire il debug remoto
//#define DEBUG_DISABLED

#define RELAY_PIN_1 D6
#define INT_PIN     D5

// nome del dispositivo
#define LAMPADA1 "test"

//tempo in secondi per la riattivazione del tasto touch
#define SECONDI_DEBOUNCE 1


/***********************************************************************
 * nome dellhost per il remote debug e l'OTA
 **********************************************************************/
const char* host = LAMPADA1;   

/***********************************************************************
 * ssid e password della propia rete wifi
********************************************************************* */
const char* ssid ="xxxxx";
const char* password = "yyyyy";


/***********************************************************************
 * variabili usate da remote debug
***********************************************************************/
uint32_t mLastTime = 0;
uint32_t mTimeSeconds = 0;


/**********************************************************************
 * definizione di una o più lampade collegate al kit
**********************************************************************/


unsigned char dimmerValue=4;

fauxmoESP fauxmo;

bool debounce=false;

void enableButton(){
debugI("tiker expired");
  debounce=false;
}

Ticker ticker;



/***********************************************************************************
 * routine di gestione dell'interrupt
 * se l'interruzione avviene nel range di bounce l'interruzione viene ignorata
 * altrimenti richiama la routine di gestione dell'interrupt
***********************************************************************************/
  void ICACHE_RAM_ATTR Interrupt() {
    if(debounce) return;
      debounce=true;
      digitalWrite(RELAY_PIN_1, !digitalRead(RELAY_PIN_1));
      fauxmo.setState(LAMPADA1, digitalRead(RELAY_PIN_1),dimmerValue);
      debugI("RELAY 1 switched to %d",digitalRead(RELAY_PIN_1));

      ticker.once_scheduled(SECONDI_DEBOUNCE,enableButton);
  }

/***********************************************************************************
 * routine di connessione alla rete
***********************************************************************************/
void wifiSetup() {
  
  // imposta WIFI in STA mode
  WiFi.mode(WIFI_STA);

  // Connessione
  WiFi.begin(ssid, password);

  // attesa della connessione
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  // connesso!
  digitalWrite(LED_BUILTIN, LOW); // accendi led sulla scheda ad indicare l'avvenuta connessione
}

void setup() {

  Serial.begin(115200);
  delay(1000);
  Serial.println("\n\navvio programma");
  // imposta porta relé 
  pinMode(RELAY_PIN_1, OUTPUT);
  digitalWrite(RELAY_PIN_1, LOW);

  //imposta led builtin
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  //imposta pin di input del pulsante pouch
  pinMode(INT_PIN, INPUT);
  attachInterrupt(INT_PIN, Interrupt, RISING);

  wifiSetup();

/////////////////////////////////////////////////////////////////////////////////////////////////////
// inizializa il RemoteDebug
/////////////////////////////////////////////////////////////////////////////////////////////////////

  Debug.begin(host); //inizializza il server WIFI per il debug

  Debug.setResetCmdEnabled(true); // abilita il comando di reset da debug
  Debug.showProfiler(true); 
  Debug.showColors(true); // Colors

    // fine setup remote debug


/////////////////////////////////////////////////////////////////////////////////////////////////////
// OTA
/////////////////////////////////////////////////////////////////////////////////////////////////////
  ArduinoOTA.setHostname(host);
  ArduinoOTA.onStart([]() {return; });

  ArduinoOTA.onEnd([]() { return;});

  ArduinoOTA.onError([](ota_error_t error) {
    (void)error;
    ESP.restart();
  });

  /* avvia il server OTA */
  ArduinoOTA.begin();
////////////////////////////////////////////////////////////////////////////////////////////////////////
// fine avvio OTA
///////////////////////////////////////////////////////////////////////////////////////////////////////
/*
 * avvia il server fauxno per Alexa
 */
  fauxmo.createServer(true);
  fauxmo.setPort(80);
  fauxmo.enable(true);
  fauxmo.addDevice(LAMPADA1);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {

    debugI("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, LAMPADA1) == 0) ) {
      dimmerValue=value;
      // this just sets a variable that the main loop() does something about
      debugI("RELAY 1 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_1, HIGH);
      } else {
        digitalWrite(RELAY_PIN_1, LOW);
      }
      debounce=true;
      ticker.once_scheduled(SECONDI_DEBOUNCE,enableButton);
    }
  });
  fauxmo.setState(LAMPADA1, 0,dimmerValue);
}

void loop() {
  // cicla tra OTA debug e fauxmo per Alexa
  ArduinoOTA.handle();
  fauxmo.handle();
  Debug.handle();
}
