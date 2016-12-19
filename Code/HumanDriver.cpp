// HumanDriver.cpp

#include "HumanDriver.h"
#include "Controller.h"
#include "Application.h"
#include "Blob.h"
#include <ParticleSystem.h>

HumanDriver::HumanDriver( void )
{
}

/*virtual*/ HumanDriver::~HumanDriver( void )
{
}

/*virtual*/ void HumanDriver::Drive( Blob* blob, const _3DMath::TimeKeeper& timeKeeper )
{
	Controller* controller = wxGetApp().controller;

	// Steer the blob.
	_3DMath::Vector unitDir;
	double mag = 1.0;
	controller->GetAnalogJoyStick( Controller::LEFT_SIDE, unitDir, mag );
	blob->axleAngle += blob->maxTurnRate * timeKeeper.GetDeltaTimeSeconds() * unitDir.x * mag;

	// Drive the blob.
	_3DMath::Vector unitAxleAxis;
	blob->GetAxleAxis( unitAxleAxis );

	double leftTriggerValue, rightTriggerValue;
	controller->GetAnalogTrigger( Controller::LEFT_SIDE, leftTriggerValue );
	controller->GetAnalogTrigger( Controller::RIGHT_SIDE, rightTriggerValue );
	
	_3DMath::ParticleSystem::TorqueForce* torqueForce = new _3DMath::ParticleSystem::TorqueForce( blob->GetParticleSystem() );
	double torqueScale = -blob->maxTorque * ( rightTriggerValue - leftTriggerValue );
	torqueForce->torque.SetScaled( unitAxleAxis, torqueScale );
	torqueForce->transient = true;
	blob->GetParticleSystem()->forceCollection.AddObject( torqueForce );
}

// HumanDriver.cpp