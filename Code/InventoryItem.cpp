// InventoryItem.cpp

#include "InventoryItem.h"
#include "Property.h"
#include "Blob.h"
#include "Message.h"
#include "Application.h"

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
	AddModifier* addModifier = new AddModifier( torqueBoost, torqueBoostTimeSeconds );
	blob->maxTorque.AddModifier( addModifier );
	wxGetApp().messageSystem->SendAMessage( new PropertyModifiedMessage( blob->GetHandle(), blob->maxTorque.GetHandle(), addModifier->GetHandle() ) );
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
	AddModifier* addModifier = new AddModifier( gravityBoost, gravityBoostTimeSeconds );
	blob->gravity.AddModifier( addModifier );
	wxGetApp().messageSystem->SendAMessage( new PropertyModifiedMessage( blob->GetHandle(), blob->gravity.GetHandle(), addModifier->GetHandle() ) );
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
	AddModifier* addModifier = new AddModifier( frictionBoost, frictionBoostTimeSeconds );
	blob->friction.AddModifier( addModifier );
	wxGetApp().messageSystem->SendAMessage( new PropertyModifiedMessage( blob->GetHandle(), blob->friction.GetHandle(), addModifier->GetHandle() ) );
}

/*virtual*/ _3DMath::HandleObject* FrictionBoosterInventoryItem::Clone( void ) const
{
	FrictionBoosterInventoryItem* item = new FrictionBoosterInventoryItem();
	item->frictionBoost = frictionBoost;
	item->frictionBoostTimeSeconds = frictionBoostTimeSeconds;
	return item;
}

// InventoryItem.cpp