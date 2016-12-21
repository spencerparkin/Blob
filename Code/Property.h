// Property.h

#pragma once

#include "TimeKeeper.h"
#include <Vector.h>
#include <list>

template< typename Type > class Modifier;

template< typename Type >
class Property
{
public:

	Property( Type value );
	virtual ~Property( void );

	bool GetValue( Type& value ) const;

	void Simulate( const _3DMath::TimeKeeper& timeKeeper );

protected:

	Type baseValue;
	std::list< void* > modifierList;
};

template< typename Type >
class Modifier		// TODO: Derive from object so that handles to us can be created.  I want to show modifiers in the UI.
{
public:

	Modifier( double lifeTimeSeconds );
	virtual ~Modifier( void );

	virtual bool Apply( Type& value ) const = 0;

	double remainingTimeSeconds;
};

class AddModifier : public Modifier< double >
{
public:

	AddModifier( double delta, double lifeTimeSeconds );
	virtual ~AddModifier( void );

	virtual bool Apply( double& value ) const override;

private:

	double delta;
};

class LengthModifier : public Modifier< _3DMath::Vector >
{
public:

	LengthModifier( double delta, double lifeTimeSeconds );
	virtual ~LengthModifier( void );

	virtual bool Apply( _3DMath::Vector& value ) const override;

private:

	double delta;
};

#include "Property.hpp"

// Property.h