// Blob.h

#pragma once

#include <TriangleMesh.h>
#include <ParticleSystem.h>
#include <Renderer.h>
#include "Camera.h"

class Blob : public Camera::Subject
{
public:

	Blob( void );
	virtual ~Blob( void );

	void Render( _3DMath::Renderer& renderer );

	void MakeRegularHexadron( void );
	void MakeRegularIcosahedron( void );
	void MakeRegularDodecahedron( void );

	virtual void GetLocation( _3DMath::Vector& location ) const override;

private:

	void MakeSpring( const _3DMath::Vector& vector );
	void MakeSpring( int index0, int index1 );

	_3DMath::ParticleSystem particleSystem;
	_3DMath::TriangleMesh triangleMesh;
};

// Blob.h