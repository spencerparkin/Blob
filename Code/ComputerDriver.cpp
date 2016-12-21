// ComputerDriver.cpp

#include "ComputerDriver.h"
#include "Blob.h"
#include <Vector.h>

ComputerDriver::ComputerDriver( void )
{
}

/*virtual*/ ComputerDriver::~ComputerDriver( void )
{
}

/*virtual*/ void ComputerDriver::Drive( Blob* blob, const _3DMath::TimeKeeper& timeKeeper )
{
	// TODO: For now, just go forward all the time.  What I'd like to do is have them follow splines.

	_3DMath::Vector unitAxleAxis;
	blob->GetAxleAxis( unitAxleAxis );

	double maxTorque = 0.0;
	blob->maxTorque.GetValue( maxTorque );

	_3DMath::ParticleSystem::TorqueForce* torqueForce = new _3DMath::ParticleSystem::TorqueForce( blob->GetParticleSystem() );
	torqueForce->torque.SetScaled( unitAxleAxis, -maxTorque );
	torqueForce->transient = true;
	blob->GetParticleSystem()->forceList->push_back( torqueForce );
}

// ComputerDriver.cpp