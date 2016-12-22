// Receptacle.h

#pragma once

#include "Blob.h"
#include <AxisAlignedBox.h>
#include <TimeKeeper.h>
#include <HandleObject.h>

class Blob;
class InventoryItem;

// These are locations that may or may not contain an inventory item that,
// when occupied by the blob, transfer ownership of the said item (if applicable),
// change the blob in some other way, or perhaps signal the completion of a stage.
// For example, one receptacle may contain a key, while another requires a key to
// unlock something.  There are an unimaginable number of applications; I'm not sure
// how far I'll get with any of them.
class Receptacle : _3DMath::HandleObject
{
public:

	Receptacle( void );
	virtual ~Receptacle( void );

	virtual void Simulate( const _3DMath::TimeKeeper& timeKeeper, BlobList& blobList );
	virtual void EnterBlob( Blob* blob );
	virtual void ExitBlob( Blob* blob );
	virtual void OccupiedByBlob( Blob* blob );	// This is called every frame the given blob occupies the volume.
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper );

	_3DMath::AxisAlignedBox boundingVolume;
	BlobSet occupantSet;
};

typedef std::list< Receptacle* > ReceptacleList;

class StageCompleteReceptacle : public Receptacle
{
public:

	StageCompleteReceptacle( void );
	virtual ~StageCompleteReceptacle( void );

	virtual void EnterBlob( Blob* blob ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper ) override;
};

class InventoryItemReceptacle : public Receptacle
{
public:

	InventoryItemReceptacle( void );
	virtual ~InventoryItemReceptacle( void );

	virtual void Simulate( const _3DMath::TimeKeeper& timeKeeper, BlobList& blobList ) override;
	virtual void EnterBlob( Blob* blob ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper ) override;

	InventoryItem* templateItem;
	InventoryItem* inventoryItem;
	double regenerationRemainingTimeSeconds;
	double regenerationRemainingResetTimeSeconds;
};

class TeleportReceptacle : public Receptacle
{
public:

	TeleportReceptacle( void );
	virtual ~TeleportReceptacle( void );

	virtual void EnterBlob( Blob* blob ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper ) override;

	// Multiple locations, condition/unconditional teleportation?
	_3DMath::Vector targetLocation;
};

class ChangeShapeReceptacle : public Receptacle
{
public:

	ChangeShapeReceptacle( void );
	virtual ~ChangeShapeReceptacle( void );

	virtual void EnterBlob( Blob* blob ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper ) override;

	//...
};

// Receptacle.h
