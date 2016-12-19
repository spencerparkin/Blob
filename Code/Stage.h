// Stage.h

#pragma once

#include <wx/string.h>
#include "Blob.h"
#include <TriangleMesh.h>
#include <Renderer.h>
#include <BoundingBoxTree.h>
#include <TimeKeeper.h>
#include <Plane.h>

class Texture;

class Stage
{
public:

	Stage( void );
	virtual ~Stage( void );

	bool Load( const wxString& stageFile );
	bool Unload( void );

	void Render( _3DMath::Renderer& renderer );
	void Simulate( const _3DMath::TimeKeeper& timeKeeper );

private:

	void DeleteBlobList( void );

	BlobList blobList;
	Texture* texture;
	_3DMath::Vector respawnLocation;
	_3DMath::Plane deathPlane;
	_3DMath::TriangleMesh mesh;
	_3DMath::BoundingBoxTree* boxTree;
	// TODO: Own list of orbs here.
};

// Stage.h