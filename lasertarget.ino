#define tgtpin A0
#define buzzpin 10
#define ledpin A2

// mapping 7-segment
// 10 9 8 7 6
//  G F + A B
//  5 4   3 2 ardu
//
//  9 8   7 6 ardu
//  E D + C DP
//  1 2 + 4 5

byte wiremap[7] = {3,2,7,8,9,4,5};
byte digits[10][7] = { { 1,1,1,1,1,1,0 }, { 0,1,1,0,0,0,0 }, { 1,1,0,1,1,0,1 }, 
{ 1,1,1,1,0,0,1 }, { 0,1,1,0,0,1,1 }, { 1,0,1,1,0,1,1 }, { 1,0,1,1,1,1,1 }, 
{ 1,1,1,0,0,0,0 }, { 1,1,1,1,1,1,1 }, { 1,1,1,0,0,1,1 }};

int pb, score, count,  incr;
int target, baseline, gameon;
long startt, t;

void setup() {  
  //Serial.begin(115200);
  for (int i=0;i<8;i++) pinMode(wiremap[i], OUTPUT);   
  pinMode(buzzpin,OUTPUT);
  pinMode(ledpin,OUTPUT);
  pinMode(tgtpin,INPUT);
  boot();
}

void loop() {
  int i;
  t = millis();
  if (t-startt>incr && gameon) {
    startt = t;
    count++;
    if (count==5) incr = 1000;
    if (count==15) {
      tone(buzzpin,150,2000);
      flashDigit(score,5,200,200,0);
      writeDigit(score);
      gameon = false; incr=10000; startt=millis();
    } else tone(buzzpin,2000,20);
  }
  
  if (gameon) {
    target = analogRead(tgtpin);
    //Serial.println(target);
    if (target < baseline*0.8) {
      score++;
      if (score==10) {
        gameon = false; incr=10000; startt=millis();
        digitalWrite(ledpin,HIGH);
        for (i=200;i<880;i++) tone(buzzpin,i);
        flashDigit(0,15,100,100, 880);
        digitalWrite(ledpin,LOW);
        flashDigit(0,10,500,500, 0);
      } else {
        digitalWrite(ledpin,HIGH);
        flashDigit(score, 6, 80, 80, 150);
        digitalWrite(ledpin,LOW);
        writeDigit(score);
      }
    }
  } else { // !gameon
    t = millis();
    if (t-startt>incr) boot();
  }
}

void boot() {
  int i;
  long tpo;
  writeDot(0);
  writeDigit(0);
  score = 0;
  tpo = 0;
  for (i=0;i<50;i++) {
    tpo += analogRead(tgtpin);
    clearDigit();
    noTone(buzzpin);
    delay(20);
    writeDigit(0);
    tone(buzzpin,1000);
    delay(20);
  }
  baseline = tpo/50;
  //Serial.println(baseline);
  delay(1000);
  noTone(buzzpin);
  gameon = true;
  count = 0;
  incr = 10000;
  startt = millis();
}

void writeDot(byte dot) {
  digitalWrite(6, !dot);
}
    
void writeDigit(int d) {
  for (int i=0; i<7; i++) {
    digitalWrite(wiremap[i], !digits[d][i]);
  }
}

void clearDigit() {
  for (int i=0; i<8; i++) {
    digitalWrite(wiremap[i], 1);
  }
}

void flashDigit(int d, int n, int on, int off, int snd) { 
  for (int i=0; i<n; i++) {
    writeDigit(d);
    if (snd) tone(buzzpin,snd);
    delay(on);
    clearDigit();
    if (snd) noTone(buzzpin);
    delay(off);
  }
}

