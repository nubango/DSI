#define Tm 10   //ms para actualizar

#define TARGET_WII
//#define TARGET_XBOX360
#ifdef TARGET_XBOX360
#include "HIDXBox.h"
#elif defined(TARGET_PS3)
#include "HIDXPS3.h"
#elif defined(TARGET_WII)
#include "HIDWii.h"
#endif

#include <math.h>

HID Control(Tm);
void GeneraEfectos(HID *Control);
VOID CALLBACK TimerCallBack();


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR, int nCmdShow)
{
	//Control.Calibra();
	UINT_PTR	ptimerxbox;
	ptimerxbox = SetTimer(NULL, NULL, Tm, (TIMERPROC)TimerCallBack);
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	KillTimer(NULL, ptimerxbox);
	return 0;
}

VOID CALLBACK TimerCallBack()
{
	Control.Actualiza();	//Actualiza nuestro HID
	GeneraEfectos(&Control);//Genera Eventos sobre Windows
}

void GeneraEfectos(HID *Control)
{
	POINT pt, pt0;                  // cursor location  
	GetCursorPos(&pt);
	//HWND hWnd = GetActiveWindow();

	//JoyL2Ratón Relativo
	if ((fabs(Control->gLJXf()) > 0.05) | (fabs(Control->gLJYf()) > 0.05))
	{
		pt.x += 20 * Control->gLJXf();
		pt.y -= 20 * Control->gLJYf();
		SetCursorPos(pt.x, pt.y);
	}

	//JoyR2ratón Absoluto
	RECT Rect = { 0, 0, 1024, 600 }; //Por defecto
	HWND Desk = GetDesktopWindow();
	GetWindowRect(Desk, &Rect);
	if ((fabs(Control->gRJXf()) > 0.01) | (fabs(Control->gRJYf()) > 0.01))
	{
		pt.x = Rect.right / 2 + Rect.right * 3 / 4 * Control->gRJXf();
		pt.y = Rect.bottom / 2 - Rect.bottom * 3 / 4 * Control->gRJYf();
		SetCursorPos(pt.x, pt.y);
	}

	//Eventos de Ratón
	if (Control->BD(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, NULL);
	if (Control->BD(XINPUT_GAMEPAD_A))
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_A))
		mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, NULL);
	if (Control->BD(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, NULL);
	if (Control->BD(XINPUT_GAMEPAD_B))
		mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_B))
		mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, NULL);
	if (Control->gLT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(40 * Control->gLT()), NULL);
	if (Control->gRT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, -(DWORD)(40 * Control->gRT()), NULL);

	//Eventos de Teclado
	if (Control->BD(XINPUT_GAMEPAD_BACK))
		PostQuitMessage(0);
	if (Control->BD(XINPUT_GAMEPAD_DPAD_UP))
		keybd_event(VK_UP, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_UP))
		keybd_event(VK_UP, 0x0, KEYEVENTF_KEYUP, 0);
	if (Control->BD(XINPUT_GAMEPAD_DPAD_DOWN))
		keybd_event(VK_DOWN, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_DOWN))
		keybd_event(VK_DOWN, 0x0, KEYEVENTF_KEYUP, 0);
	if (Control->BD(XINPUT_GAMEPAD_DPAD_LEFT))
		keybd_event(VK_LEFT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_LEFT))
		keybd_event(VK_LEFT, 0x0, KEYEVENTF_KEYUP, 0);
	if (Control->BD(XINPUT_GAMEPAD_DPAD_RIGHT))
		keybd_event(VK_RIGHT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_DPAD_RIGHT))
		keybd_event(VK_RIGHT, 0x0, KEYEVENTF_KEYUP, 0);

	//Si se ha pulsado algún botón
	if (Control->BD(0xFFFF))
		Control->sRR(0.5, 0.1);

	if (Control->GR()) //Gesto Rotación Reconocido
		Control->sLR(1.0, 1.0);
}