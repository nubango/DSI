#include "HID.h"

float BaseHID::gLT() { return fLeftTrigger; }				//Triggers
float BaseHID::gRT() { return fRightTrigger; }
float BaseHID::gLJX() { return fThumbLX; }					//JoyL
float BaseHID::gLJY() { return fThumbLY; }
float BaseHID::gRJX() { return fThumbRX; }					//JoyR filtrado
float BaseHID::gRJY() { return fThumbRY; }
float BaseHID::gLJXf() { return fThumbLXf; }					//JoyR filtrado
float BaseHID::gLJYf() { return fThumbLYf; }
float BaseHID::gRJXf() { return fThumbRXf; }					//JoyR filtrado
float BaseHID::gRJYf() { return fThumbRYf; }
void BaseHID::sLR(float cantidad, float tiempo) { fLeftVibration = cantidad; tLR = tiempo; }			//Set Motores
void BaseHID::sRR(float cantidad, float tiempo) { fRightVibration = cantidad; tRR = tiempo; }
bool BaseHID::BD(WORD Bit) { return (wButtonsDown & Bit); } //Botones
bool BaseHID::BU(WORD Bit) { return (wButtonsUp & Bit); }
bool  BaseHID::GR() { return (Ro == np); }			//Gesto de Rotación

void BaseHID::Actualiza() 
{
	wLastButtons = wButtons;
	bConected = LeeMando(); //Leo Mando
	if (bConected == true)
		{
			Mando2HID();	//Vuelco Mando2HID
		
		//Detecto Gestos genéricos
			wButtonsDown = (~wLastButtons)&(wButtons);
			wButtonsUp = (wLastButtons)&(~wButtons);

		//Filtro Thumbs
			fThumbLXf = (1 - a)*fThumbLXf + a*fThumbLX;
			fThumbLYf = (1 - a)*fThumbLYf + a*fThumbLY;

			fThumbRXf = (1 - a)*fThumbRXf + a*fThumbRX;
			fThumbRYf = (1 - a)*fThumbRYf + a*fThumbRY;

			//Gesto de Rotación
			if ((fThumbLX>0)&(fThumbLY > 0))
			{	Ro = pp;
				tRo = 1.0;
			}
			if (tRo > 0)
			{	tRo = tRo - T;
				if (Ro == np) Ro = pp;
				if ((fThumbLX > 0)&(fThumbLY <= 0)&(Ro == pp))	Ro = pn;
				if ((fThumbLX <= 0)&(fThumbLY < 0)&(Ro == pn))  Ro = nn;
				if ((fThumbLX < 0)&(fThumbLY >= 0)&(Ro == nn))	Ro = np;
			}
			else Ro = pp;
			
			//Actualizo tiempos de de feedback (Salida)
			if (tLR>0)	tLR -= T/1000; 
			else sLR(0.0, 0.0);
			if (tRR>0)	tRR -= T/1000; 
			else sRR(0.0, 0.0);

			
			//Escribo en Mando el feedback
			EscribeMando();

		}
	}
