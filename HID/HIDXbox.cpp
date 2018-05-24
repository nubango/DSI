#include "HIDXBox.h"
#define INPUT_DEADZONE  ( 0.20f * FLOAT(MAXINT16) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.


bool HID::LeeMando()
{
	//XBox.LastState = XBox.State;
	XBox.dwResult = XInputGetState(0, &XBox.State);
	if (XBox.dwResult == ERROR_SUCCESS)		return true;
	else									return false;
}

void HID::EscribeMando()
{
	XBox.vibration.wLeftMotorSpeed = (WORD)(fLeftVibration * MAXUINT16);
	XBox.vibration.wRightMotorSpeed = (WORD)(fRightVibration * MAXUINT16);
	XInputSetState(0, &XBox.vibration);
}

void HID::Mando2HID()
{	
	wButtons = XBox.State.Gamepad.wButtons;  //Utilizo codificación XBOX para botones
	fLeftTrigger = (float)XBox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)XBox.State.Gamepad.bRightTrigger / (float)MAXBYTE;
	
	//Elimino OffSet de señales analógicas
	/* //¡¡¡Hay que mejorarlo desborda!!!
	XBox.State.Gamepad.sThumbLX -= XBox.OffState.Gamepad.sThumbLX;
	XBox.State.Gamepad.sThumbLY -= XBox.OffState.Gamepad.sThumbLY;
	XBox.State.Gamepad.sThumbRX -= XBox.OffState.Gamepad.sThumbRX;
	XBox.State.Gamepad.sThumbRY -= XBox.OffState.Gamepad.sThumbRY;
	*/

	//Nueva Zona muerta Joys
	if (XBox.State.Gamepad.sThumbLX > INPUT_DEADZONE) 			XBox.State.Gamepad.sThumbLX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLX < -INPUT_DEADZONE)		XBox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else														XBox.State.Gamepad.sThumbLX = 0;
	if (XBox.State.Gamepad.sThumbLY > INPUT_DEADZONE)			XBox.State.Gamepad.sThumbLY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLY < -INPUT_DEADZONE)		XBox.State.Gamepad.sThumbLY += INPUT_DEADZONE;
	else														XBox.State.Gamepad.sThumbLY = 0;
	if (XBox.State.Gamepad.sThumbRX > INPUT_DEADZONE) 			XBox.State.Gamepad.sThumbRX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRX < -INPUT_DEADZONE)		XBox.State.Gamepad.sThumbRX += INPUT_DEADZONE;
	else														XBox.State.Gamepad.sThumbRX = 0;
	if (XBox.State.Gamepad.sThumbRY > INPUT_DEADZONE)			XBox.State.Gamepad.sThumbRY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRY < -INPUT_DEADZONE)		XBox.State.Gamepad.sThumbRY += INPUT_DEADZONE;
	else														XBox.State.Gamepad.sThumbRY = 0;

	// Zona muerta antigua
	/*if (XBox.State.Gamepad.sThumbLX < INPUT_DEADZONE &&	XBox.State.Gamepad.sThumbLX > -INPUT_DEADZONE)
	XBox.State.Gamepad.sThumbLX = 0;
	if (XBox.State.Gamepad.sThumbLY < INPUT_DEADZONE &&	XBox.State.Gamepad.sThumbLY > -INPUT_DEADZONE)
	XBox.State.Gamepad.sThumbLY = 0;
	if (XBox.State.Gamepad.sThumbRX < INPUT_DEADZONE &&	XBox.State.Gamepad.sThumbRX > -INPUT_DEADZONE)
	XBox.State.Gamepad.sThumbRX = 0;
	if (XBox.State.Gamepad.sThumbRY < INPUT_DEADZONE &&	XBox.State.Gamepad.sThumbRY > -INPUT_DEADZONE)
	XBox.State.Gamepad.sThumbRY = 0;
	*/
	
	//Normalizamos Joys
	fThumbLX = (float)XBox.State.Gamepad.sThumbLX / (float)(MAXINT16 - INPUT_DEADZONE);//[-1.0,1.0]
	fThumbLY = (float)XBox.State.Gamepad.sThumbLY / (float)(MAXINT16 - INPUT_DEADZONE);//[-1.0,1.0]
	fThumbRX = (float)XBox.State.Gamepad.sThumbRX / (float)(MAXINT16 - INPUT_DEADZONE);//[-1.0,1.0]
	fThumbRY = (float)XBox.State.Gamepad.sThumbRY / (float)(MAXINT16 - INPUT_DEADZONE);//[-1.0,1.0]
}

void HID::Calibra()
{
	if (LeeMando())
	{
		XBox.OffState = XBox.State;
	}
}