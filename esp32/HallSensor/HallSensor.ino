//Simple sketch to access the internal hall effect detector on the esp32.
//values can be quite low.
//Brian Degger / @sctv

#define TRIG 40

int i = 0;
int count = 0;
const int LED = 2;

void loop();
int averageHall(void);


void setup() {
  int hVal;
  
  Serial.begin(115200);
  pinMode(LED, OUTPUT);
  hVal = hallRead();
  Serial.print("Initial Value: ");
  Serial.println(hVal);
}

int averageHall(void) {

  int buffer[64] = { 0 };
  int hallValue = 0;

  for (i = 0; i < sizeof(buffer); i++)
  {
    hallValue += hallRead();
  }
  return ( hallValue / sizeof(buffer) );
};


void loop() {
  int hVal; 

  hVal = averageHall();
  if (hVal > TRIG)  
  { 
    count+=1;
    Serial.print("Count:");
    Serial.print(count);
    Serial.print(" ");
    Serial.println(hVal); 
    Serial.println("Magnet Passed");
    digitalWrite(LED, HIGH);
    delay(100);
    while ((hallRead()) > TRIG);
    digitalWrite(LED, LOW);
  }
};
