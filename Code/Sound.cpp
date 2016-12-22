// Sound.cpp

#include "Sound.h"

Sound::Sound( void )
{
	enabled = false;
}

/*virtual*/ Sound::~Sound( void )
{
}

bool Sound::PlayMusic( const wxString& musicFile )
{
	if( !music.openFromFile( ( const char* )musicFile.c_str() ) )
		return false;

	music.play();
	return true;
}

// Sound.h