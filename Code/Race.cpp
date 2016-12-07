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
			wxString meshData = xmlNode->GetChildren()->GetContent();

			std::stringstream meshDataStream;
			meshDataStream << meshData.c_str();

			_3DMath::PlyFormat* plyFormat = new _3DMath::PlyFormat();
			bool meshLoaded = plyFormat->LoadTriangleMesh( raceTrackMesh, meshDataStream );
			delete plyFormat;
			if( !meshLoaded )
				return false;

			if( !raceTrackMeshTexture )
			{
				wxString textureFile = xmlNode->GetAttribute( "texture" );
				if( !textureFile.IsEmpty() )
				{
					raceTrackMeshTexture = new Texture();
					if( !raceTrackMeshTexture->Load( textureFile ) )
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
	}

	return true;
}

bool Race::Unload( void )
{
	DeleteBlobList();

	raceTrackMesh.Clear();

	delete raceTrackMeshTexture;
	raceTrackMeshTexture = nullptr;

	return true;
}

void Race::Render( _3DMath::Renderer& renderer )
{
	if( raceTrackMeshTexture )
		raceTrackMeshTexture->Bind();

	renderer.DrawTriangleMesh( raceTrackMesh );

	for( BlobList::iterator iter = blobList.begin(); iter != blobList.end(); iter++ )
	{
		Blob* blob = *iter;
		blob->Render( renderer );
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