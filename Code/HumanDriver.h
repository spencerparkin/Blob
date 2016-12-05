// HumanDriver.h

#pragma once

#include "Driver.h"

class HumanDriver : public Driver
{
public:

	HumanDriver( void );
	virtual ~HumanDriver( void );

	virtual void Drive( Blob* blob ) override;
};

// HumanDriver.h
