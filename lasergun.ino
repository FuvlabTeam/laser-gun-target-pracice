#define ledp 10 //LED - need pwm
#define buzzp 11 //BUZZER - need pwm
#define laserp 9
#define pb1 4 //trigger
#define pb2 2 //debug
#define pot A0

int laser, shotd=200, shotsnd;
int charge, recharge, mincharge=50, maxcharge=255, chargeout;
long shott, t;
int pb1dwn, d, dmin=20, dmax=100;
int CHEAT, deb = 150;

void setup() {    
  //Serial.begin(9600);  
  pinMode(ledp, OUTPUT);
  pinMode(buzzp, OUTPUT);
  pinMode(laserp, OUTPUT);
  pinMode(pb1, INPUT_PULLUP);
  pinMode(pb2, INPUT_PULLUP);
  pinMode(pot, INPUT);
  digitalWrite(ledp, HIGH);  
  laser = LOW;
  shotsnd = 0;
  charge = maxcharge;
  recharge = 0;
  chargeout = 0;
  pb1dwn = false;
  
  if (digitalRead(pb1)==LOW) { 
    tone(buzzp,100,2000);
    for (int i=0;i<10;i++) {
      tone(buzzp,100);
      analogWrite(ledp, 255);
      delay(200);
      noTone(buzzp);
      analogWrite(ledp, 0);
      delay(50); 
    }
    CHEAT = true;
  } 
  else {
    CHEAT = false;
    bootsequence();
  }
}

void loop() {
  //d = map(analogRead(pot),0,1023,dmin,dmax);
  
  if (CHEAT) charge = maxcharge;
  
  if (charge<maxcharge && !chargeout) {
    recharge++;
    if (recharge>200) {
      recharge=0;
      charge++;
    }
  }
  
  // led lighting
  if (charge>mincharge) analogWrite(ledp, charge);
    else analogWrite(ledp, LOW); 
  
  // shot sound
  if (shotsnd > (1000)) {
    tone(buzzp,shotsnd);
    shotsnd-=4;
  } else noTone(buzzp);

  t = millis();  
  
  // turn off laser if on for more than shotd
  if (laser && (t-shott>shotd)) {
    laser = LOW;
    digitalWrite(laserp,laser);
  }
  
  // trigger debounce
  if (digitalRead(pb1)==HIGH && t-shott > deb) pb1dwn = false;
  
  // 
  if (chargeout && t-shott>3000) {
    chargeout = 0;
    charge = mincharge;
  }
  
  //shot triggered
  if (digitalRead(pb1)==LOW && !pb1dwn) {
    pb1dwn = true;
    //debug();
    // enough juice to shoot
    if (charge>mincharge) {
      shott = millis();
      laser = HIGH;
      digitalWrite(laserp,laser);
      shotsnd = 2500;
      charge -= mincharge;
      if (charge<mincharge) chargeout = 1;
    } else { // not enough juice
      tone(buzzp,50,200);
      delay(200);
    }
    //debug();
  }
  
  if (digitalRead(pb2) == LOW) {
    debug();
    charge=maxcharge;
  }
}

void bootsequence() {
  int i;
  for (i=0;i<4;i++) {
    tone(buzzp,1500);
    analogWrite(ledp, 255);
    delay(50);
    noTone(buzzp);
    analogWrite(ledp, 0);
    delay(1000);
  }
  for (i=20;i<800;i++) {
    tone(buzzp,i);
    analogWrite(ledp, int(i/10));
    delay(5);
  }
  delay(200);
  for (i=0;i<10;i++) {
    tone(buzzp,800);
    analogWrite(ledp, 255);
    delay(50);
    noTone(buzzp);
    analogWrite(ledp, 0);
    delay(50);
  }
  tone(buzzp,800);
  digitalWrite(laserp,HIGH);
  analogWrite(ledp, 255);
  delay(1000);
  noTone(buzzp);
  digitalWrite(laserp,LOW);
}

void flashled(int n, int d) {
  for (int i=0;i<n;i++) {
    digitalWrite(ledp, LOW); 
    delay(d);
    digitalWrite(ledp, HIGH);
    delay(d);
  }
}

void debug() {
  //Serial.print(analogRead(pot)); Serial.print("  "); 
  //Serial.print(d); Serial.print("  ");
  Serial.println(charge); //Serial.print("  "); 
  //Serial.print(mode); Serial.print("  ");
  //Serial.println(state);
}
