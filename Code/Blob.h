// Blob.h

#pragma once

#include <TriangleMesh.h>
#include <ParticleSystem.h>

class Blob
{
public:

	Blob( void );
	virtual ~Blob( void );

	_3DMath::ParticleSystem particleSystem;
	_3DMath::TriangleMesh triangleMesh;
};

// Blob.h