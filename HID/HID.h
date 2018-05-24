
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


typedef unsigned short      WORD;
enum Rotacion { pp, pn, nn, np };	//Estados del gesto de rotación

class BaseHID
{
public:
	bool bConected;		//Mando Conectado

	//Gets & Sets 
	float gLT();			//Trigers
	float gRT();
	float gLJX();		//LeftJoyX
	float gLJY();		//LeftJoyY
	float gRJX();		//RightJoyX
	float gRJY();		//RightJoyY
	float gLJXf();		//LeftJoyXfiltered
	float gLJYf();		//LeftJoyYfiltered
	float gRJXf();		//RightJoyXfiltered
	float gRJYf();		//RightJoyY
	void  sLR(float cantidad, float tiempo);	//LeftRumble [0,1]: cantidad [0,1], tiempo [0,inf]
	void  sRR(float cantidad, float tiempo);	//RightRumble [0,1]: cantidad [0,1], tiempo [0,inf]

	//gestos
	bool BD(WORD Bit);	//botones
	bool BU(WORD Bit);
	bool GR();			//Gesto de Rotación

	//Utilidades	
	BaseHID() {
		//T = t/1000.0; 
		//a = T / (0.1 + T);			//
	};
	BaseHID(float t) { T = t; a = T / (0.1 + T); };
	~BaseHID() {};
	void Actualiza();				//Actualiza Mando2HID y HID2Mando.

protected:
	//Entradas
	WORD  wButtons;		//Botones (Utilizo Codificación Xbox360)
	WORD  wLastButtons;	//Botones anteriores (Utilizo Codificación Xbox360)
	float fLeftTrigger, fRightTrigger;			//[0.0,1.0]
	float fThumbLX, fThumbLY, fThumbRX, fThumbRY;//[-1.0,1.0]
	//Filtrado de Joys
	float fThumbLXf, fThumbLYf, fThumbRXf, fThumbRYf;					//[-1.0,1.0] Filtrado
	float T = 0.01;					//Periodo de muestreo en s
	float a = T / (0.1 + T);					//Cte.Tiempo Filtro ThumbR 
	//Salidas
	float fLeftVibration, fRightVibration;		//[0.0,1.0]  Salida
	float tLR = 0.0;			//Tiempo que queda de vibración LR
	float tRR = 0.0;			//Tiempo que queda de vibración RR
	//Gestos
	WORD wButtonsDown;	//EventosDown Botones (Codificación Xbox360?)
	WORD wButtonsUp;	//EventosUp Botones (Codificación Xbox360?)	
	Rotacion Ro;		//Estados del gesto de rotación
	float tRo = 0.0;		//Tiempo que queda para el gesto	
	//funciones Virtuales
	virtual bool LeeMando() = 0;		//Lee estado del mando
	virtual void EscribeMando() = 0;	//Escribe Feeback en mando
	virtual void Mando2HID() = 0;		//Vuelca el estado del mando al HID
	virtual void Calibra() = 0;		//Calibra Mando
};

