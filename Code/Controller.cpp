// Controller.cpp

#include "Controller.h"
#include <wx/joystick.h>

//----------------------------------------------------------------------------------------------
//                                        Controller
//----------------------------------------------------------------------------------------------

Controller::Controller( void )
{
}

/*virtual*/ Controller::~Controller( void )
{
}

//----------------------------------------------------------------------------------------------
//                                        XboxController
//----------------------------------------------------------------------------------------------

XboxController::XboxController( void )
{
#if defined _WINDOWS
	userIndex = -1;
	stateIndex = 0;
#endif
}

/*virtual*/ XboxController::~XboxController( void )
{
}

/*virtual*/ bool XboxController::SetupAndConnect( void )
{
#if defined _WINDOWS
	ZeroMemory( &state[0], sizeof( XINPUT_STATE ) );
	ZeroMemory( &state[1], sizeof( XINPUT_STATE ) );

	for( userIndex = 0; userIndex < XUSER_MAX_COUNT; userIndex++ )
	{
		if( ERROR_SUCCESS == XInputGetState( userIndex, &state[ stateIndex ] ) )
		{
			stateIndex = 1 - stateIndex;
			return true;
		}
	}

	return false;
#elif defined _LINUX
	return false;
#else
	return false;
#endif
}

/*virtual*/ bool XboxController::UpdateState( void )
{
#if defined _WINDOWS
	ZeroMemory( &state[ stateIndex ], sizeof( XINPUT_STATE ) );
	if( ERROR_SUCCESS != XInputGetState( userIndex, &state[ stateIndex ] ) )
		return false;

	int otherStateIndex = 1 - stateIndex;
	if( state[ otherStateIndex ].dwPacketNumber != state[ stateIndex ].dwPacketNumber )
		stateIndex = otherStateIndex;

	return true;
#else
	return false;
#endif
}

/*virtual*/ bool XboxController::ButtonDown( int button )
{
	int otherStateIndex = 1 - stateIndex;
	XINPUT_STATE* currentState = &state[ otherStateIndex ];
	DWORD currentButtons = currentState->Gamepad.wButtons;
	DWORD nativeFlag = MapButtonToNativeFlag( button );
	return( currentButtons & nativeFlag ) ? true : false;
}

/*virtual*/ bool XboxController::ButtonUp( int button )
{
	return !ButtonDown( button );
}

/*virtual*/ bool XboxController::ButtonPressed( int button )
{
	int otherStateIndex = 1 - stateIndex;
	XINPUT_STATE* currentState = &state[ otherStateIndex ];
	XINPUT_STATE* previousState = &state[ stateIndex ];
	DWORD currentButtons = currentState->Gamepad.wButtons;
	DWORD previousButtons = previousState->Gamepad.wButtons;
	DWORD nativeFlag = MapButtonToNativeFlag( button );
	return( ( previousButtons & nativeFlag ) != 0 && ( currentButtons & nativeFlag ) == 0 ) ? true : false;
}

#if defined _WINDOWS
DWORD XboxController::MapButtonToNativeFlag( int button )
{
	switch( button )
	{
		case BUTTON_A:				return XINPUT_GAMEPAD_A;
		case BUTTON_B:				return XINPUT_GAMEPAD_B;
		case BUTTON_X:				return XINPUT_GAMEPAD_X;
		case BUTTON_Y:				return XINPUT_GAMEPAD_Y;
		case BUTTON_DPAD_UP:		return XINPUT_GAMEPAD_DPAD_UP;
		case BUTTON_DPAD_DN:		return XINPUT_GAMEPAD_DPAD_DOWN;
		case BUTTON_DPAD_LF:		return XINPUT_GAMEPAD_DPAD_LEFT;
		case BUTTON_DPAD_RT:		return XINPUT_GAMEPAD_DPAD_RIGHT;
		case BUTTON_L_SHOULDER:		return XINPUT_GAMEPAD_LEFT_SHOULDER;
		case BUTTON_R_SHOULDER:		return XINPUT_GAMEPAD_RIGHT_SHOULDER;
		case BUTTON_BACK:			return XINPUT_GAMEPAD_BACK;
		case BUTTON_START:			return XINPUT_GAMEPAD_START;
		case BUTTON_L_THUMB:		return XINPUT_GAMEPAD_LEFT_THUMB;
		case BUTTON_R_THUMB:		return XINPUT_GAMEPAD_RIGHT_THUMB;
	}

	return 0;
}
#endif //_WINDOWS

/*virtual*/ void XboxController::GetAnalogTrigger( int side, double& value )
{
#ifdef _WINDOWS
	int otherStateIndex = 1 - stateIndex;
	XINPUT_STATE* currentState = &state[ otherStateIndex ];
	BYTE trigger = 0;
	if( side == LEFT_SIDE )
		trigger = currentState->Gamepad.bLeftTrigger;
	else if( side == RIGHT_SIDE )
		trigger = currentState->Gamepad.bRightTrigger;
	if( trigger < XINPUT_GAMEPAD_TRIGGER_THRESHOLD )
		trigger = 0;
	value = double( trigger ) / 255.0;
#endif
}

/*virtual*/ void XboxController::GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag )
{
#ifdef _WINDOWS
	int otherStateIndex = 1 - stateIndex;
	XINPUT_STATE* currentState = &state[ otherStateIndex ];
	int thumbX = 0, thumbY = 0;
	int deadZone = 0;
	if( side == LEFT_SIDE )
	{
		thumbX = currentState->Gamepad.sThumbLX;
		thumbY = currentState->Gamepad.sThumbLY;
		deadZone = XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;
	}
	else if( side == RIGHT_SIDE )
	{
		thumbX = currentState->Gamepad.sThumbRX;
		thumbY = currentState->Gamepad.sThumbRY;
		deadZone = XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;
	}
	if( abs( thumbX ) < deadZone )
		thumbX = 0;
	if( abs( thumbY ) < deadZone )
		thumbY = 0;
	unitDir.x = double( thumbX ) / 32768.0;
	unitDir.y = double( thumbY ) / 32768.0;
	unitDir.z = 0.0;
	mag = unitDir.Length();
	if( mag != 0.0 )
		unitDir.Scale( 1.0 / mag );
#endif
}

//----------------------------------------------------------------------------------------------
//                                     KeyboardMouseController
//----------------------------------------------------------------------------------------------

KeyboardMouseController::KeyboardMouseController( void )
{
}

/*virtual*/ KeyboardMouseController::~KeyboardMouseController( void )
{
}

/*virtual*/ bool KeyboardMouseController::SetupAndConnect( void )
{
	return true;
}

/*virtual*/ bool KeyboardMouseController::UpdateState( void )
{
	//...can we directly query keyboard and mouse state using wx?
	return false;
}

/*virtual*/ bool KeyboardMouseController::ButtonDown( int button )
{
	return false;
}

/*virtual*/ bool KeyboardMouseController::ButtonUp( int button )
{
	return false;
}

/*virtual*/ bool KeyboardMouseController::ButtonPressed( int button )
{
	return false;
}

/*virtual*/ void KeyboardMouseController::GetAnalogTrigger( int side, double& value )
{
}

/*virtual*/ void KeyboardMouseController::GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag )
{
}

//----------------------------------------------------------------------------------------------
//                                      JoyStickController
//----------------------------------------------------------------------------------------------

JoyStickController::JoyStickController( void )
{
}

/*virtual*/ JoyStickController::~JoyStickController( void )
{
}

/*virtual*/ bool JoyStickController::SetupAndConnect( void )
{
	return false;
}

/*virtual*/ bool JoyStickController::UpdateState( void )
{
	return false;
}

/*virtual*/ bool JoyStickController::ButtonDown( int button )
{
	return false;
}

/*virtual*/ bool JoyStickController::ButtonUp( int button )
{
	return false;
}

/*virtual*/ bool JoyStickController::ButtonPressed( int button )
{
	return false;
}

/*virtual*/ void JoyStickController::GetAnalogTrigger( int side, double& value )
{
}

/*virtual*/ void JoyStickController::GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag )
{
}

// Controller.cpp