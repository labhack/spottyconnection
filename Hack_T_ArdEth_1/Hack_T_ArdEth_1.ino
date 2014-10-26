#include <Ethernet.h>
#include <SPI.h>
#include "plotly_streaming_ethernet.h"

int ecount = 0;
#define nTraces 3
char *tokens[nTraces] = {"ptfmmlg58r", "ybmp9ot8wa" , "urn3603tit"};
// arguments: username, api key, streaming token, filename
plotly graph = plotly("irahulone", "87r3qvse0y", tokens, "temp", nTraces);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
byte my_ip[] = { 199, 168, 222, 18 }; // google will tell you: "public ip address"

void startEthernet(){
 
    Serial.println("... Initializing ethernet");
    if(Ethernet.begin(mac) == 0){
        Serial.println("... Failed to configure Ethernet using DHCP");
        // no point in carrying on, so do nothing forevermore:
        // try to congifure using IP address instead of DHCP:
        Ethernet.begin(mac, my_ip);
    }
    Serial.println("... Done initializing ethernet");
    delay(1000);
}


void setup() 
{
   pinMode(9, OUTPUT);
digitalWrite(9,1);
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  startEthernet();

  graph.fileopt="overwrite"; // See the "Usage" section in https://github.com/plotly/arduino-api for details
  bool success;
  graph.log_level = 0;
  success = graph.init();
  if(!success){while(true){}}
  graph.openStream();
}
int temp = 0;
int hum = 0;
int bpm = 0;
unsigned long x;
int y;

void loop() {
  
  //if(Serial.available() > 0)
  while (Serial.available() >= 9)
  { 
     temp = Serial.parseInt(); 
     hum = Serial.parseInt(); 
     bpm = Serial.parseInt(); 
  delay(5);
    graph.plot(millis(), temp, tokens[0]);
    graph.plot(millis(), hum, tokens[1]);
    graph.plot(millis(), bpm, tokens[2]);

  delay(50);
  }
  
  if(bpm < 60 || bpm > 220 || temp >35 || temp < 10 || hum >60 || hum < 5)
  {
    if(bpm != 0 && temp !=0 && hum !=0 )
    ecount ++;
    delay(100);
  }
  /*
  for(int l=0;l<ecount;l++)
  {
    digitalWrite(9,1);
    delay(10);
    digitalWrite(9,0);
    delay(90);
  }
  */
  if (ecount > 5)
  {
    ecount = 0;
    digitalWrite(9,0);
    delay(1000);
    digitalWrite(9,1);
  }   
    digitalWrite(9,1);

}
