#include <SparkFun_MMA8452Q.h>

MMA8452Q accel;     

unsigned char screen[5];
int row[] ={6,7,8,9,10,11,12};
int column[] ={5,4,3,2,13};
long xposPrey;
long yposPrey;
long xposPred;
long yposPred;
long preyLast = 0;
long predLast = 0;
long preyDelta = 150;
long predDelta = 300;
long switchDelta = 1;
long switchLast = 0; 
long gameDelta = 15000;
long lastGame = 0;
long statsDelta = 4000;
bool prey = true;
char keyboardState;
int preyCount;
int predCount;
bool playing = true;
bool preyWon;
long onDelta = 250;
long lastOn = 0;
long lastOff = 0;
bool isOn = true;


bool isTouching(int xprey, int yprey, int xpred, int ypred) {
  float xCenter = 0.5f + xpred;
  float yCenter = 0.5f + ypred;
  float dx = xCenter - xprey;
  float dy = yCenter - yprey;
  return (((dx*dx) + (dy*dy)) <= (1.5f*1.5f + 0.5f*0.5f));
}

void randomizePos() { 
 xposPred = random(0,4);
 yposPred = random(0,6);
 
 xposPrey = random(0,5);
 yposPrey = random(0,7);
 
   
  while (isTouching(xposPrey, yposPrey, xposPred, yposPred)) {
   xposPrey = random(0,5);
   yposPrey = random(0,7);
  }
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(11, OUTPUT);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  randomSeed(analogRead(0) + analogRead(1));

  randomizePos();

   if (accel.begin() == false) {
    Serial.println("Not Connected. Please check connections and read the hookup guide.");
    while (1);
  }
}
 

void drawPrey(int x,int y) {
  digitalWrite(column[x], LOW);
  for (int i = 0; i < 5; i++) {
    if (i != x) {
      digitalWrite(column[i], HIGH);
    }
  }
  digitalWrite(row[y], HIGH);
    for (int i = 0; i < 7; i++) {
    if (i != y) {
      digitalWrite(row[i], LOW);
    }
  }
}

void drawPred(int x, int y) {
  long current = millis();
  if ((current - lastOn) > onDelta) {
    if (isOn) {
      digitalWrite(column[x], LOW);
      digitalWrite(column[x+1], LOW);
        for (int i = 0; i < 5; i++) {
           if ((i != x) && (i != x+1)) {
             digitalWrite(column[i], HIGH);
           }
        }
      digitalWrite(row[y], HIGH);
      digitalWrite(row[y+1], HIGH);
        for (int i = 0; i < 7; i++) {
           if ((i != y) && (i != y+1)) {
           digitalWrite(row[i], LOW);
        }
      }
      isOn = false;
    } 
    else {
      for (int i = 0; i < 6; i++) {
        digitalWrite(column[i], HIGH);
      }
      isOn = true;
    }
    lastOn = current;
    }
 }

void drawPoints(int winnerPoints, int loserPoints) {
  int diff = loserPoints - winnerPoints;
  int minimum = max(min(0, diff), -2) + 2;
  int maximum = min(max(0, diff), 2) + 2;
  for (int i = 0; i < 5; i++) {
    if (i < minimum || i > maximum) {
      digitalWrite(column[i], HIGH);
    }
    else {
      digitalWrite(column[i], LOW);
    }
  }
  digitalWrite(row[6], HIGH);
  for (int i = 0; i < 6; i++) {
    digitalWrite(row[i], LOW);
  }
}

void preyWins() {
  playing = false;
  preyWon = true;
  preyCount += 1;
  xposPrey = 2;
  yposPrey = 3;
}

void predWins() {
  playing = false;
  preyWon = false;
  predCount += 1;
  xposPred = 2;
  yposPred = 3;
}

void loop() {
long current = millis();
  
  if ((current - preyLast) > preyDelta) {
   if (accel.available()) {
     if (accel.getCalculatedX() > 0.40) {
      if (xposPrey != 0){
         xposPrey--;
       }
     }
     else  if (accel.getCalculatedX() < -0.40) {
       if (xposPrey != 4){
         xposPrey++;
        }
     }
  
     if (accel.getCalculatedY() > 0.40) {
      if (yposPrey != 0){
         yposPrey--;
       }
     }
     else  if (accel.getCalculatedY() < -0.40) {
       if (yposPrey != 6){
         yposPrey++;
        }
     }
   }
   preyLast = current;
  }
  
if (Serial.available() > 0) {
  keyboardState = Serial.read();
}

if ((current - predLast) > predDelta) {
    if (keyboardState == 0b00001000){
      if (yposPred > 0) {
        yposPred -= 1;
      }
    }
    else if (keyboardState == 0b00000100){
      if (yposPred < 5) {
        yposPred += 1;
      }
    }
    else if (keyboardState == 0b00000010){ 
      if (xposPred < 3) {
        xposPred += 1;
      }
    }
    else if (keyboardState == 0b00000001){ 
      if (xposPred > 0) {
        xposPred -= 1;
      }
    }
  predLast = current;
}

 if ((current - switchLast) > switchDelta) {
   prey=!prey;  
   if (!prey) {
    if (playing) {
      drawPrey(xposPrey, yposPrey);
    }
    else {
      if (preyWon) {      
        drawPoints(preyCount, predCount);     
      }
      else {
        drawPoints(predCount, preyCount);
      }
    }
   }
   else {
    if (playing) {
      drawPred(xposPred, yposPred);
    }
     else {
      if (preyWon) {
        drawPrey(2, 3);
      }
      else {
        digitalWrite(column[2], LOW);        
        digitalWrite(column[3], LOW);
        digitalWrite(row[3], HIGH);        
        digitalWrite(row[4], HIGH);        
        digitalWrite(column[0], HIGH);        
        digitalWrite(column[1], HIGH);        
        digitalWrite(column[4], HIGH);        
        digitalWrite(row[0], LOW);                
        digitalWrite(row[1], LOW);         
        digitalWrite(row[2], LOW);            
        digitalWrite(row[5], LOW);                
        digitalWrite(row[6], LOW);        
      }
    }
   }
    switchLast = current;
  }
  
  if (playing) {
    if (current - lastGame > gameDelta) {
      preyWins();
      lastGame = current;
    }
    else if (isTouching(xposPrey, yposPrey, xposPred, yposPred)) {
      predWins();
      lastGame = current;
    }
  }
  else {
    if (current - lastGame > statsDelta) {
      playing = true;
      lastGame = current;
      randomizePos();
    }
  }

  
}


 
 
