// HumanDriver.cpp

#include "HumanDriver.h"
#include "Controller.h"
#include "Application.h"
#include "Blob.h"
#include <ParticleSystem.h>

HumanDriver::HumanDriver( void )
{
	maxTurnRate = M_PI / 4.0;
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
	blob->axleAngle += maxTurnRate * timeKeeper.GetDeltaTimeSeconds() * unitDir.x * mag;

	// Drive the blob.
	_3DMath::Vector unitAxleAxis;
	blob->GetAxleAxis( unitAxleAxis );

	double leftTriggerValue, rightTriggerValue;
	controller->GetAnalogTrigger( Controller::LEFT_SIDE, leftTriggerValue );
	controller->GetAnalogTrigger( Controller::RIGHT_SIDE, rightTriggerValue );
	
	double maxTorque = 0.0;
	blob->maxTorque.GetValue( maxTorque );

	_3DMath::ParticleSystem::TorqueForce* torqueForce = new _3DMath::ParticleSystem::TorqueForce( blob->GetParticleSystem() );
	double torqueMag = maxTorque * ( leftTriggerValue - rightTriggerValue );
	torqueForce->torque.SetScaled( unitAxleAxis, torqueMag );
	torqueForce->transient = true;
	blob->GetParticleSystem()->forceList->push_back( torqueForce );
}

// HumanDriver.cpp