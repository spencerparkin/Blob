// HumanDriver.h

#pragma once

#include "Driver.h"

class HumanDriver : public Driver
{
public:

	HumanDriver( void );
	virtual ~HumanDriver( void );

	virtual void Drive( Blob* blob, const _3DMath::TimeKeeper& timeKeeper ) override;
};

// HumanDriver.h
