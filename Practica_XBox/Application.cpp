#include "HIDXbox.h"
#include <math.h>

#define T 10 //ms para actualizar
#define TARGET_XBOX360
#ifdef TARGET_XBOX360
HIDXBox Control(T);
#elif defined(TARGET_PS3)
HIDPs Control(T);
#elif defined(TARGET_WII)
HIDWii Control(T);
#endif

void GeneraEfectos(HIDXBox * Control);
VOID CALLBACK TimerCallBack();

HWND hWndAnt;
HWND hWnd;

int WINAPI wWinMain(HINSTANCE hInst, HINSTANCE, LPWSTR, int nCmdShow)
{
	Control.Calibra();
	UINT_PTR ptimerxbox;
	ptimerxbox = SetTimer(NULL, NULL, T, (TIMERPROC)TimerCallBack);
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
	Control.Actualiza();		// Actualiza nuestro HID
	GeneraEfectos(&Control);	// Genera los efectos en la aplicación en función de los gestos del control
}

void GeneraEfectos(HIDXBox * Control)
{
	POINT pt, pt0;
	GetCursorPos(&pt);

	// JoyL2Raton Relativo
	if ((fabs(Control->gLJX()) > 0.05) | (fabs(Control->gLJY()) > 0.05))
	{
		pt.x += 20 * Control->gLJX();
		pt.y -= 20 * Control->gLJY();
		SetCursorPos(pt.x, pt.y);
	}

	// JoyR2Raton Relativo filtrado
	if ((fabs(Control->gRJXf()) > 0.05) | (fabs(Control->gRJYf()) > 0.05))
	{
		pt.x += 20 * Control->gRJXf();
		pt.y -= 20 * Control->gRJYf();
		SetCursorPos(pt.x, pt.y);
	}

	// Eventos de Raton \\

	// El RB actua como el boton derecho del raton
	if (Control->BD(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_RIGHT_SHOULDER))
		mouse_event(MOUSEEVENTF_RIGHTUP, pt.x, pt.y, 0, NULL);

	// El LB actua como el boton izquierdo del raton
	if (Control->BD(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTDOWN, pt.x, pt.y, 0, NULL);
	if (Control->BU(XINPUT_GAMEPAD_LEFT_SHOULDER))
		mouse_event(MOUSEEVENTF_LEFTUP, pt.x, pt.y, 0, NULL);

	// El LT actua como rueda de raton hacia abajo
	if (Control->gLT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(-40 * Control->gLT()), 0);

	// El RT actua como rueda de raton hacia arriba
	if (Control->gRT() != 0)
		mouse_event(MOUSEEVENTF_WHEEL, pt.x, pt.y, (DWORD)(40 * Control->gRT()), 0);

	// Eventos de teclado \\

	// El boton back ESC (cierra la aplicacion)
	if (Control->BD(XINPUT_GAMEPAD_BACK))
		PostQuitMessage(0);

	// El boton Start actua como Enter
	if (Control->BD(XINPUT_GAMEPAD_START))
		keybd_event(VK_RETURN, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_START))
		keybd_event(VK_RETURN, 0x0, KEYEVENTF_KEYUP, 0);

	// El DPAD actua como las flechas de direccion
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

	// El boton B actua como la tecla FIN
	if (Control->BD(XINPUT_GAMEPAD_B))
		keybd_event(VK_END, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_B))
		keybd_event(VK_END, 0x0, KEYEVENTF_KEYUP, 0);

	// El boton X actua como la tecla INICIO
	if (Control->BD(XINPUT_GAMEPAD_X))
		keybd_event(VK_HOME, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_X))
		keybd_event(VK_HOME, 0x0, KEYEVENTF_KEYUP, 0);

	// El boton Y actua como AV PAG
	if (Control->BD(XINPUT_GAMEPAD_Y))
		keybd_event(VK_PRIOR, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_Y))
		keybd_event(VK_PRIOR, 0x0, KEYEVENTF_KEYUP, 0);

	// El boton A actua como RE PAG
	if (Control->BD(XINPUT_GAMEPAD_A))
		keybd_event(VK_NEXT, 0x0, 0, 0);
	if (Control->BU(XINPUT_GAMEPAD_A))
		keybd_event(VK_NEXT, 0x0, KEYEVENTF_KEYUP, 0);

	// Si se ha pulsado algun boton vibra el motor izquierdo
	if (Control->BD(0xFFFF))
		Control->sLR(0.5, 0.1);

	// Gestiones de Ventana \\

	hWndAnt = hWnd;
	hWnd = GetForegroundWindow();
	// Gesto rotacion
	if (Control->GRLJ())
		ShowWindow(hWnd, SW_MINIMIZE);
	// Si se cambia de ventana vibra el motor derecho
	if (hWnd != hWndAnt)
		Control->sRR(0.5, 0.2);
}