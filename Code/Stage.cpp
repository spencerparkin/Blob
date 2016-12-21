// Stage.cpp

#include "Stage.h"
#include "Texture.h"
#include "Blob.h"
#include "HumanDriver.h"
#include "ComputerDriver.h"
#include "Application.h"
#include "Camera.h"
#include "Frame.h"
#include <FileFormat.h>
#include <wx/xml/xml.h>
#include <wx/wfstream.h>
#include <sstream>

Stage::Stage( void )
{
	texture = nullptr;
	boxTree = nullptr;
}

/*virtual*/ Stage::~Stage( void )
{
	( void )Unload();
}

void Stage::DeleteBlobList( void )
{
	while( blobList.size() > 0 )
	{
		BlobList::iterator iter = blobList.begin();
		Blob* blob = *iter;
		delete blob;
		blobList.erase( iter );
	}
}

bool Stage::Load( const wxString& stageFile )
{
	if( !Unload() )
		return false;

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
		if( xmlNode->GetName() == "DeathPlane" )
		{
			_3DMath::Vector center( 0.0, 0.0, 0.0 );

			wxString attribStr = xmlNode->GetAttribute( "y" );
			if( !attribStr.ToCDouble( &center.y ) )
				return false;

			deathPlane.SetCenterAndNormal( center, _3DMath::Vector( 0.0, 1.0, 0.0 ) );
		}
		else if( xmlNode->GetName() == "Mesh" )
		{
			wxString geometryFile = xmlNode->GetAttribute( "geometry" );
			if( geometryFile.IsEmpty() )
				return false;

			wxString meshData;
			wxFile file( "Data/" + geometryFile );
			if( !file.ReadAll( &meshData ) )
				return false;

			std::stringstream meshDataStream;
			meshDataStream << meshData.c_str();

			_3DMath::FileFormat* format = _3DMath::FileFormat::CreateForFile( ( const char* )geometryFile.c_str() );
			bool meshLoaded = format->LoadTriangleMesh( mesh, meshDataStream );
			delete format;
			if( !meshLoaded )
				return false;

			wxString attribStr = xmlNode->GetAttribute( "scale" );
			double scale = 1.0;
			if( !attribStr.IsEmpty() && attribStr.ToCDouble( &scale ) )
			{
				_3DMath::AffineTransform affineTransform;
				affineTransform.linearTransform.SetScale( scale );
				mesh.Transform( affineTransform );
			}

			if( !texture )
			{
				wxString textureFile = xmlNode->GetAttribute( "texture" );
				if( !textureFile.IsEmpty() )
				{
					texture = new Texture();
					if( !texture->Load( "Data/" + textureFile ) )
						return false;
				}
			}

			if( !boxTree )
			{
				_3DMath::AxisAlignedBox boundingBox;
				if( mesh.GenerateBoundingBox( boundingBox ) )
				{
					boxTree = new _3DMath::BoundingBoxTree();
					boxTree->GenerateNodes( boundingBox, 5 );

					_3DMath::TriangleList triangleList;
					mesh.GenerateTriangleList( triangleList );

					_3DMath::Vector normalFilter( 0.0, 1.0, 0.0 );
					double angleFilter = M_PI / 2.0;
					if( !boxTree->InsertTriangleList( triangleList, &normalFilter, angleFilter ) )
						return false;
				}
			}
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
		blob->RegisterTrackCollisionObject( boxTree, 1.0 );
	}

	// TODO: Load spline data from XML.  This helps AI blobs blob-around the stage.

	return true;
}

bool Stage::Unload( void )
{
	DeleteBlobList();

	mesh.Clear();

	delete texture;
	texture = nullptr;

	delete boxTree;
	boxTree = nullptr;

	return true;
}

void Stage::Render( _3DMath::Renderer& renderer )
{
	if( wxGetApp().frame->debugDrawFlags & Frame::DRAW_COLLISION_OBJECTS )
	{
		if( boxTree )
		{
			glDisable( GL_TEXTURE_2D );
			renderer.random.Seed(0);
			renderer.DrawBoundingBoxTree( *boxTree, _3DMath::Renderer::DRAW_BOXES | _3DMath::Renderer::DRAW_TRIANGLES );
		}
	}
	else
	{
		if( texture )
			texture->Bind();

		// TODO: To implement track mesh self-shadowing, we would have to figure out
		//       a way to render the scene from the light-source perspective to generate
		//       an off-screen depth-map.  This map would then be passed to the shader
		//       that will get used to render the mesh.  It's worth trying to figure out.
		//       Note that this would also handle blob shadowing for us.  Look up "Framebuffer Objects."

		renderer.DrawTriangleMesh( mesh );
	}

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
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
			//...
		}

		if( deathPlane.GetSide( location ) == _3DMath::Plane::SIDE_BACK )
		{
			_3DMath::AffineTransform transform;
			transform.translation.Subtract( respawnLocation, location );
			blob->Teleport( transform );
		}
	}
}

// Stage.cpp