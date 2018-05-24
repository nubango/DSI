#include "HID.h"
#include "UsbHID.h"

#pragma once
class HID : public BaseHID
{
	static const int VENDER_ID;
	static const int PRODUCT_ID;

private:
	bool LeeMando();
	void EscribeMando();
	void Mando2HID();
	void Calibra();

	UsbHID* usbHID;

public:
	HID(float t) :BaseHID(t) { usbHID = new UsbHID(); open(); }
	~HID() {};

	bool open();
	bool setLEDs(bool led1, bool led2, bool led3, bool led4);
	void setRumble(bool on);
};

