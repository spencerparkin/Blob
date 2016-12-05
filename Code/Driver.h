// Driver.h

#pragma once

class Blob;

class Driver
{
public:

	Driver( void );
	virtual ~Driver( void );

	virtual void Drive( Blob* blob ) = 0;
};

// Driver.h
