// HumanDriver.cpp

#include "HumanDriver.h"
#include "Controller.h"
#include "Application.h"
#include "Camera.h"
#include "Blob.h"
#include <ParticleSystem.h>

HumanDriver::HumanDriver( void )
{
}

/*virtual*/ HumanDriver::~HumanDriver( void )
{
}

/*virtual*/ void HumanDriver::Drive( Blob* blob )
{
	Controller* controller = wxGetApp().controller;

	Camera* camera = wxGetApp().GetCamera();
	if( camera->mode == Camera::MODE_FREE_CAM )
		return;

	double leftTriggerValue, rightTriggerValue;
	controller->GetAnalogTrigger( Controller::LEFT_SIDE, leftTriggerValue );
	controller->GetAnalogTrigger( Controller::RIGHT_SIDE, rightTriggerValue );
	
	_3DMath::ParticleSystem::TorqueForce* torqueForce = new _3DMath::ParticleSystem::TorqueForce( blob->GetParticleSystem() );
	double torqueScale = -blob->maxTorque * ( rightTriggerValue - leftTriggerValue );
	torqueForce->torque.SetScaled( camera->viewTransform.linearTransform.xAxis, torqueScale );
	torqueForce->transient = true;
	blob->GetParticleSystem()->forceCollection.AddObject( torqueForce );
}

// HumanDriver.cpp