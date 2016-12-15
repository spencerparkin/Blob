// Race.cpp

#include "Race.h"
#include "Texture.h"
#include "Blob.h"
#include "HumanDriver.h"
#include "ComputerDriver.h"
#include "Application.h"
#include "Camera.h"
#include <FileFormat.h>
#include <wx/xml/xml.h>
#include <wx/wfstream.h>
#include <sstream>

Race::Race( void )
{
	laps = 0;

	raceTrackMeshTexture = nullptr;
	boxTree = nullptr;
}

/*virtual*/ Race::~Race( void )
{
	( void )Unload();
}

void Race::DeleteBlobList( void )
{
	while( blobList.size() > 0 )
	{
		BlobList::iterator iter = blobList.begin();
		Blob* blob = *iter;
		delete blob;
		blobList.erase( iter );
	}
}

bool Race::Load( const wxString& raceFile )
{
	if( !Unload() )
		return false;

	wxFileInputStream raceFileStream( raceFile );
	if( !raceFileStream.IsOk() )
		return false;

	wxXmlDocument xmlDoc( raceFileStream );
	if( !xmlDoc.IsOk() )
		return false;

	wxXmlNode* xmlRootNode = xmlDoc.GetRoot();
	if( xmlRootNode->GetName() != "Track" )
		return false;

	for( wxXmlNode* xmlNode = xmlRootNode->GetChildren(); xmlNode; xmlNode = xmlNode->GetNext() )
	{
		if( xmlNode->GetName() == "Mesh" )
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
			bool meshLoaded = format->LoadTriangleMesh( raceTrackMesh, meshDataStream );
			delete format;
			if( !meshLoaded )
				return false;

			if( !raceTrackMeshTexture )
			{
				wxString textureFile = xmlNode->GetAttribute( "texture" );
				if( !textureFile.IsEmpty() )
				{
					raceTrackMeshTexture = new Texture();
					if( !raceTrackMeshTexture->Load( "Data/" + textureFile ) )
						return false;
				}
			}

			if( !boxTree )
			{
				_3DMath::AxisAlignedBox boundingBox;
				if( raceTrackMesh.GenerateBoundingBox( boundingBox ) )
				{
					boxTree = new _3DMath::BoundingBoxTree();
					boxTree->GenerateNodes( boundingBox, 5 );

					_3DMath::TriangleList triangleList;
					raceTrackMesh.GenerateTriangleList( triangleList );

					if( !boxTree->InsertTriangleList( triangleList ) )
						return false;
				}
			}
		}
		else if( xmlNode->GetName() == "Laps" )
		{
			wxString lapString = xmlNode->GetChildren()->GetContent();

			long lapsLong;
			if( !lapString.ToLong( &lapsLong ) )
				return false;

			laps = ( int )lapsLong;
		}
		else if( xmlNode->GetName() == "Blob" )
		{
			Blob* blob = new Blob();
			blobList.push_back( blob );

			wxString attribStr = xmlNode->GetAttribute( "scale" );
			double scale = 1.0;
			attribStr.ToCDouble( &scale );

			_3DMath::AffineTransform affineTransform;
			affineTransform.linearTransform.SetScale( scale );

			attribStr = xmlNode->GetAttribute( "x" );
			attribStr.ToCDouble( &affineTransform.translation.x );
			attribStr = xmlNode->GetAttribute( "y" );
			attribStr.ToCDouble( &affineTransform.translation.y );
			attribStr = xmlNode->GetAttribute( "z" );
			attribStr.ToCDouble( &affineTransform.translation.z );

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
				camera->mode = Camera::MODE_FOLLOW_SUBJECT;
			}
			else if( attribStr == "computer" )
				blob->driver = new ComputerDriver();
		}
	}

	// TODO: Load spline data from XML.  This can be used to determine laps
	//       and keep AI blobs on track.

	return true;
}

bool Race::Unload( void )
{
	DeleteBlobList();

	raceTrackMesh.Clear();

	delete raceTrackMeshTexture;
	raceTrackMeshTexture = nullptr;

	delete boxTree;
	boxTree = nullptr;

	return true;
}

void Race::Render( _3DMath::Renderer& renderer )
{
	if( raceTrackMeshTexture )
		raceTrackMeshTexture->Bind();

	// TODO: It would be neat to write our own shader for drawing the track mesh
	//       so that we could shadow the blobs based on a light source.

	renderer.DrawTriangleMesh( raceTrackMesh );

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
	}

#if 0
	if( boxTree )
	{
		glDisable( GL_TEXTURE_2D );
		renderer.random.Seed(0);
		renderer.DrawBoundingBoxTree( *boxTree, _3DMath::Renderer::DRAW_BOXES | _3DMath::Renderer::DRAW_TRIANGLES );
	}
#endif
}

void Race::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Simulate( timeKeeper );
	}
}

// Race.cpp