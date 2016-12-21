// Property.h

#pragma once

#include "TimeKeeper.h"
#include <Vector.h>
#include <HandleObject.h>
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

	void AddModifier( void* modifier );

protected:

	Type baseValue;
	std::list< void* > modifierList;
};

template< typename Type >
class Modifier : public _3DMath::HandleObject
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

	double delta;
};

class LengthModifier : public Modifier< _3DMath::Vector >
{
public:

	LengthModifier( double delta, double lifeTimeSeconds );
	virtual ~LengthModifier( void );

	virtual bool Apply( _3DMath::Vector& value ) const override;

	double delta;
};

#include "Property.hpp"

// Property.h