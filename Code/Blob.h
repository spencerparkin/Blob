// Blob.h

#pragma once

#include <TriangleMesh.h>
#include <ParticleSystem.h>
#include <Renderer.h>
#include "Camera.h"

class Texture;

class Blob : public Camera::Subject
{
public:

	Blob( void );
	virtual ~Blob( void );

	void Render( _3DMath::Renderer& renderer );
	void Simulate( double currentTime );

	enum Polyhedron
	{
		HEXADRON,
		ICOSAHEDRON,
		DODECAHEDRON,
	};

	void MakePolyhedron( Polyhedron polyhedron );

	virtual void GetLocation( _3DMath::Vector& location ) const override;

private:

	Texture* texture;

	void AddVertexPair( const _3DMath::Vector& vector );
	void MakeSpring( int index0, int index1, std::vector< int >& particleIds, double stiffness );

	_3DMath::ParticleSystem particleSystem;
	_3DMath::TriangleMesh triangleMesh;

	double maxTorque;
};

// Blob.h