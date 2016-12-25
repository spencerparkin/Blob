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

class ModelInventoryItem : public InventoryItem
{
public:

	ModelInventoryItem( void );
	virtual ~ModelInventoryItem( void );

	virtual std::string GetModelName( void ) const = 0;

	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const override;

	mutable double rotationAngle;
	mutable double rotationRate;
	mutable _3DMath::Vector color;
};

class TorqueBoosterInventoryItem : public ModelInventoryItem
{
public:

	TorqueBoosterInventoryItem( void );
	virtual ~TorqueBoosterInventoryItem( void );

	virtual std::string GetModelName( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double torqueBoost;
	double torqueBoostTimeSeconds;
};

class GravityBoosterInventoryItem : public ModelInventoryItem
{
public:

	GravityBoosterInventoryItem( void );
	virtual ~GravityBoosterInventoryItem( void );

	virtual std::string GetModelName( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double gravityBoost;
	double gravityBoostTimeSeconds;
};

class FrictionBoosterInventoryItem : public ModelInventoryItem
{
public:

	FrictionBoosterInventoryItem( void );
	virtual ~FrictionBoosterInventoryItem( void );

	virtual std::string GetModelName( void ) const override;
	virtual void Use( Blob* blob ) override;
	virtual _3DMath::HandleObject* Clone( void ) const override;

	double frictionBoost;
	double frictionBoostTimeSeconds;
};

// InventoryItem.h