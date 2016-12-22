// Sound.h

#pragma once

#include <wx/string.h>
#include <SFML/Audio.hpp>

class Sound
{
public:

	Sound( void );
	virtual ~Sound( void );

	bool IsEnabled( void ) { return enabled; }
	void SetEnabled( bool enabled ) { this->enabled = enabled; }

	bool PlayMusic( const wxString& musicFile );

private:

	sf::SoundBuffer soundBuffer;		// Cache these?
	sf::Sound sound;
	sf::Music music;

	bool enabled;
};

// Sound.h