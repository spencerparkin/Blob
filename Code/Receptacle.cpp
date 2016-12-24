// Receptacle.cpp

#include "Receptacle.h"
#include "InventoryItem.h"
#include "Message.h"
#include "Application.h"

//------------------------------------------------------------------------------------------
//                                       Receptacle
//------------------------------------------------------------------------------------------

Receptacle::Receptacle( void )
{
}

/*virtual*/ Receptacle::~Receptacle( void )
{
}

/*virtual*/ void Receptacle::Simulate( const _3DMath::TimeKeeper& timeKeeper, BlobList& blobList )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;

		_3DMath::Vector location;
		blob->GetLocation( location );

		BlobSet::iterator setIter = occupantSet.find( blob );

		if( boundingVolume.ContainsPoint( location ) )
		{
			if( setIter == occupantSet.end() )
			{
				occupantSet.insert( blob );
				blob->occupiedReceptacleHandle = GetHandle();
				EnterBlob( blob );
			}

			OccupiedByBlob( blob );
		}
		else
		{
			if( setIter != occupantSet.end() )
			{
				ExitBlob( blob );
				blob->occupiedReceptacleHandle = 0;
				occupantSet.erase( blob );
			}
		}
	}
}

/*virtual*/ void Receptacle::EnterBlob( Blob* blob )
{
}

/*virtual*/ void Receptacle::ExitBlob( Blob* blob )
{
}

/*virtual*/ void Receptacle::OccupiedByBlob( Blob* blob )
{
}

/*virtual*/ void Receptacle::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
}

//------------------------------------------------------------------------------------------
//                                   StageCompleteReceptacle
//------------------------------------------------------------------------------------------

StageCompleteReceptacle::StageCompleteReceptacle( void )
{
}

/*virtual*/ StageCompleteReceptacle::~StageCompleteReceptacle( void )
{
}

/*virtual*/ void StageCompleteReceptacle::EnterBlob( Blob* blob )
{
	// TODO: Send only if the given blob has all needed orbs in its inventory?
	wxGetApp().messageSystem->SendAMessage( new StageCompleteMessage() );
}

/*virtual*/ void StageCompleteReceptacle::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
}

//------------------------------------------------------------------------------------------
//                                   InventoryItemReceptacle
//------------------------------------------------------------------------------------------

InventoryItemReceptacle::InventoryItemReceptacle( void )
{
	templateItem = nullptr;
	inventoryItem = nullptr;
	regenerationRemainingTimeSeconds = 0.0;
	regenerationRemainingResetTimeSeconds = 0.0;
}

/*virtual*/ InventoryItemReceptacle::~InventoryItemReceptacle( void )
{
	delete inventoryItem;
	delete templateItem;
}

/*virtual*/ void InventoryItemReceptacle::Simulate( const _3DMath::TimeKeeper& timeKeeper, BlobList& blobList )
{
	if( !inventoryItem && templateItem )
	{
		regenerationRemainingTimeSeconds -= timeKeeper.GetDeltaTimeSeconds();
		if( regenerationRemainingTimeSeconds <= 0.0 )
		{
			inventoryItem = ( InventoryItem* )templateItem->Clone();
		}
	}

	Receptacle::Simulate( timeKeeper, blobList );
}

/*virtual*/ void InventoryItemReceptacle::EnterBlob( Blob* blob )
{
	if( blob && blob->inventoryItemList && inventoryItem )
	{
		blob->inventoryItemList->push_back( inventoryItem );
		inventoryItem = nullptr;
		regenerationRemainingTimeSeconds = regenerationRemainingResetTimeSeconds;
	}
}

/*virtual*/ void InventoryItemReceptacle::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
	// TODO: Forgo rendering if our bounding box is not visible?

	_3DMath::AffineTransform transform;
	boundingVolume.GetCenter( transform.translation );

	if( inventoryItem )
		inventoryItem->Render( renderer, timeKeeper, transform );
}

//------------------------------------------------------------------------------------------
//                                   TeleportReceptacle
//------------------------------------------------------------------------------------------

TeleportReceptacle::TeleportReceptacle( void )
{
	targetLocation.Set( 0.0, 0.0, 0.0 );
}

/*virtual*/ TeleportReceptacle::~TeleportReceptacle( void )
{
}

/*virtual*/ void TeleportReceptacle::EnterBlob( Blob* blob )
{
}

/*virtual*/ void TeleportReceptacle::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
}

//------------------------------------------------------------------------------------------
//                                 ChangeShapeReceptacle
//------------------------------------------------------------------------------------------

ChangeShapeReceptacle::ChangeShapeReceptacle( void )
{
}

/*virtual*/ ChangeShapeReceptacle::~ChangeShapeReceptacle( void )
{
}

/*virtual*/ void ChangeShapeReceptacle::EnterBlob( Blob* blob )
{
}

/*virtual*/ void ChangeShapeReceptacle::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
}

// Receptacle.cpp