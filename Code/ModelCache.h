// ModelCache.h

#pragma once

#include <TriangleMesh.h>
#include <BoundingBoxTree.h>
#include <BspTree.h>

class Model;
class Texture;

typedef std::map< std::string, Model* > ModelMap;
typedef std::list< std::string > ModelNameList;

class ModelCache
{
public:

	ModelCache( void );
	virtual ~ModelCache( void );

	template< typename Type >
	Type* GetModel( const std::string& modelName );

	void Clear( void );
	void Clear( const std::string& modelName );

private:

	ModelMap modelMap;
};

class Model
{
public:

	Model( void );
	virtual ~Model( void );

	static Model* Create( void ) { return new Model(); }

	virtual bool Load( const std::string& modelName );
	virtual bool Unload( void );
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::AffineTransform* affineTransform = nullptr ) const;

	_3DMath::TriangleMesh mesh;
	Texture* texture;
};

class GroundModel : public Model
{
public:

	GroundModel( void );
	virtual ~GroundModel( void );

	static GroundModel* Create( void ) { return new GroundModel(); }

	virtual bool Load( const std::string& modelName ) override;
	virtual bool Unload( void ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::AffineTransform* affineTransform = nullptr ) const override;

	_3DMath::BoundingBoxTree* boxTree;
};

#include "ModelCache.hpp"

// ModelCache.h
