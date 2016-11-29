// Blob.h

#pragma once

#include <TriangleMesh.h>
#include <ParticleSystem.h>

class Blob
{
public:

	Blob( void );
	virtual ~Blob( void );

	void MakeRegularHexadron( void );
	void MakeRegularIcosahedron( void );
	void MakeRegularDodecahedron( void );

private:

	void MakeSpring( const _3DMath::Vector& vector );
	void MakeSpring( int index0, int index1 );

	_3DMath::ParticleSystem particleSystem;
	_3DMath::TriangleMesh triangleMesh;
};

// Blob.h