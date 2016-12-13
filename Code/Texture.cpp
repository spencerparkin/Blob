// Texture.cpp

#include "Texture.h"
#include <wx/filename.h>

Texture::Texture( void )
{
	texName = GL_INVALID_VALUE;
	image = nullptr;
	texData = nullptr;
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
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );

	const GLubyte* imageData = image->GetData();

	GLuint componentsPerPixel = 3;
	GLuint imageDataSize = image->GetWidth() * image->GetHeight() * sizeof( GLubyte ) * componentsPerPixel;
	texData = new GLubyte[ imageDataSize ];

	// Unfortunately, we have to flip the image.
	for( int i = 0; i < image->GetWidth(); i++ )
		for( int j = 0; j < image->GetHeight(); j++ )
			for( int k = 0; k < ( signed )componentsPerPixel; k++ )
				texData[ ( j * image->GetWidth() + i ) * componentsPerPixel + k ] = imageData[ ( ( image->GetHeight() - 1 - j ) * image->GetWidth() + i ) * componentsPerPixel + k ];

	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGB, image->GetWidth(), image->GetHeight(), 0, GL_RGB, GL_UNSIGNED_BYTE, texData );

	delete image;
	image = nullptr;

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

	if( texData )
	{
		delete[] texData;
		texData = nullptr;
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