// Blob.h

#pragma once

#include <TriangleMesh.h>
#include <ParticleSystem.h>
#include <Renderer.h>
#include <TimeKeeper.h>
#include <AxisAlignedBox.h>
#include "Camera.h"
#include "Property.h"

class Texture;
class Driver;

class Blob : public Camera::Subject
{
public:

	Blob( void );
	virtual ~Blob( void );

	void Render( _3DMath::Renderer& renderer );
	void Simulate( const _3DMath::TimeKeeper& timeKeeper );
	void RegisterTrackCollisionObject( _3DMath::BoundingBoxTree* boxTree, double friction );

	enum Polyhedron
	{
		HEXADRON,
		ICOSAHEDRON,
		DODECAHEDRON,
	};

	void MakePolyhedron( Polyhedron polyhedron, bool subDivide, const _3DMath::AffineTransform& transform );
	void Teleport( const _3DMath::AffineTransform& transform );

	virtual void GetLocation( _3DMath::Vector& location ) const override;
	virtual void GetFacingDirection( _3DMath::Vector& facingUnitDir ) const override;

	Driver* driver;

	_3DMath::ParticleSystem* GetParticleSystem( void ) { return &particleSystem; }

	void GetAxleAxis( _3DMath::Vector& unitAxleAxis ) const;

	double axleAngle;
	Property< double > maxTorque;
	Property< double > friction;
	Property< double > gravity;

private:

	Texture* texture;

	void AddSymmetricVertices( const _3DMath::Vector& vector );
	void MakeSpring( int index0, int index1, std::vector< int >& particleHandles, double stiffness );

	_3DMath::ParticleSystem particleSystem;
	_3DMath::TriangleMesh triangleMesh;
	_3DMath::AxisAlignedBox boundingBox;

	int gravityForceHandle;

	// TODO: Add inventory here?
};

typedef std::list< Blob* > BlobList;

// Blob.h