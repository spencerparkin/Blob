// Driver.h

#pragma once

#include <TimeKeeper.h>

class Blob;

class Driver
{
public:

	Driver( void );
	virtual ~Driver( void );

	virtual void Drive( Blob* blob, const _3DMath::TimeKeeper& timeKeeper ) = 0;
};

// Driver.h
