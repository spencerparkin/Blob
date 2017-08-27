// InventoryItem.cpp

#include "InventoryItem.h"
#include "Property.h"
#include "Blob.h"
#include "Message.h"
#include "Application.h"
#include "Camera.h"
#include "Application.h"
#include "ModelCache.h"
#include <wx/glcanvas.h>

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
//                                      ModelInventoryItem
//------------------------------------------------------------------------------------------

ModelInventoryItem::ModelInventoryItem( void )
{
	color.Set( 0.5, 0.5, 0.5 );
	rotationRate = M_PI / 2.0;
	rotationAngle = 0.0;
}

/*virtual*/ ModelInventoryItem::~ModelInventoryItem( void )
{
}

/*virtual*/ void ModelInventoryItem::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const
{
	Model* model = wxGetApp().modelCache->GetModel< Model >( GetModelName() );
	if( model )
	{
		Camera* camera = wxGetApp().GetCamera();

		glDisable( GL_TEXTURE_2D );
		glColor3d( color.x, color.y, color.z );

		rotationAngle += rotationRate * timeKeeper.GetDeltaTimeSeconds();
		if( rotationAngle > 2.0 * M_PI )
			rotationAngle -= 2.0 * M_PI;

		_3DMath::AffineTransform rotation;
		rotation.linearTransform.SetRotation( _3DMath::Vector( 0.0, 1.0, 0.0 ), rotationAngle );

		_3DMath::AffineTransform modelTransform;
		modelTransform.Concatinate( rotation, transform );

		model->Render( renderer, &modelTransform );
	}
}

//------------------------------------------------------------------------------------------
//                                TorqueBoosterInventoryItem
//------------------------------------------------------------------------------------------

TorqueBoosterInventoryItem::TorqueBoosterInventoryItem( void )
{
	torqueBoost = 0;
	torqueBoostTimeSeconds = 0;

	color.Set( 0.0, 1.0, 0.0 );
}

/*virtual*/ TorqueBoosterInventoryItem::~TorqueBoosterInventoryItem( void )
{
}

/*virtual*/ std::string TorqueBoosterInventoryItem::GetModelName( void ) const
{
	return "TorqueBoost";
}

/*virtual*/ std::string TorqueBoosterInventoryItem::GetDisplayName( void ) const
{
	return "Torque Boost";
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

/*virtual*/ std::string GravityBoosterInventoryItem::GetModelName( void ) const
{
	return "GravityBoost";
}

/*virtual*/ std::string GravityBoosterInventoryItem::GetDisplayName( void ) const
{
	return "Gravity Boost";
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

/*virtual*/ std::string FrictionBoosterInventoryItem::GetModelName( void ) const
{
	return "FrictionBoost";
}

/*virtual*/ std::string FrictionBoosterInventoryItem::GetDisplayName( void ) const
{
	return "Friction Boost";
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