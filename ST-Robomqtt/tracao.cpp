#include <Arduino.h>
#include "config.h"

#define ESQD D3
#define ESQV D1
#define DIRD D4
#define DIRV D2
#define BACK LOW
#define FWRD HIGH

int veloc = 0;

void motorE(bool dir, int vel)
{
      digitalWrite(ESQD,dir);
      analogWrite(ESQV,vel);
}
void motorD(boolean dir, int vel)
{
      digitalWrite(DIRD,dir);
      analogWrite(DIRV,vel);
}


void brake()
{
  motorE(FWRD,0);
  motorD(FWRD,0);
}

void setupTracao(int qvel) 
{
        pinMode(ESQD,OUTPUT);
        pinMode(ESQV,OUTPUT);
        pinMode(DIRD,OUTPUT);
        pinMode(DIRV,OUTPUT);
        brake();
}

void frente(int qvel)
{
  motorE(FWRD,qvel);
  motorD(FWRD,qvel);
}

void tras(int qvel)
{
  motorE(BACK,qvel);;
  motorD(BACK,qvel);
}

void direita(int qvel)
{
   motorE(FWRD,qvel);
   motorD(BACK,qvel);
}

void esquerda(int qvel)
{
  motorE(BACK,qvel);
  motorD(FWRD,qvel);
}

void pwmTeste()
{
  for(int i=0;i<1024;i++){
      frente(i);
      delay(15);
      Serial.println(i);
    }
    brake();
    delay(3000);
    
}
