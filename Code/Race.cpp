// Race.cpp

#include "Race.h"
#include "Texture.h"
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

					//boxTree->InsertTriangleList( triangleList );
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
	}

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

	//renderer.drawStyle = _3DMath::Renderer::DRAW_STYLE_WIRE_FRAME;
	//glColor3f( 1.f, 1.f, 1.f );

	renderer.DrawTriangleMesh( raceTrackMesh );

	glDisable( GL_TEXTURE_2D );

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
	}

	if( boxTree )
	{
		glColor3f( 1.f, 1.f, 1.f );
		//renderer.DrawBoundingBoxTree( *boxTree );
	}
}

void Race::Simulate( double currentTime )
{
	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Simulate( currentTime );
	}
}

// Race.cpp