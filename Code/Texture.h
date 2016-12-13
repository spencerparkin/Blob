// Texture.h

#pragma once

#include <wx/image.h>
#include <wx/glcanvas.h>

class Texture
{
public:

	Texture( void );
	virtual ~Texture( void );

	bool Load( const wxString& texFile );
	bool Unload( void );
	bool Bind( void );

private:

	GLuint texName;
	wxImage* image;
	GLubyte* texData;
};

// Texture.h