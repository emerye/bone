#include <stdio.h>
#include <wiringPi.h>

void gpioTest() {
  int ret; 
 
  ret = wiringPiSetup();  
  if(ret == -1) {
    printf("Error number %d\n",ret); 
    perror("Opening wiring Pi"); 
  }
  pinMode(29,OUTPUT); 
  
  for (int i=0; i<2000; i++) {
    digitalWrite(29,LOW);
    digitalWrite(29,HIGH);      
  } 
}
      


int main(int numArgs, char *args[]) {
  gpioTest();  
  return 0; 
}
