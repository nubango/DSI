#include "HIDXBox.h"

bool HIDXBox::LeeMando()
{
	XBox.dwResult = XInputGetState(0, &XBox.State);

	if (XBox.dwResult == ERROR_SUCCESS)
		return true;
	else
		return false;
}

void HIDXBox::EscribeMando()
{
	XBox.vibration.wLeftMotorSpeed = (WORD)(fLeftVibration * MAXUINT16);
	XBox.vibration.wRightMotorSpeed = (WORD)(fRightVibration * MAXUINT16);
	XInputSetState(0, &XBox.vibration);
}

void HIDXBox::Mando2HID()
{
	wButtons = XBox.State.Gamepad.wButtons; // Utiliza codificacion XBOX para botones
	fLeftTrigger = (float)XBox.State.Gamepad.bLeftTrigger / (float)MAXBYTE;
	fRightTrigger = (float)XBox.State.Gamepad.bRightTrigger / (float)MAXBYTE;

	if (XBox.State.Gamepad.sThumbLX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLX += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbLX = 0;

	if (XBox.State.Gamepad.sThumbLY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbLY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbLY += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbLY = 0;

	if (XBox.State.Gamepad.sThumbRX > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRX -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRX < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRX += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbRX = 0;

	if (XBox.State.Gamepad.sThumbRY > INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY -= INPUT_DEADZONE;
	else if (XBox.State.Gamepad.sThumbRY < -INPUT_DEADZONE)
		XBox.State.Gamepad.sThumbRY += INPUT_DEADZONE;
	else
		XBox.State.Gamepad.sThumbRY = 0;

	// Normalizamos Joys
	fThumbLX = (float)XBox.State.Gamepad.sThumbLX / (float)(MAXINT16 - INPUT_DEADZONE); // [-1.0, 1.0]
	fThumbLY = (float)XBox.State.Gamepad.sThumbLY / (float)(MAXINT16 - INPUT_DEADZONE); // [-1.0, 1.0]
	fThumbRX = (float)XBox.State.Gamepad.sThumbRX / (float)(MAXINT16 - INPUT_DEADZONE); // [-1.0, 1.0]
	fThumbRY = (float)XBox.State.Gamepad.sThumbRY / (float)(MAXINT16 - INPUT_DEADZONE); // [-1.0, 1.0]
}

void HIDXBox::Calibra()
{
	if (LeeMando())
		XBox.OffState = XBox.State;
}
