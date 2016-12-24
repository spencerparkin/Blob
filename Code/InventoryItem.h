// InventoryItem.h

#pragma once

#include <HandleObject.h>
#include <TimeKeeper.h>
#include <AffineTransform.h>
#include <TriangleMesh.h>
#include <BspTree.h>
#include <wx/string.h>

class Blob;

class InventoryItem : public _3DMath::HandleObject
{
public:

	InventoryItem( void );
	virtual ~InventoryItem( void );

	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const = 0;
	virtual void Use( Blob* blob ) = 0;
};

typedef InventoryItem* ( *InventoryItemCreateFunc )( void );

typedef std::list< InventoryItem* > InventoryItemList;

class BspTreeInventoryItem : public InventoryItem
{
public:

	BspTreeInventoryItem( void );
	virtual ~BspTreeInventoryItem( void );

	virtual wxString GetMeshFile( void ) const = 0;

	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const override;

	bool LoadBspTree( const wxString& inventoryItemMeshFile ) const;

	mutable double rotationAngle;
	mutable double rotationRate;
	mutable double alpha;
	mutable _3DMath::Vector color;
	mutable _3DMath::BspTree* bspTree;
};

class TorqueBoosterInventoryItem : public BspTreeInventoryItem
{
public:

	TorqueBoosterInventoryItem( void );
	virtual ~TorqueBoosterInventoryItem( void );

	virtual wxString GetMeshFile( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double torqueBoost;
	double torqueBoostTimeSeconds;
};

class GravityBoosterInventoryItem : public BspTreeInventoryItem
{
public:

	GravityBoosterInventoryItem( void );
	virtual ~GravityBoosterInventoryItem( void );

	virtual wxString GetMeshFile( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double gravityBoost;
	double gravityBoostTimeSeconds;
};

class FrictionBoosterInventoryItem : public BspTreeInventoryItem
{
public:

	FrictionBoosterInventoryItem( void );
	virtual ~FrictionBoosterInventoryItem( void );

	virtual wxString GetMeshFile( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double frictionBoost;
	double frictionBoostTimeSeconds;
};

// InventoryItem.h