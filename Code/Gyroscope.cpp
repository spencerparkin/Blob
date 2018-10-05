// Gyroscope.cpp

#include "Gyroscope.h"
#include "Application.h"
#include "Controller.h"
#include <Vector.h>
#include <TimeKeeper.h>

Gyroscope::Gyroscope( double radius, double mass, int num_particles )
{
	double particle_mass = mass / double( num_particles );

	_3DMath::ParticleSystem::GenericParticle** particleArray = new _3DMath::ParticleSystem::GenericParticle*[ num_particles ];

	for( int i = 0; i < num_particles; i++ )
	{
		double angle = 2.0 * M_PI * double(i) / double( num_particles );
		_3DMath::Vector position( radius * cos( angle ), radius * sin( angle ), 0.0 );
		_3DMath::ParticleSystem::GenericParticle* particle = new _3DMath::ParticleSystem::GenericParticle( &position );
		particle->mass = particle_mass;
		particleSystem.particleList->push_back( particle );
		particleArray[i] = particle;
	}

	_3DMath::ParticleSystem::GenericParticle* centerParticleA = new _3DMath::ParticleSystem::GenericParticle();
	centerParticleA->SetPosition( _3DMath::Vector( 0.0, 0.0, -radius / 5.0 ) );
	particleSystem.particleList->push_back( centerParticleA );

	_3DMath::ParticleSystem::GenericParticle* centerParticleB = new _3DMath::ParticleSystem::GenericParticle();
	centerParticleA->SetPosition( _3DMath::Vector( 0.0, 0.0, radius / 5.0 ) );
	particleSystem.particleList->push_back( centerParticleB );

	_3DMath::ParticleSystem::SpringForce* springForce = new _3DMath::ParticleSystem::SpringForce( &particleSystem );
	springForce->endPointParticleHandles[0] = centerParticleA->GetHandle();
	springForce->endPointParticleHandles[1] = centerParticleB->GetHandle();
	springForce->ResetEquilibriumLength();
	particleSystem.forceList->push_back( springForce );

	for( int i = 0; i < num_particles; i++ )
	{
		int j = ( i + 1 ) % num_particles;

		springForce = new _3DMath::ParticleSystem::SpringForce( &particleSystem );
		springForce->endPointParticleHandles[0] = particleArray[i]->GetHandle();
		springForce->endPointParticleHandles[1] = particleArray[j]->GetHandle();
		springForce->ResetEquilibriumLength();
		particleSystem.forceList->push_back( springForce );

		springForce = new _3DMath::ParticleSystem::SpringForce( &particleSystem );
		springForce->endPointParticleHandles[0] = particleArray[i]->GetHandle();
		springForce->endPointParticleHandles[1] = centerParticleA->GetHandle();
		springForce->ResetEquilibriumLength();
		particleSystem.forceList->push_back( springForce );

		springForce = new _3DMath::ParticleSystem::SpringForce( &particleSystem );
		springForce->endPointParticleHandles[0] = particleArray[i]->GetHandle();
		springForce->endPointParticleHandles[1] = centerParticleB->GetHandle();
		springForce->ResetEquilibriumLength();
		particleSystem.forceList->push_back( springForce );
	}

	delete[] particleArray;
}

/*virtual*/ Gyroscope::~Gyroscope( void )
{
}

void Gyroscope::Render( _3DMath::Renderer& renderer )
{
	renderer.Color( _3DMath::Vector( 1.0, 1.0, 1.0 ) );
	renderer.DrawParticleSystem( particleSystem, _3DMath::Renderer::DRAW_FORCES );
}

void Gyroscope::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	Controller* controller = wxGetApp().controller;

	if( controller->ButtonDown( Controller::BUTTON_A ) )
	{
		double torqueMag = 30.0;
		_3DMath::ParticleSystem::TorqueForce* torqueForce = new _3DMath::ParticleSystem::TorqueForce( &particleSystem );
		torqueForce->torque = _3DMath::Vector( 0.0, 0.0, torqueMag );
		torqueForce->transient = true;
		particleSystem.forceList->push_back( torqueForce );
	}

	timeKeeper.fixedDeltaTimeMilliseconds = 50.0;
	particleSystem.Simulate( timeKeeper );
	timeKeeper.fixedDeltaTimeMilliseconds = 0.0;
}

// I have no idea if this will work or not.  The plan is to apply a torque
// force on the gyro to get it spinning in the XY plane.  That part is easy.  (Uh...no, the whole things falls apart.  My simulation is too unstable and unsound and not rooted in good physics or calculus methods.)
// Then, each frame, calculate the plane that best fits all the points of the
// gyro, then apply a transient torque force to the gyro in that plane and
// perpendicular to the Z-axis.  The question is then whether the direction
// of angular momentum of the gyro will go toward the torque direction.  I have
// serious doubts that this will work.  If it doesn't work, I'll delete the whole
// project, and pretend I never tried to write it.

// Gyroscope.cpp