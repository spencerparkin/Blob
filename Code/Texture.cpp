// Texture.cpp

#include "Texture.h"
#include <wx/filename.h>

Texture::Texture( void )
{
	texName = GL_INVALID_VALUE;
	image = nullptr;
}

/*virtual*/ Texture::~Texture( void )
{
	( void )Unload();
}

bool Texture::Load( const wxString& texFile )
{
	( void )Unload();

	wxString texFilePath = wxFileName::GetCwd() + "/" + texFile;

	image = new wxImage();
	if( !image->LoadFile( texFilePath, wxBITMAP_TYPE_ANY ) )
		return false;

	glGenTextures( 1, &texName );
	if( texName == GL_INVALID_VALUE )
		return false;

	glBindTexture( GL_TEXTURE_2D, texName );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	const unsigned char* imageData = image->GetData();
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, imageData );

	return true;
}

bool Texture::Unload( void )
{
	if( texName != GL_INVALID_VALUE )
	{
		glDeleteTextures( 1, &texName );
		texName = GL_INVALID_VALUE;
	}

	if( image )
	{
		delete image;
		image = nullptr;
	}

	return true;
}

bool Texture::Bind( void )
{
	if( texName == GL_INVALID_VALUE )
		return false;

	glEnable( GL_TEXTURE_2D );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL );
	glBindTexture( GL_TEXTURE_2D, texName );
	return true;
}

// Texture.cpp