// Receptacle.h

#pragma once

#include "Blob.h"
#include <AxisAlignedBox.h>
#include <TimeKeeper.h>

class Blob;

// These are locations that may or may not contain an inventory item that,
// when occupied by the blob, transfer ownership of the said item (if applicable),
// change the blob in some other way, or perhaps signal the completion of a stage.
// For example, one receptacle may contains a key, while another requires a key to
// unlock something.  There are an unimaginable number of applications; I'm not sure
// how far I'll get with any of them.
class Receptacle
{
public:

	Receptacle( void );
	virtual ~Receptacle( void );

	virtual void Simulate( const _3DMath::TimeKeeper& timeKeeper, BlobList& blobList );
	virtual void EnterBlob( Blob* blob ) = 0;
	virtual void ExitBlob( Blob* blob ) = 0;
	virtual void OccupiedByBlob( Blob* blob ) = 0;	// This is called every frame the given blob occupies the volume.
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper ) = 0;

	_3DMath::AxisAlignedBox voundingVolume;
};

// Receptacle.h
