#include <Espelho.h>

#define SERVO1 3
#define SERVO2 4
#define SERVO3 5
#define SERVO4 6
#define ADDR1 0
#define ADDR2 1
#define ADDR3 2
#define ADDR4 3
#define ADDR5 4
#define ADDR6 5
#define BTNUP 9
#define BTNDWN 11
#define BTNLEFT 12
#define BTNRIGHT 10
#define BTNSELECT 8
#define RE 2

//INSTANCIA OS OBJETOS ESPELHO
Espelho direito(SERVO1, SERVO2, ADDR1, ADDR2);
Espelho esquerdo(SERVO3, SERVO4, ADDR3, ADDR4);
Espelho* espelhoSelecionado();
Espelho *e = &esquerdo;
Espelho *d =  &direito;

void setup() {
  pinMode(BTNUP, INPUT_PULLUP);
  pinMode(BTNDWN, INPUT_PULLUP);
  pinMode(BTNLEFT, INPUT_PULLUP);
  pinMode(BTNRIGHT, INPUT_PULLUP);
  pinMode(BTNSELECT, INPUT_PULLUP);
  pinMode(RE, INPUT_PULLUP);
  d->enableTiltDown(ADDR5, ADDR6); //HABILITA O TILTDOWN NO ESPELHO DIREITO
}

void loop() {
  ajuste();
  if(reEngatada()){
    d->tiltDownMode();
  }
  if(!reEngatada()){
    d->normalMode();
  }
}

//AJUSTA O ESPELHO PELOS BOTÕES
void ajuste(){
  e->timeToDisable();
  d->timeToDisable();
  if(digitalRead(BTNUP) == LOW){
    espelhoSelecionado()->up();
    return;
  }
  if(digitalRead(BTNDWN) == LOW){
    espelhoSelecionado()->down();
    return;
  }
  if(digitalRead(BTNLEFT) == LOW){
    espelhoSelecionado()->left();
    return;
  }
  if(digitalRead(BTNRIGHT) == LOW){
    espelhoSelecionado()->right();
    return;
  }
}

//VERIFICA SE A RÉ FOI ENGATADA
bool reEngatada(){
  if(digitalRead(RE) == LOW){
    return true;
  }
  return false;
}

//VERIFICA QUAL ESPELHO FOI SELECIONADO PARA AJUSTAR
Espelho* espelhoSelecionado(){
  if(digitalRead(BTNSELECT) == LOW){
    return e;
  }
  return d;
}
