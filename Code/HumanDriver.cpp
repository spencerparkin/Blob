// HumanDriver.cpp

#include "HumanDriver.h"
#include "Controller.h"
#include "Application.h"

HumanDriver::HumanDriver( void )
{
}

/*virtual*/ HumanDriver::~HumanDriver( void )
{
}

/*virtual*/ void HumanDriver::Drive( Blob* blob )
{
	Controller* controller = wxGetApp().controller;

	// TODO: Use controller and maybe camera context to drive the given blob.
}

// HumanDriver.cpp