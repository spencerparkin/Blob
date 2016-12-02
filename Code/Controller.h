// Controller.h

#pragma once

#include <Vector.h>
#if defined _WINDOWS
#	include <Windows.h>
#	include <Xinput.h>
#endif

class Controller
{
public:

	Controller( void );
	virtual ~Controller( void );

	enum
	{
		BUTTON_A,
		BUTTON_B,
		BUTTON_X,
		BUTTON_Y,
		BUTTON_DPAD_UP,
		BUTTON_DPAD_DN,
		BUTTON_DPAD_LF,
		BUTTON_DPAD_RT,
		BUTTON_L_SHOULDER,
		BUTTON_R_SHOULDER,
		BUTTON_BACK,
		BUTTON_START,
		BUTTON_L_THUMB,
		BUTTON_R_THUMB,
		LEFT_SIDE,
		RIGHT_SIDE,
	};

	virtual bool SetupAndConnect( void ) = 0;
	virtual bool UpdateState( void ) = 0;
	virtual bool ButtonDown( int button ) = 0;
	virtual bool ButtonUp( int button );
	virtual bool ButtonPressed( int button ) = 0;
	virtual void GetAnalogTrigger( int side, double& value ) = 0;
	virtual void GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag ) = 0;
};

class XboxController : public Controller
{
public:

	XboxController( void );
	virtual ~XboxController( void );

	virtual bool SetupAndConnect( void ) override;
	virtual bool UpdateState( void ) override;
	virtual bool ButtonDown( int button ) override;
	virtual bool ButtonPressed( int button ) override;
	virtual void GetAnalogTrigger( int side, double& value ) override;
	virtual void GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag ) override;

#if defined _WINDOWS
	DWORD userIndex;
	XINPUT_STATE state[2];
	int stateIndex;

	DWORD MapButtonToNativeFlag( int button );
#endif
};

class KeyboardMouseController : public Controller
{
public:

	KeyboardMouseController( void );
	virtual ~KeyboardMouseController( void );

	virtual bool SetupAndConnect( void ) override;
	virtual bool UpdateState( void ) override;
	virtual bool ButtonDown( int button ) override;
	virtual bool ButtonPressed( int button ) override;
	virtual void GetAnalogTrigger( int side, double& value ) override;
	virtual void GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag ) override;

	unsigned char MapButtonToKeyboardKey( int button );
};

class JoyStickController : public Controller
{
public:

	JoyStickController( void );
	virtual ~JoyStickController( void );

	virtual bool SetupAndConnect( void ) override;
	virtual bool UpdateState( void ) override;
	virtual bool ButtonDown( int button ) override;
	virtual bool ButtonPressed( int button ) override;
	virtual void GetAnalogTrigger( int side, double& value ) override;
	virtual void GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag ) override;
};

// Controller.h