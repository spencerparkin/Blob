// Property.hpp

//--------------------------------------------------------------------------------------
//                                    Modifier
//--------------------------------------------------------------------------------------

template< typename Type >
Modifier< Type >::Modifier( double lifeTimeSeconds )
{
	remainingTimeSeconds = lifeTimeSeconds;
}

template< typename Type >
/*virtual*/ Modifier< Type >::~Modifier( void )
{
}

//--------------------------------------------------------------------------------------
//                                     Property
//--------------------------------------------------------------------------------------

template< typename Type >
Property< Type >::Property( Type value )
{
	baseValue = value;
}

template< typename Type >
/*virtual*/ Property< Type >::~Property( void )
{
	while( modifierList.size() > 0 )
	{
		std::list< void* >::iterator iter = modifierList.begin();
		Modifier< Type >* modifier = ( Modifier< Type >* )*iter;
		delete modifier;
		modifierList.erase( iter );
	}
}

template< typename Type >
bool Property< Type >::GetValue( Type& value ) const
{
	value = baseValue;

	for( std::list< void* >::const_iterator iter = modifierList.cbegin(); iter != modifierList.cend(); iter++ )
	{
		const Modifier< Type >* modifier = ( const Modifier< Type >* )*iter;
		if( !modifier->Apply( value ) )
			return false;
	}

	return true;
}

template< typename Type >
void Property< Type >::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	std::list< void* >::iterator iter = modifierList.begin();
	while( iter != modifierList.end() )
	{
		std::list< void* >::iterator nextIter = iter;
		nextIter++;

		Modifier< Type >* modifier = ( Modifier< Type >* )*iter;
		modifier->remainingTimeSeconds -= timeKeeper.GetDeltaTimeSeconds();

		if( modifier->remainingTimeSeconds <= 0.0 )
		{
			delete modifier;
			modifierList.erase( iter );
		}

		iter = nextIter;
	}
}

template< typename Type >
void Property< Type >::AddModifier( void* modifier )
{
	modifierList.push_back( modifier );
}

// Property.hpp