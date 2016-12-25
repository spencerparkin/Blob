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

	enum Type
	{
		MODEL_TYPE_GENERIC,
		MODEL_TYPE_GROUND,
	};

	Model* GetModel( const std::string& modelName, Type type = MODEL_TYPE_GENERIC );

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

	virtual bool Load( const std::string& modelName ) override;
	virtual bool Unload( void ) override;
	virtual void Render( _3DMath::Renderer& renderer, const _3DMath::AffineTransform* affineTransform = nullptr ) const override;

	_3DMath::BoundingBoxTree* boxTree;
};

// ModelCache.h
