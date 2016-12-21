// InventoryItem.cpp

#include "InventoryItem.h"
#include "Property.h"
#include "Blob.h"

//------------------------------------------------------------------------------------------
//                                      InventoryItem
//------------------------------------------------------------------------------------------

InventoryItem::InventoryItem( void )
{
}

/*virtual*/ InventoryItem::~InventoryItem( void )
{
}

//------------------------------------------------------------------------------------------
//                                TorqueBoosterInventoryItem
//------------------------------------------------------------------------------------------

TorqueBoosterInventoryItem::TorqueBoosterInventoryItem( void )
{
	torqueBoost = 0;
	torqueBoostTimeSeconds = 0;
}

/*virtual*/ TorqueBoosterInventoryItem::~TorqueBoosterInventoryItem( void )
{
}

/*virtual*/ void TorqueBoosterInventoryItem::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const
{
}

/*virtual*/ void TorqueBoosterInventoryItem::Use( Blob* blob )
{
	blob->maxTorque.AddModifier( new AddModifier( torqueBoost, torqueBoostTimeSeconds ) );
}

/*virtual*/ _3DMath::HandleObject* TorqueBoosterInventoryItem::Clone( void ) const
{
	TorqueBoosterInventoryItem* item = new TorqueBoosterInventoryItem();
	item->torqueBoost = torqueBoost;
	item->torqueBoostTimeSeconds = torqueBoostTimeSeconds;
	return item;
}

//------------------------------------------------------------------------------------------
//                                GravityBoosterInventoryItem
//------------------------------------------------------------------------------------------

GravityBoosterInventoryItem::GravityBoosterInventoryItem( void )
{
	gravityBoost = 0;
	gravityBoostTimeSeconds = 0;
}

/*virtual*/ GravityBoosterInventoryItem::~GravityBoosterInventoryItem( void )
{
}

/*virtual*/ void GravityBoosterInventoryItem::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const
{
}

/*virtual*/ void GravityBoosterInventoryItem::Use( Blob* blob )
{
	blob->gravity.AddModifier( new AddModifier( gravityBoost, gravityBoostTimeSeconds ) );
}

/*virtual*/ _3DMath::HandleObject* GravityBoosterInventoryItem::Clone( void ) const
{
	GravityBoosterInventoryItem* item = new GravityBoosterInventoryItem();
	item->gravityBoost = gravityBoost;
	item->gravityBoostTimeSeconds = gravityBoostTimeSeconds;
	return item;
}

//------------------------------------------------------------------------------------------
//                                FrictionBoosterInventoryItem
//------------------------------------------------------------------------------------------

FrictionBoosterInventoryItem::FrictionBoosterInventoryItem( void )
{
	frictionBoost = 0;
	frictionBoostTimeSeconds = 0;
}

/*virtual*/ FrictionBoosterInventoryItem::~FrictionBoosterInventoryItem( void )
{
}

/*virtual*/ void FrictionBoosterInventoryItem::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const
{
}

/*virtual*/ void FrictionBoosterInventoryItem::Use( Blob* blob )
{
	blob->friction.AddModifier( new AddModifier( frictionBoost, frictionBoostTimeSeconds ) );
}

/*virtual*/ _3DMath::HandleObject* FrictionBoosterInventoryItem::Clone( void ) const
{
	FrictionBoosterInventoryItem* item = new FrictionBoosterInventoryItem();
	item->frictionBoost = frictionBoost;
	item->frictionBoostTimeSeconds = frictionBoostTimeSeconds;
	return item;
}

// InventoryItem.cpp