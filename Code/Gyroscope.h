// Gyroscope.h

#pragma once

#include <ParticleSystem.h>
#include <Renderer.h>

class Gyroscope
{
public:

	Gyroscope( double radius, double mass, int num_particles );
	virtual ~Gyroscope( void );

	void Render( _3DMath::Renderer& renderer );
	void Simulate( const _3DMath::TimeKeeper& timeKeeper );

	_3DMath::ParticleSystem particleSystem;
};

// Gyroscope.h