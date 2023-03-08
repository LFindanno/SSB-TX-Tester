// SSB tester single/double tone generator
// Luigi Findanno 08/03/2023
// Chips: ATTINY85, HT9200 (DTMF tone generator)


// HT9200 to uC connections
// connect pin 1 (reset) to +5V 
#define DATA 0 // pin 5 to pin 6 of HT9200
#define CK 1   // pin 6 to pin 5 of HT9200
#define SW 4   // pin 3 to 2 potitions switch, NO pin connected to GND
#define CE 3   // pin 2 to pin 1 of HT9200

// HT9200 data of corrisponding tones, see the datasheet for more details
#define SINGLE_TONE 0b00010011  // 941Hz
#define DOUBLE_TONE 0b00001011  // 941Hz + 1209Hz 

int swStatus;
byte d;

void setup() {
  pinMode(DATA, OUTPUT);
  pinMode(CK, OUTPUT);
  pinMode(CE, OUTPUT);  
  pinMode(SW, INPUT);  // switch input, put a 10k pullup.
  digitalWrite(CE, LOW);  
  swStatus = digitalRead(SW);
  if (swStatus == HIGH){
    d = SINGLE_TONE;
  }else{
    d = DOUBLE_TONE;
  }  
  delay(10);
  write5Bit(d); // first data sending is ignored, why? I don't know.
  delay(100);
  write5Bit(d); // once data are resent it works. I had this illumination when I was in the bed, before sleep.
}


void loop() {
  int swRead;
  swRead = digitalRead(SW);
  if (swRead != swStatus){ // if the switch change position
    swStatus = swRead;
    if (swStatus == HIGH){ // 941
      d = SINGLE_TONE;
    }else{                 // 941 + 1209
      d = DOUBLE_TONE;
    }
    write5Bit(d);  // send the data to HT9200
    delay(200);    // anti bounce delay
  }  
}

void write5Bit(byte b){
  for(int i=0; i<5; i++){
    digitalWrite(CK, HIGH);            // set clock high
    digitalWrite(DATA, b & (1 << i));  // set data bit
    delay(10);
    digitalWrite(CK, LOW); // data are sent on falling edge
    delay(10);
  }
}
