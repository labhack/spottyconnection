

# define ss 10
int SHT_clockPin = 5;  // pin used for clock
int SHT_dataPin  = 6;  // pin used for data

int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 6;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
int bpm[ss-1];
int bpmIdx = 0;
int avgBPM = 0;



int temperature[ss-1];
int humidity[ss-1];
int avgTemp = 0;
int avgHum = 0;
int idx = 0;
  
// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
//int bpm = 0;
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.


void setup(){
  Serial.begin(9600); 
   pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
   interruptSetup();  
}







float getTemperature(){
  //Return Temperature in Celsius
  SHT_sendCommand(B00000011, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);

  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  return (float)val * 0.01 - 40; //convert to celsius
}

float getHumidity(){
  //Return  Relative Humidity
  SHT_sendCommand(B00000101, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);
  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  return -4.0 + 0.0405 * val + -0.0000028 * val * val; 
}


void SHT_sendCommand(int command, int dataPin, int clockPin){
  // send a command to the SHTx sensor
  // transmission start
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, LOW);

  // shift out the command (the 3 MSB are address and must be 000, the last 5 bits are the command)
  shiftOut(dataPin, clockPin, MSBFIRST, command);

  // verify we get the right ACK
  digitalWrite(clockPin, HIGH);
  pinMode(dataPin, INPUT);

  if (digitalRead(dataPin)) Serial.println("ACK error 0");
  digitalWrite(clockPin, LOW);
  if (!digitalRead(dataPin)) Serial.println("ACK error 1");
}


void SHT_waitForResult(int dataPin){
  // wait for the SHTx answer
  pinMode(dataPin, INPUT);

  int ack; //acknowledgement

  //need to wait up to 2 seconds for the value
  for (int i = 0; i < 1000; ++i){
    delay(2);
    ack = digitalRead(dataPin);
    if (ack == LOW) break;
  }

  if (ack == HIGH) Serial.println("ACK error 2");
}

int SHT_getData(int dataPin, int clockPin){
  // get data from the SHTx sensor

  // get the MSB (most significant bits)
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  byte MSB = shiftIn(dataPin, clockPin, MSBFIRST);

  // send the required ACK
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);

  // get the LSB (less significant bits)
  pinMode(dataPin, INPUT);
  byte LSB = shiftIn(dataPin, clockPin, MSBFIRST);
  return ((MSB << 8) | LSB); //combine bits
}

void SHT_skipCrc(int dataPin, int clockPin)
{
  // skip CRC data from the SHTx sensor
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}



void loop()
{
  if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        //fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        //sendDataToProcessing('B',BPM);   // send heart rate with a 'B' prefix
        //sendDataToProcessing('Q',IBI);   // send time between beats with a 'Q' prefix
        QS = false;                      // reset the Quantified Self flag for next time 
       //if(noPulseCount < 10) 
        //bpm = BPM;
        bpm[bpmIdx] = BPM; 
        bpmIdx++;
     }
     if(bpmIdx == ss-1)
     {
       bpmIdx = 0;
       for(int k = 0; k<ss;k++)
       {
         avgBPM = avgBPM + bpm[k];
       }
       avgBPM = avgBPM/ss;
     }
     /*
     else
     noPulseCount++;
     if(noPulseCount > 10)
     noPulseCount = 0;
 */
 if(idx < ss)
 {
  temperature[idx] = getTemperature();
  humidity[idx] = getHumidity();
  idx++;
 }
 if(idx > ss-1)
 {
   for (int j=0;j<ss;j++)
   {
     avgTemp += temperature[j];
     avgHum += humidity[j];
   }
   avgTemp = avgTemp/ss;
   avgHum = avgHum/ss;
   idx = 0;
 }
if(idx==ss-1)
{
  Serial.print(avgTemp);
  Serial.print(",");
  Serial.print(avgHum);
  Serial.print(",");
  //Serial.print(bpm[bpmIdx]);
  //Serial.print(",");
  if(avgBPM<100)
  Serial.print("0");
  Serial.println(avgBPM);
  
  delay(1);
}
}

