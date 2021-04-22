/*
 Espelho.cpp - Biblioteca Arduino para controle de espelhos elétricos utilizando servos motores.
 Copyright (c) 2021 André Olive.  Todos os direitos reservados.

 Esta Biblioteca é um software livre; você pode redistribuir ou
 modificar-la livremente.  

 Esta Biblioteca foi desenvolvida com a esperança de ser útil,
 mas SEM NENHUMA GARANTIA; seu funcionamento foi testado e se mostrou
 funcional.

 */

#include "Arduino.h"
#include "Espelho.h"
#include "Servo.h"
#include "EEPROM.h"

#define DELAY 100
#define SECONDS_TO_DISABLE 5000

static Espelho *espelhos[2];
uint8_t EspelhoCount = 0;

void restorePositions(){
	for(int i=0; i < EspelhoCount; i++){
		espelhos[i]->restorePosition();
	}
}

//CONSTRUTOR
Espelho::Espelho(int pin1, int pin2, int addr1, int addr2)
{
	espelhos[EspelhoCount] = this;
	EspelhoCount++;
	this->pin1 = pin1;
  	this->pin2 = pin2;
	this->addr1 = addr1;
	this->addr2 = addr2;
	this->loadPosition();
}

//CONSTRUTOR PASSANDO POSIÇÃO MINIMA E MÁXIMA
Espelho::Espelho(int pin1, int pin2, int addr1, int addr2, uint8_t min, uint8_t max)
{
	espelhos[EspelhoCount] = this;
	EspelhoCount++;
	this->pin1 = pin1;
  	this->pin2 = pin2;
	this->addr1 = addr1;
	this->addr2 = addr2;
	this->minPos = min;
	this->maxPos = max;
	this->loadPosition();
}

//VERIFICA O TEMPO DO ULTIMO MOVIMENTO DO ESPELHO PARA DESABILITAR APOS 5 SEGUNDOS
void Espelho::timeToDisable()
{
	if(this->isEnabled)
	{
		if(millis() >= this->lastChange + SECONDS_TO_DISABLE)
		{
			this->disable();
		}
	}
}

//HABILITA A FUNÇÃO TILTDOWN DO ESPELHO
void Espelho::enableTiltDown(int addr3, int addr4)
{
	tiltDownIsEnabled = true;
	this->addr3 = addr3;
	this->addr4 = addr4;
	this->loadTiltDownPosition();
}

//HABILITA OS MOTORES DO ESPELHO
void Espelho::enable()
{
	if(!this->isEnabled)
	{
		this->isEnabled = true;
		this->servoX.attach(this->pin1);
		this->servoY.attach(this->pin2);
	}
}

//DESABILITA OS MOTORES DO ESPELHO
void Espelho::disable()
{
	if(this->isEnabled)
	{
		this->isEnabled = false;
		this->servoX.detach();
		this->servoY.detach();
		this->savePosition();
	}
}

//SALVA NO EEPROM A POSIÇÂO DOS ESPELHOS
void Espelho::savePosition()
{
	if(this->editNormalPos)
	{
		this->editNormalPos = false;
		EEPROM.write(this->addr1, this->normalPosX);
		EEPROM.write(this->addr2, this->normalPosY);
	}
}

//SALVA NO EEPROM A POSIÇÂO DE TILTDOWN
void Espelho::saveTiltDownPosition()
{
	if(this->editTiltDownPos)
	{
		this->editTiltDownPos = false;
		EEPROM.write(this->addr3, this->tiltDwnPosX);
		EEPROM.write(this->addr4, this->tiltDwnPosY);
		this->hasTiltDownPos = true;
	}
}

//CARREGA POSIÇÃO DO ESPELHO DA EEPROM
void Espelho::loadPosition()
{
	if(EEPROM.read(this->addr1) && EEPROM.read(this->addr2))
	{
		this->normalPosX = EEPROM.read(this->addr1);
		this->normalPosY = EEPROM.read(this->addr2);
	}
	else 
	{
		this->normalPosX = 90;
		this->normalPosY = 90;
	}
}

//CARREGA POSIÇÃO DO TILTDONW DA EEPROM
void Espelho::loadTiltDownPosition()
{
	if(EEPROM.read(this->addr3) && EEPROM.read(this->addr4))
	{
		this->hasTiltDownPos = true;
		this->tiltDwnPosX = EEPROM.read(this->addr3);
		this->tiltDwnPosY = EEPROM.read(this->addr4);
	}
	else 
	{
		this->tiltDwnPosX = this->normalPosX;
		this->tiltDwnPosY = this->normalPosY;
	}
}

//RESTAURA POSIÇÕES DOS ESPELHOS
void Espelho::restorePosition(){
	if(!this->inTiltDownMode){
		this->goToNormalPosition();
	}
}

//MOVE O ESPELHO PRA POSIÇÂO NORMAL
void Espelho::goToNormalPosition()
{
	this->enable();
	this->lastChange = millis();
	this->servoX.write(this->normalPosX);
	this->servoY.write(this->normalPosY);
}

//MOVE O ESPELHO PRA POSIÇÂO TILTDONW
void Espelho::goToTiltdownPosition()
{
	if(this->hasTiltDownPos)
	{
		this->enable();
		this->lastChange = millis();
		this->servoX.write(this->tiltDwnPosX);
		this->servoY.write(this->tiltDwnPosY);
		return;
	}
	this->tiltDwnPosX = this->normalPosX;
	this->tiltDwnPosY = this->normalPosY;
	this->hasTiltDownPos = true;
	this->goToNormalPosition();
}

//MUDA PARA O MODO TILTDOWN
void Espelho::tiltDownMode()
{
	if(!this->inTiltDownMode && this->tiltDownIsEnabled)
	{
		this->inTiltDownMode=true;
		this->goToTiltdownPosition();
    }
}

//MUDA PARA MODO NORMAL E VERIFICA SE FOI ALTERADA A POSIÇÂO DE TILTDOWN PARA SALVAR A NOVA POSIÇÂO
void Espelho::normalMode()
{
	if(this->inTiltDownMode)
	{
		this->inTiltDownMode=false;
		this->goToNormalPosition();
		this->saveTiltDownPosition();		
    }
}

//MOVIMENTOS DO ESPELHO

void Espelho::up()
{
	if(!this->inTiltDownMode)
	{
		this->editNormalPos = true;
		this->normalPosY < this->maxPos ? this->normalPosY++ : this->normalPosY = this->maxPos;
		this->goToNormalPosition();
	}
	else if(this->inTiltDownMode)
	{
		this->editTiltDownPos = true;
		this->tiltDwnPosY < this->maxPos ? this->tiltDwnPosY++ : this->tiltDwnPosY = this->maxPos;
		this->goToTiltdownPosition();
	}
    delay(DELAY);
}
void Espelho::down()
{
	if(!this->inTiltDownMode)
	{
		this->editNormalPos = true;
		this->normalPosY > this->minPos ? this->normalPosY-- : this->normalPosY = this->minPos;
		this->goToNormalPosition();
	}
	else if(this->inTiltDownMode)
	{
		this->editTiltDownPos = true;
		this->tiltDwnPosY > this->minPos ? this->tiltDwnPosY-- : this->tiltDwnPosY = this->minPos;
		this->goToTiltdownPosition();
	}
    delay(DELAY);
}
void Espelho::left()
{
	if(!this->inTiltDownMode)
	{
		this->editNormalPos = true;
		this->normalPosX > this->minPos ? this->normalPosX-- : this->normalPosX = this->minPos;
		this->goToNormalPosition();
	}
	else if(this->inTiltDownMode)
	{
		this->editTiltDownPos = true;
		this->tiltDwnPosX > this->minPos ? this->tiltDwnPosX-- : this->tiltDwnPosX = this->minPos;
		this->goToTiltdownPosition();
	}
    delay(DELAY);
}
void Espelho::right()
{
	if(!this->inTiltDownMode)
	{
		this->editNormalPos = true;
		this->normalPosX < this->maxPos ? this->normalPosX++ : this->normalPosX = this->maxPos;
		this->goToNormalPosition();
	}
	else if(this->inTiltDownMode)
	{
		this->editTiltDownPos = true;
		this->tiltDwnPosX < this->maxPos ? this->tiltDwnPosX++ : this->tiltDwnPosX = this->maxPos;
		this->goToTiltdownPosition();
	}
    delay(DELAY);
}
