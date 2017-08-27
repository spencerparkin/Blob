// Stage.cpp

#include "Stage.h"
#include "Texture.h"
#include "Blob.h"
#include "HumanDriver.h"
#include "ComputerDriver.h"
#include "Application.h"
#include "Camera.h"
#include "Frame.h"
#include "Receptacle.h"
#include <ListFunctions.h>
#include <wx/xml/xml.h>
#include <wx/wfstream.h>

Stage::Stage( void )
{
}

/*virtual*/ Stage::~Stage( void )
{
	( void )Unload();
}

bool Stage::Load( const wxString& stageFile )
{
	if( !Unload() )
		return false;

	wxGetApp().modelCache->Clear();

	wxFileInputStream stageFileStream( stageFile );
	if( !stageFileStream.IsOk() )
		return false;

	wxXmlDocument xmlDoc( stageFileStream );
	if( !xmlDoc.IsOk() )
		return false;

	wxXmlNode* xmlRootNode = xmlDoc.GetRoot();
	if( xmlRootNode->GetName() != "Stage" )
		return false;

	for( wxXmlNode* xmlNode = xmlRootNode->GetChildren(); xmlNode; xmlNode = xmlNode->GetNext() )
	{
		if( xmlNode->GetName() == "Receptacle" )
		{
			Receptacle* receptacle = nullptr;

			wxString attribStr = xmlNode->GetAttribute( "type" );
			if( attribStr == "StageComplete" )
			{
				receptacle = new StageCompleteReceptacle();
			}
			else if( attribStr == "TorqueBooster" || attribStr == "FrictionBooster" || attribStr == "GravityBooster" )
			{
				wxString inventoryType = attribStr;

				InventoryItemReceptacle* inventoryItemReceptacle = new InventoryItemReceptacle();
				receptacle = inventoryItemReceptacle;

				double boostValue;
				attribStr = xmlNode->GetAttribute( "boostValue" );
				attribStr.ToCDouble( &boostValue );

				double boostTime;
				attribStr = xmlNode->GetAttribute( "boostTime" );
				attribStr.ToCDouble( &boostTime );

				double regenTime;
				attribStr = xmlNode->GetAttribute( "regenTime" );
				attribStr.ToCDouble( &regenTime );

				inventoryItemReceptacle->regenerationRemainingResetTimeSeconds = regenTime;

				if( inventoryType == "TorqueBooster" )
				{
					TorqueBoosterInventoryItem* torqueBoosterInventoryItem = new TorqueBoosterInventoryItem();
					torqueBoosterInventoryItem->torqueBoost = boostValue;
					torqueBoosterInventoryItem->torqueBoostTimeSeconds = boostTime;
					inventoryItemReceptacle->templateItem = torqueBoosterInventoryItem;
				}
				else if( inventoryType == "FrictionBooster" )
				{
					FrictionBoosterInventoryItem* frictionBoosterInventoryItem = new FrictionBoosterInventoryItem();
					frictionBoosterInventoryItem->frictionBoost = boostValue;
					frictionBoosterInventoryItem->frictionBoostTimeSeconds = boostTime;
					inventoryItemReceptacle->templateItem = frictionBoosterInventoryItem;
				}
				else if( inventoryType == "GravityBooster" )
				{
					GravityBoosterInventoryItem* gravityBoosterInventoryItem = new GravityBoosterInventoryItem();
					gravityBoosterInventoryItem->gravityBoost = boostValue;
					gravityBoosterInventoryItem->gravityBoostTimeSeconds = boostTime;
					inventoryItemReceptacle->templateItem = gravityBoosterInventoryItem;
				}
			}
			
			if( receptacle )
			{
				_3DMath::Vector center, dimensions;

				attribStr = xmlNode->GetAttribute( "x" );
				attribStr.ToCDouble( &center.x );
				attribStr = xmlNode->GetAttribute( "y" );
				attribStr.ToCDouble( &center.y );
				attribStr = xmlNode->GetAttribute( "z" );
				attribStr.ToCDouble( &center.z );

				attribStr = xmlNode->GetAttribute( "w" );
				attribStr.ToCDouble( &dimensions.x );
				attribStr = xmlNode->GetAttribute( "h" );
				attribStr.ToCDouble( &dimensions.y );
				attribStr = xmlNode->GetAttribute( "d" );
				attribStr.ToCDouble( &dimensions.z );

				receptacle->boundingVolume.SetCenterAndDimensions( center, dimensions );

				receptacleList.push_back( receptacle );
			}
		}
		else if( xmlNode->GetName() == "DeathPlane" )
		{
			_3DMath::Vector center( 0.0, 0.0, 0.0 );

			wxString attribStr = xmlNode->GetAttribute( "y" );
			if( !attribStr.ToCDouble( &center.y ) )
				return false;

			deathPlane.SetCenterAndNormal( center, _3DMath::Vector( 0.0, 1.0, 0.0 ) );
		}
		else if( xmlNode->GetName() == "Ground" )
		{
			wxString modelName = xmlNode->GetAttribute( "model" );
			if( modelName.IsEmpty() )
				return false;

			GroundModel* model = wxGetApp().modelCache->GetModel< GroundModel >( ( const char* )modelName.c_str() );
			if( !model )
				return false;

			groundList.push_back( ( const char* )modelName.c_str() );
		}
		else if( xmlNode->GetName() == "Blob" )
		{
			Blob* blob = new Blob();
			blobList.push_back( blob );

			wxString attribStr = xmlNode->GetAttribute( "scale" );
			double scale = 1.0;
			attribStr.ToCDouble( &scale );

			attribStr = xmlNode->GetAttribute( "x" );
			attribStr.ToCDouble( &respawnLocation.x );
			attribStr = xmlNode->GetAttribute( "y" );
			attribStr.ToCDouble( &respawnLocation.y );
			attribStr = xmlNode->GetAttribute( "z" );
			attribStr.ToCDouble( &respawnLocation.z );

			_3DMath::AffineTransform affineTransform;
			affineTransform.linearTransform.SetScale( scale );
			affineTransform.translation = respawnLocation;

			bool subDivide = false;
			attribStr = xmlNode->GetAttribute( "subDivide" );
			if( attribStr == "1" )
				subDivide = true;

			attribStr = xmlNode->GetAttribute( "type" );
			if( attribStr == "icosahedron" )
				blob->MakePolyhedron( Blob::ICOSAHEDRON, subDivide, affineTransform );
			else if( attribStr == "dodecahedron" )
				blob->MakePolyhedron( Blob::DODECAHEDRON, subDivide, affineTransform );
			else if( attribStr == "hexadron" )
				blob->MakePolyhedron( Blob::HEXADRON, subDivide, affineTransform );

			attribStr = xmlNode->GetAttribute( "driver" );
			if( attribStr == "human" )
			{
				blob->driver = new HumanDriver();
				Camera* camera = wxGetApp().GetCamera();
				camera->subject = blob;
				camera->mode = Camera::MODE_FOLLOW_BEHIND_SUBJECT;
			}
			else if( attribStr == "computer" )
				blob->driver = new ComputerDriver();
		}
	}

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;

		for( ModelNameList::iterator groundIter = groundList.begin(); groundIter != groundList.end(); groundIter++ )
		{
			GroundModel* groundModel = wxGetApp().modelCache->GetModel< GroundModel >( *groundIter );

			blob->RegisterGroundCollisionObject( groundModel->boxTree, 1.0 );
		}
	}

	// TODO: Load spline data from XML.  This helps AI blobs blob-around the stage.

	return true;
}

bool Stage::Unload( void )
{
	_3DMath::FreeList< Blob >( blobList );
	_3DMath::FreeList< Receptacle >( receptacleList );

	groundList.clear();

	return true;
}

void Stage::Render( _3DMath::Renderer& renderer, const _3DMath::TimeKeeper& timeKeeper )
{
	// TODO: We need a sky-dome that shows stars and the milky way that rotates slowly in random directions.
	// TODO: Shadows by rendering from light-source perspective into off-screen depth map and then using that in a shader?

	for( ModelNameList::iterator iter = groundList.begin(); iter != groundList.end(); iter++ )
	{
		GroundModel* model = wxGetApp().modelCache->GetModel< GroundModel >( *iter );
		if( model )
		{
			// TODO: Only render if mesh at least partially visible in frustum?
			model->Render( renderer );
		}
	}

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
	}

	for( ReceptacleList::iterator iter = receptacleList.begin(); iter != receptacleList.end(); iter++ )
	{
		Receptacle* receptacle = *iter;
		receptacle->Render( renderer, timeKeeper );
	}
}

void Stage::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Simulate( timeKeeper );

		_3DMath::Vector location;
		blob->GetLocation( location );

		// It doesn't happen too often, but if the simulation blows up, try to detect that and recover from it.
		if( location.x != location.x || location.y != location.y || location.z != location.z )
		{
			// TODO: Do this how the ChangeShapeReceptacle class does it, I presume.
		}

		if( deathPlane.GetSide( location ) == _3DMath::Plane::SIDE_BACK )
		{
			_3DMath::AffineTransform transform;
			transform.translation.Subtract( respawnLocation, location );
			blob->Teleport( transform );
			blob->WipeInventory( true );
		}
	}

	for( ReceptacleList::iterator iter = receptacleList.begin(); iter != receptacleList.end(); iter++ )
	{
		Receptacle* receptacle = *iter;
		receptacle->Simulate( timeKeeper, blobList );
	}
}

// Stage.cpp