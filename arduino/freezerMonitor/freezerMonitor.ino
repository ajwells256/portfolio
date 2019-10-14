#include "temp.h"
#include "heap.h"
#include "utils.h"

// begin my code:
#define RESET_SECONDS 43200
#define NUM_CATS 7
#define LED LED_BUILTIN
#define TEMP_DEFAULT -20
#define RED 2
#define GREEN 3
#define BLUE 4
#define OFF 0

// timing
int milli_sec = 0; // milliseconds
bool half_sec, second = false;
unsigned long seconds = 0;

// serial
int buff[16];
int rPtr, wPtr; 
bool live = false;

int cats[7];
int patterns[][2] = {{OFF, OFF}, {BLUE, OFF}, {BLUE, GREEN}, {GREEN, OFF}, {GREEN, RED}, {RED, OFF}, {RED, RED}};
int worst = TEMP_DEFAULT;
int currentPattern = 0;
int proposal[2]; // times observed, value

ISR(TIMER2_COMPA_vect) {
  // fires every 1 ms
  milli_sec++;
  if(milli_sec == 500) {
    half_sec = true;
  } else if(milli_sec == 1000) {
    milli_sec = 0;
    second = true;
  }
}

void readSerial();

void setup() {
  // interrupts
  cli(); // interrupts off
  OCR2A = 249; // compare to value
  TIMSK2 |= (1<<OCIE2A); // enable comparison mode
  TCCR2B |= (1<<CS22); // CS22 is the 3rd option (zero indexed) for the 2nd timer -- 64 prescaler
  sei();

  // Serial
  Serial.begin(9600);

  // heap
  capacity = -1;

  // initialize digital pins as outputs
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(RED, OUTPUT);
  pinMode(GREEN, OUTPUT);
  pinMode(BLUE, OUTPUT);
  
  int i;
  for (i = 0; i < NUM_CATS; i++) {
    cats[i] = (i - 2) * 5;
  }
  resetProposal();
}

inline void resetProposal() {
  proposal[0] = 0;
  proposal[1] = TEMP_DEFAULT;
}

inline void clearRGB() {
  digitalWrite(RED, LOW);
  digitalWrite(GREEN, LOW);
  digitalWrite(BLUE, LOW);
}

inline void blinkLow(int pattern) {
  digitalWrite(LED, LOW);
  clearRGB();

  int rgbOn = patterns[pattern][1];
  if (rgbOn != OFF) {
    digitalWrite(rgbOn, HIGH);
  }
}

inline void blinkHigh(int pattern) {
  clearRGB();

  int rgbOn = patterns[pattern][0];
  if (rgbOn != OFF) {
    digitalWrite(rgbOn, HIGH);
  }
}

int getPattern(int temp) {
  int i;
  for (i = 0; i < NUM_CATS - 1 && temp > cats[i]; i++) {}
  return i;
}

void one_second_func() {
  int currentTemp = Temperature(4300.0f, 298.15f, 50000.0f, 50000.0f);
  insert(seconds, currentTemp);
  if(live) {
    Serial.print(seconds);
    Serial.print(",");
    Serial.println(currentTemp);
    Serial.print("Worst: ");
    Serial.println(peek());
  }

  if (currentTemp > worst || proposal[0] != 0) {
    int delt = currentTemp - proposal[1];
    if (delt > -2 && delt < 2) {
      proposal[0]++;
    } else if (currentTemp > worst) {
      proposal[0] = 0;
      proposal[1] = currentTemp;
    } else {
      resetProposal();
    }

    if (proposal[0] > 7) {
      worst = proposal[1];
      resetProposal();
      currentPattern = getPattern(currentTemp);
    }
  }
}

void loop() {
  if(second) {
    second = false;
    blinkHigh(currentPattern);
    one_second_func();
    if(seconds++ == RESET_SECONDS) {
      worst = TEMP_DEFAULT;
      currentPattern = 0;
    } else if(seconds > RESET_SECONDS - 3600) {
      digitalWrite(LED, HIGH); // blink for the last hour before restart
    }
  } else if(half_sec) {
    half_sec = false;
    blinkLow(currentPattern);
  }

  readSerial();
}

void testGrowth() {
  int i;
  int q;
  for(i = 0; i < 1 << 8; i++) {
    Serial.print(insert(i, i>>4));
    Serial.print("\t");
    Serial.print(getStats(&q));
    Serial.print("\t");
    Serial.print(q);
    Serial.print("\t");    
    Serial.println(freeMemory());
  }

  struct measurement* m = malloc(sizeof(struct measurement));
  pop(m);
  while(m->timestamp != 0) {
    Serial.print(m->timestamp);
    Serial.print("\t");
    Serial.println(m->value);
    pop(m);
  }
  free(m);
}

void dump() {
  struct measurement *m = malloc(sizeof(struct measurement));
  while(pop(m) != -1) {
    Serial.print(m->timestamp);
    Serial.print(",");
    Serial.println(m->value);
  }
  free(m);
}

void readSerial() {
  while(Serial.available() > 0) {
    buff[wPtr] = Serial.read();
    if(buff[wPtr] == (int)';' || buff[wPtr] == (int)'\n') {
      char build[16];
      int i = 0;
      while(wPtr != rPtr) {
        build[i] = (char)buff[rPtr];
        rPtr++;
        if(rPtr == 16) {
          rPtr = 0;
        }
        i++;
      }
      build[i] = '\0';
      if(strcmp(build, "dump") == 0) {
        dump();
      } else if(strcmp(build, "grow") == 0) {
        testGrowth();
      } else if(strcmp(build, "live") == 0) {
        live = !live;
      }
      Serial.println("Message Received");
      rPtr++; // get past the trailing semicolon
    }
    wPtr++;
    if(wPtr == 16) {
      wPtr = 0;
    }
  }
}
