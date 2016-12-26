// ModelCache.hpp

template< typename Type >
Type* ModelCache::GetModel( const std::string& modelName )
{
	Type* model = nullptr;

	ModelMap::iterator iter = modelMap.find( modelName );
	if( iter != modelMap.end() )
		model = dynamic_cast< Type* >( iter->second );
	else
	{
		model = Type::Create();

		if( model && model->Load( modelName ) )
			modelMap.insert( std::pair< std::string, Model* >( modelName, model ) );
		else
		{
			delete model;
			model = nullptr;
		}
	}

	return model;
}

// ModelCache.hpp