// Race.cpp

#include "Race.h"

Race::Race( void )
{
}

/*virtual*/ Race::~Race( void )
{
	DeleteBlobList();
}

void Race::DeleteBlobList( void )
{
	while( blobList.size() > 0 )
	{
		BlobList::iterator iter = blobList.begin();
		Blob* blob = *iter;
		delete blob;
		blobList.erase( iter );
	}
}

bool Race::Load( const wxString& raceFile )
{
	return false;
}

bool Race::Unload( void )
{
	return false;
}

void Race::Render( _3DMath::Renderer& renderer )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
	}
}

void Race::Simulate( double currentTime )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Simulate( currentTime );
	}
}

// Race.cpp