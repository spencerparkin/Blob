// ModelCache.cpp

#include "ModelCache.h"
#include "Texture.h"
#include "Application.h"
#include "Frame.h"
#include <FileFormat.h>
#include <wx/file.h>
#include <wx/filename.h>
#include <sstream>

//---------------------------------------------------------------------------
//                                 ModelCache
//---------------------------------------------------------------------------

ModelCache::ModelCache( void )
{
}

/*virtual*/ ModelCache::~ModelCache( void )
{
	Clear();
}

Model* ModelCache::GetModel( const std::string& modelName, Type type /*= MODEL_TYPE_GENERIC*/ )
{
	Model* model = nullptr;

	ModelMap::iterator iter = modelMap.find( modelName );
	if( iter != modelMap.end() )
		model = iter->second;
	else
	{
		switch( type )
		{
			default:
			case MODEL_TYPE_GENERIC:
			{
				model = new Model();
				break;
			}
			case MODEL_TYPE_GROUND:
			{
				model = new GroundModel();
				break;
			}
		}

		if( model->Load( modelName ) )
			modelMap.insert( std::pair< std::string, Model* >( modelName, model ) );
		else
		{
			delete model;
			model = nullptr;
		}
	}

	return model;
}

void ModelCache::Clear( void )
{
	while( modelMap.size() > 0 )
	{
		ModelMap::iterator iter = modelMap.begin();
		Model* model = iter->second;
		delete model;
		modelMap.erase( iter );
	}
}

void ModelCache::Clear( const std::string& modelName )
{
	ModelMap::iterator iter = modelMap.find( modelName );
	if( iter != modelMap.end() )
	{
		Model* model = iter->second;
		delete model;
		modelMap.erase( iter );
	}
}

//---------------------------------------------------------------------------
//                                    Model
//---------------------------------------------------------------------------

Model::Model( void )
{
	texture = nullptr;
}

/*virtual*/ Model::~Model( void )
{
	Unload();
}

/*virtual*/ bool Model::Load( const std::string& modelName )
{
	Unload();

	wxFileName fileName( ( "Data/" + modelName ).c_str() );
	fileName.SetExt( "obj" );

	wxString meshData;
	wxFile file( fileName.GetFullPath() );
	if( !file.ReadAll( &meshData ) )
		return false;

	std::stringstream meshDataStream;
	meshDataStream << meshData.c_str();

	_3DMath::FileFormat* format = _3DMath::FileFormat::CreateForFile( ( const char* )fileName.GetFullPath().c_str() );
	if( !format )
		return false;

	bool meshLoaded = format->LoadTriangleMesh( mesh, meshDataStream );
	delete format;
	if( !meshLoaded )
		return false;

	fileName.SetExt( "png" );
	if( fileName.Exists() )
	{
		texture = new Texture();
		if( !texture->Load( fileName.GetFullPath() ) )
			return false;
	}

	return true;
}

/*virtual*/ bool Model::Unload( void )
{
	delete texture;
	texture = nullptr;

	mesh.Clear();

	return true;
}

/*virtual*/ void Model::Render( _3DMath::Renderer& renderer, const _3DMath::AffineTransform* affineTransform /*= nullptr*/ ) const
{
	if( texture )
		texture->Bind();

	renderer.DrawTriangleMesh( mesh, 0, affineTransform );
}

//---------------------------------------------------------------------------
//                                 GroundModel
//---------------------------------------------------------------------------

GroundModel::GroundModel( void )
{
	boxTree = nullptr;
}

/*virtual*/ GroundModel::~GroundModel( void )
{
	Unload();
}

/*virtual*/ bool GroundModel::Load( const std::string& modelName )
{
	if( !Model::Load( modelName ) )
		return false;

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

	return true;
}

/*virtual*/ bool GroundModel::Unload( void )
{
	Model::Unload();

	delete boxTree;
	boxTree = nullptr;

	return true;
}

/*virtual*/ void GroundModel::Render( _3DMath::Renderer& renderer, const _3DMath::AffineTransform* affineTransform /*= nullptr*/ ) const
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
		Model::Render( renderer, affineTransform );
	}
}

// ModelCache.cpp