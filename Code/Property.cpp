// Property.cpp

#include "Property.h"

//--------------------------------------------------------------------------------------
//                                  AddModifier
//--------------------------------------------------------------------------------------

AddModifier::AddModifier( double delta, double lifeTimeSeconds ) : Modifier< double >( lifeTimeSeconds )
{
	this->delta = delta;
}

/*virtual*/ AddModifier::~AddModifier( void )
{
}

/*virtual*/ bool AddModifier::Apply( double& value ) const
{
	value += delta;
	return true;
};

//--------------------------------------------------------------------------------------
//                                 LengthModifier
//--------------------------------------------------------------------------------------

LengthModifier::LengthModifier( double delta, double lifeTimeSeconds ) : Modifier< _3DMath::Vector >( lifeTimeSeconds )
{
	this->delta = delta;
}

/*virtual*/ LengthModifier::~LengthModifier( void )
{
}

/*virtual*/ bool LengthModifier::Apply( _3DMath::Vector& value ) const
{
	double oldLength = value.Length();
	double newLength = oldLength + delta;
	value.Scale( newLength / oldLength );
	return true;
}

// Property.cpp