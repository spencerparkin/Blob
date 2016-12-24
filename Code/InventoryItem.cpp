// InventoryItem.cpp

#include "InventoryItem.h"
#include "Property.h"
#include "Blob.h"
#include "Message.h"
#include "Application.h"
#include "Camera.h"
#include "Application.h"
#include <FileFormat.h>
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
//                                 BspTreeInventoryItem
//------------------------------------------------------------------------------------------

BspTreeInventoryItem::BspTreeInventoryItem( void )
{
	alpha = 1.0;
	color.Set( 0.5, 0.5, 0.5 );
	rotationRate = M_PI / 2.0;
	rotationAngle = 0.0;
	bspTree = nullptr;
}

/*virtual*/ BspTreeInventoryItem::~BspTreeInventoryItem( void )
{
	delete bspTree;
}

/*virtual*/ void BspTreeInventoryItem::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper, const _3DMath::AffineTransform& transform ) const
{
	if( !bspTree )
	{
		// TODO: Come up with a caching mechanism here to reduce load times.
		//       We might need the reference-object mechanism so that instances
		//       can share a pointer to the same mesh.
		LoadBspTree( GetMeshFile() );
	}

	if( bspTree )
	{
		Camera* camera = wxGetApp().GetCamera();

		glDisable( GL_TEXTURE_2D );

		glColor4d( color.x, color.y, color.z, alpha );

		rotationAngle += rotationRate * timeKeeper.GetDeltaTimeSeconds();
		if( rotationAngle > 2.0 * M_PI )
			rotationAngle -= 2.0 * M_PI;

		_3DMath::AffineTransform rotation;
		rotation.linearTransform.SetRotation( _3DMath::Vector( 0.0, 1.0, 0.0 ), rotationAngle );

		_3DMath::AffineTransform bspTreeTransform;
		bspTreeTransform.Concatinate( rotation, transform );

		// TODO: Enable alpha-blending, then disable it afterword.

		bspTree->Render( renderer, _3DMath::BspTree::RENDER_BACK_TO_FRONT, camera->viewTransform.translation, &bspTreeTransform, _3DMath::Renderer::VTX_FLAG_POSITION | _3DMath::Renderer::VTX_FLAG_NORMAL );
	}
}

bool BspTreeInventoryItem::LoadBspTree( const wxString& inventoryItemMeshFile ) const
{
	bool success = false;

	_3DMath::FileFormat* fileFormat = nullptr;
	_3DMath::TriangleMesh mesh;

	do
	{
		if( bspTree )
			break;

		fileFormat = _3DMath::FileFormat::CreateForFile( ( const char* )inventoryItemMeshFile.c_str() );
		if( !fileFormat )
			break;

		if( !fileFormat->LoadTriangleMesh( mesh, ( const char* )inventoryItemMeshFile.c_str() ) )
			break;

		bspTree = new _3DMath::BspTree();

		if( !bspTree->Generate( mesh ) )
			break;

		success = true;
	}
	while( false );

	delete fileFormat;

	return success;
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

/*virtual*/ wxString TorqueBoosterInventoryItem::GetMeshFile( void ) const
{
	return "Data/TorqueBoost.obj";
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

/*virtual*/ wxString GravityBoosterInventoryItem::GetMeshFile( void ) const
{
	return "Data/GravityBoost.obj";
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

/*virtual*/ wxString FrictionBoosterInventoryItem::GetMeshFile( void ) const
{
	return "Data/FrictionBoost.obj";
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