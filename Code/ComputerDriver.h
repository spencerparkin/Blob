// ComputerDriver.h

#pragma once

#include "Driver.h"

class ComputerDriver : public Driver
{
public:

	ComputerDriver( void );
	virtual ~ComputerDriver( void );

	virtual void Drive( Blob* blob ) override;
};

// ComputerDriver.h