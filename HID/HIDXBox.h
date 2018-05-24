#include <Windows.h>
#include <XInput.h>
#include "HID.h"

#define INPUT_DEADZONE  ( 0.05f * FLOAT(0x7FFF) )  // Default to 24% of the +/- 32767 range.   This is a reasonable default value but can be altered if needed.

// Struct to hold xinput state
//-----------------------------------------------------------------------------
struct CONTROLER_STATE
{
	XINPUT_STATE State;
	XINPUT_STATE OffState; //Estado para calibrar señales analógicas

	DWORD dwResult;
	XINPUT_VIBRATION vibration;
};


//-----------------------------------------------------------------------------
//class HIDXBox : public BaseHID<HIDXBox>
class HID : public BaseHID
{
private:
	CONTROLER_STATE XBox;
	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
	void Calibra();

public:
	HID(float t) : BaseHID(t){};
};

