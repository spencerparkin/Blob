// Race.h

#pragma once

#include <wx/string.h>
#include "Blob.h"
#include <TriangleMesh.h>
#include <Renderer.h>

class Race
{
public:

	Race( void );
	virtual ~Race( void );

	bool Load( const wxString& raceFile );
	bool Unload( void );

	void Render( _3DMath::Renderer& renderer );
	void Simulate( double currentTime );

private:

	void DeleteBlobList( void );

	BlobList blobList;

	_3DMath::TriangleMesh raceTrackMesh;
};

// Race.h