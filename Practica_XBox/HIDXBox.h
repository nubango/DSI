#pragma once

#include "BaseHID.h"
#include <XInput.h>
#pragma comment(lib, "xinput9_1_0.lib")

#define INPUT_DEADZONE (0.05f * FLOAT(0x7FFF)) // Default to 24% of the +/- 32767 range

//Struct to hold xinput state
struct CONTROLER_STATE 
{
	XINPUT_STATE State;
	XINPUT_STATE OffState; 
	DWORD dwResult;
	XINPUT_VIBRATION vibration;
};

class HIDXBox : public BaseHID
{
private:
	CONTROLER_STATE XBox;

public:
	HIDXBox(float t) :BaseHID(t) {}
	~HIDXBox() {}
	virtual bool LeeMando();
	virtual void EscribeMando();
	virtual void Mando2HID();
	virtual void Calibra();
};
