// ComputerDriver.h

#pragma once

#include "Driver.h"

class ComputerDriver : public Driver
{
public:

	ComputerDriver( void );
	virtual ~ComputerDriver( void );

	virtual void Drive( Blob* blob, const _3DMath::TimeKeeper& timeKeeper ) override;
};

// ComputerDriver.h