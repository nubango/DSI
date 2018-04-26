#include "BaseHID.h"

float BaseHID::gLT(){ return fLeftTrigger; }
float BaseHID::gRT(){ return fRightTrigger; }
float BaseHID::gLJX() { return fThumbLX; }
float BaseHID::gLJY() { return fThumbLY; }
float BaseHID::gRJX() { return fThumbRX; }
float BaseHID::gRJY() { return fThumbRY; }
float BaseHID::gLJXf() { return fThumbLXf; }
float BaseHID::gLJYf() { return fThumbLYf; }
float BaseHID::gRJXf() { return fThumbRXf; }
float BaseHID::gRJYf() { return fThumbRYf; }
void BaseHID::sLR(float cantidad, float tiempo) { fLeftVibration = cantidad; tLR = tiempo; }
void BaseHID::sRR(float cantidad, float tiempo) { fRightVibration = cantidad; tRR = tiempo; }

bool BaseHID::BD(WORD Bit) { return (wButtonsDown & Bit); }
bool BaseHID::BU(WORD Bit) { return (wButtonsUp & Bit); }
bool BaseHID::GRLJ() { return (Ro == np); } // Gesto de Rotacion

void BaseHID::Actualiza()
{
	wLastButtons = wButtons; // Copia estado de botones
	bConected = LeeMando(); // Leo Mando
	if (bConected == true)
	{
		Mando2HID(); // Vuelco de Mando a HID normalizando
					 // Actualizo Gestos de entrada genéricos (entradas)

		// Detecta gestos genericos
		wButtonsDown = (~wLastButtons)&(wButtons);
		wButtonsUp = (wLastButtons)&(~wButtons);

		// Filtro Thumbs
		fThumbLXf = (1 - a)*fThumbLXf + a*fThumbLX;
		fThumbLYf = (1 - a)*fThumbLYf + a*fThumbLY;

		fThumbRXf = (1 - a)*fThumbRXf + a*fThumbRX;
		fThumbRYf = (1 - a)*fThumbRYf + a*fThumbRY;

		// Genero Gesto de feedback (salida)
		if ((fThumbLX > 0) &(fThumbLY > 0))
		{
			Ro = pp;
			tRo = 1.0;
		}
		if (tRo > 0) 
		{
			tRo = tRo - T;
			if (Ro == np) 
				Ro = pp;
			if ((fThumbLX > 0)&(fThumbLY <= 0)&(Ro == pp))
				Ro = pn;
			if ((fThumbLX <= 0)&(fThumbLY < 0)&(Ro == pn))
				Ro = nn;
			if ((fThumbLX < 0)&(fThumbLY >= 0)&(Ro == nn))
				Ro = np;
		}
		else 
			Ro = pp;

		// Actualiza tiempos de feedback (Salida)
		if (tLR > 0) 
			tLR -= T;
		else 
			sLR(0.0, 0.0);
		if (tRR > 0)
			tRR -= T;
		else
			sRR(0.0, 0.0);

		EscribeMando(); //Escribo en Mando el feedback
	}
}