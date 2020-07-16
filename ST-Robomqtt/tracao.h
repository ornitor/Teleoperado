#include <Arduino.h>


void setupTracao(int qvel);
float dEsq();
float dDir();
float dFrente();
void esquerdaAte(float d);
void direitaAte(float d);

void velocidade(String s);
void tras();
void toogleservo();
void pilotagem(int piloto);
void release();
void motor(char m, char dir, char vel);
void frente(int qvel);
void tras(int qvel);
void esquerda(int qvel);
void direita(int qvel);
void brake();

void direita(float t);
void esquerda(float t);
void frente(float t);
void tras(float t);
void direita(String s);
void esquerda(String s);
void frente(String s);
void tras(String s);
void pilotagem(String s);
void pwmTeste();
void espiral(String s);

boolean orientaMaxd(String s);
float xdistancia(float d0, float t, float v);
void espiral(String s);

void receiveEvent(int howMany);
void setupComando()  ;
void comando() ;

void sensor(String s);

extern int veloc;
