// Controller.cpp

#include "Controller.h"
#include <wx/joystick.h>
#include <wx/utils.h>

//----------------------------------------------------------------------------------------------
//                                        Controller
//----------------------------------------------------------------------------------------------

Controller::Controller( void )
{
}

/*virtual*/ Controller::~Controller( void )
{
}

/*virtual*/ bool Controller::ButtonUp( int button )
{
	return !ButtonDown( button );
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

	for( userIndex = 0; userIndex < 4 /*XUSER_MAX_COUNT*/; userIndex++ )
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
	for( int button = 0; button < BUTTON_COUNT; button++ )
	{
		unsigned int keyCode = MapButtonToKeyboardKey( button );

		KeyStateMap::iterator iter = keyStateMap.find( keyCode );
		if( iter == keyStateMap.end() )
		{
			keyStateMap.insert( std::pair< unsigned int, KeyState >( keyCode, KeyState() ) );
			iter = keyStateMap.find( keyCode );
			iter->second.down = false;
			iter->second.pressed = false;
		}
		
		KeyState* keyState = &iter->second;

		bool keyDown = wxGetKeyState( ( wxKeyCode )keyCode );
		keyState->pressed = !keyState->down && keyDown;
		keyState->down = keyDown;
	}

	return true;
}

unsigned int KeyboardMouseController::MapButtonToKeyboardKey( int button )
{
	switch( button )
	{
		case BUTTON_A:				return 'a';
		case BUTTON_B:				return 'b';
		case BUTTON_X:				return 'x';
		case BUTTON_Y:				return 'y';
		case BUTTON_DPAD_UP:		return ( unsigned int )WXK_HOME;
		case BUTTON_DPAD_DN:		return ( unsigned int )WXK_END;
		case BUTTON_DPAD_LF:		return ( unsigned int )WXK_DELETE;
		case BUTTON_DPAD_RT:		return ( unsigned int )WXK_PAGEDOWN;
		case BUTTON_L_SHOULDER:		return ( unsigned int )WXK_NUMPAD7;
		case BUTTON_R_SHOULDER:		return ( unsigned int )WXK_NUMPAD9;
		case BUTTON_BACK:			return 'b';
		case BUTTON_START:			return 's';
		case BUTTON_L_THUMB:		return 'l';
		case BUTTON_R_THUMB:		return 'r';
	}

	return 0;
}

/*virtual*/ bool KeyboardMouseController::ButtonDown( int button )
{
	KeyState* keyState = nullptr;
	if( !GetKeyState( button, keyState ) )
		return false;
	return keyState->down;
}

/*virtual*/ bool KeyboardMouseController::ButtonPressed( int button )
{
	KeyState* keyState = nullptr;
	if( !GetKeyState( button, keyState ) )
		return false;
	return keyState->pressed;
}

bool KeyboardMouseController::GetKeyState( int button, KeyState*& keyState )
{
	keyState = nullptr;
	unsigned int keyCode = MapButtonToKeyboardKey( button );
	KeyStateMap::iterator iter = keyStateMap.find( keyCode );
	if( iter == keyStateMap.end() )
		return false;
	keyState = &iter->second;
	return true;
}

/*virtual*/ void KeyboardMouseController::GetAnalogTrigger( int side, double& value )
{
	bool keyDown = false;
	if( side == LEFT_SIDE )
		keyDown = wxGetKeyState( WXK_NUMPAD1 );
	else if( side == RIGHT_SIDE )
		keyDown = wxGetKeyState( WXK_NUMPAD3 );
	value = keyDown ? 1.0 : 0.0;
}

/*virtual*/ void KeyboardMouseController::GetAnalogJoyStick( int side, _3DMath::Vector& unitDir, double& mag )
{
	unitDir.Set( 0.0, 0.0, 0.0 );

	_3DMath::Vector xAxis( 1.0, 0.0, 0.0 );
	_3DMath::Vector yAxis( 0.0, 1.0, 0.0 );

	bool controlDown = wxGetKeyState( WXK_CONTROL );

	if( ( side == LEFT_SIDE && !controlDown ) || ( side == RIGHT_SIDE && controlDown ) )
	{
		if( wxGetKeyState( WXK_UP ) )
			unitDir.Add( yAxis );
		if( wxGetKeyState( WXK_DOWN ) )
			unitDir.Subtract( yAxis );
		if( wxGetKeyState( WXK_RIGHT ) )
			unitDir.Add( xAxis );
		if( wxGetKeyState( WXK_LEFT ) )
			unitDir.Subtract( xAxis );
	}

	mag = unitDir.Length();
	if( mag != 0.0 )
	{
		unitDir.Scale( 1.0 / mag );
		mag = 1.0;
	}
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