// Stage.h

#pragma once

#include <wx/string.h>
#include "Blob.h"
#include "ModelCache.h"
#include "Receptacle.h"
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

	void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper );
	void Simulate( const _3DMath::TimeKeeper& timeKeeper );

private:

	BlobList blobList;
	ReceptacleList receptacleList;
	_3DMath::Vector respawnLocation;
	_3DMath::Plane deathPlane;
	ModelNameList groundList;
};

// Stage.h