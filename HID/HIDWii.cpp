#include "HIDWii.h"

const int HID::VENDER_ID = 0x057E;
const int HID::PRODUCT_ID = 0x0306;

bool HID::LeeMando()
{
	return usbHID->read();
}

void HID::EscribeMando()
{
	
	if (tRR > 0 && !rumbling)
		setRumble(true);
	else if (rumbling && tRR <= 0)
		setRumble(false);												

}

/*
#define XINPUT_GAMEPAD_DPAD_UP          0x0001
#define XINPUT_GAMEPAD_DPAD_DOWN        0x0002
#define XINPUT_GAMEPAD_DPAD_LEFT        0x0004
#define XINPUT_GAMEPAD_DPAD_RIGHT       0x0008
#define XINPUT_GAMEPAD_START            0x0010
#define XINPUT_GAMEPAD_BACK             0x0020
#define XINPUT_GAMEPAD_LEFT_THUMB       0x0040
#define XINPUT_GAMEPAD_RIGHT_THUMB      0x0080
#define XINPUT_GAMEPAD_LEFT_SHOULDER    0x0100
#define XINPUT_GAMEPAD_RIGHT_SHOULDER   0x0200			
#define XINPUT_GAMEPAD_A                0x1000
#define XINPUT_GAMEPAD_B                0x2000
#define XINPUT_GAMEPAD_X                0x4000
#define XINPUT_GAMEPAD_Y                0x8000
*/


void HID::Mando2HID()
{
	if (usbHID->IsButtonPressed(2, 0x08)) 		wButtons |= XINPUT_GAMEPAD_BACK;
	else 	wButtons &= ~XINPUT_GAMEPAD_BACK;

	if (usbHID->IsButtonPressed(2, 0x08))		wButtons |= XINPUT_GAMEPAD_A;
	else  	wButtons &= ~XINPUT_GAMEPAD_A;

	if (usbHID->IsButtonPressed(2, 0x04)) wButtons |= XINPUT_GAMEPAD_B;
	else wButtons &= ~XINPUT_GAMEPAD_B;

	if (usbHID->IsButtonPressed(2, 0x02)) wButtons |= XINPUT_GAMEPAD_LEFT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;

	if (usbHID->IsButtonPressed(2, 0x01)) wButtons |= XINPUT_GAMEPAD_RIGHT_SHOULDER;
	else wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;

	usbHID->IsButtonPressed(1, 0x08) ? wButtons |= XINPUT_GAMEPAD_DPAD_UP : wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
	usbHID->IsButtonPressed(1, 0x04) ? wButtons |= XINPUT_GAMEPAD_DPAD_DOWN : wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
	usbHID->IsButtonPressed(1, 0x01) ? wButtons |= XINPUT_GAMEPAD_DPAD_LEFT : wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
	usbHID->IsButtonPressed(1, 0x02) ? wButtons |= XINPUT_GAMEPAD_DPAD_RIGHT : wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;


	//Normalizamos Joys
	fThumbLX = (float)(usbHID->getInputByte(3) - 0x80) / 25.0;  // +g=24/127 [-1.0,1.0]
	fThumbLY = -(float)(usbHID->getInputByte(4) - 0x80) / 25.0; //[-1.0,1.0]	
}

void HID::Calibra()
{
}

bool HID::open()
{
	bool openRes = usbHID->open(VENDER_ID, PRODUCT_ID);
	if (!openRes)
		return false;

	setLEDs(false, false, false, false);

	unsigned char *outBuf = usbHID->getOutputByteBuffer();

	usbHID->clearOutputByteBuffer();
	outBuf[0] = 0x12;
	outBuf[1] = 0x00;
	outBuf[2] = 0x33;
	if (usbHID->write(outBuf, 3) <= 0)
		return false;

	return true;
}

bool HID::setLEDs(bool led1, bool led2, bool led3, bool led4)
{
	unsigned char *outBuf = usbHID->getOutputByteBuffer();

	usbHID->clearOutputByteBuffer();
	outBuf[0] = 0x11;
	outBuf[1] = 0x00;
	if (led1)
		outBuf[1] |= 0x10;
	if (led2)
		outBuf[1] |= 0x20;
	if (led3)
		outBuf[1] |= 0x40;
	if (led4)
		outBuf[1] |= 0x80;
	if (usbHID->write(outBuf, 2) <= 0)
		return false;

	return true;
}

void HID::setRumble(bool on)
{
	_ASSERT(usbHID->isConnected());

	if (!usbHID->isConnected())
		return;

	if (rumbling == on)
		return;

	rumbling = on;

	BYTE buff[2] = { 0 };
	unsigned char *outBuf = usbHID->getOutputByteBuffer();

	usbHID->clearOutputByteBuffer();

	outBuf[0] = 21;
	outBuf[1] = on ? 0x01 : 0x00;
	usbHID->write(outBuf, 2);
	
}
