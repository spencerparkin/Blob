// Message.cpp

#include "Message.h"
#include <ListFunctions.h>

//-----------------------------------------------------------------------------------
//                                 MessageSystem
//-----------------------------------------------------------------------------------

MessageSystem::MessageSystem( void )
{
}

/*virtual*/ MessageSystem::~MessageSystem( void )
{
	Clear();
}

// All messages are handled asynchronously.
void MessageSystem::SendAMessage( Message* message )
{
	std::string messageType = message->GetType();
	MessageMapEntry* entry = GetMessageMapEntry( messageType );
	entry->messageList.push_back( message );
}

void MessageSystem::RegisterMessageHandler( const std::string& messageType, MessageHandler messageHandler )
{
	MessageMapEntry* entry = GetMessageMapEntry( messageType );
	entry->messageHandlerList.push_back( messageHandler );
}

void MessageSystem::DispatchAllMessages( void )
{
	for( MessageMap::iterator iter = messageMap.begin(); iter != messageMap.end(); iter++ )
	{
		MessageMapEntry* entry = iter->second;

		while( entry->messageList.size() > 0 )
		{
			MessageList::iterator msgIter = entry->messageList.begin();
			Message* message = *msgIter;
			entry->messageList.erase( msgIter );

			MessageHandlerList::iterator handlerIter = entry->messageHandlerList.begin();
			while( handlerIter != entry->messageHandlerList.end() )
			{
				MessageHandler& messageHandler = *handlerIter;
				messageHandler( message );
				handlerIter++;
			}

			delete message;
		}
	}
}

void MessageSystem::Clear( void )
{
	while( messageMap.size() > 0 )
	{
		MessageMap::iterator iter = messageMap.begin();
		MessageMapEntry* entry = iter->second;
		_3DMath::FreeList< Message >( entry->messageList );
		delete entry;
		messageMap.erase( iter );
	}
}

MessageSystem::MessageMapEntry* MessageSystem::GetMessageMapEntry( const std::string& messageType )
{
	MessageMapEntry* entry = nullptr;

	MessageMap::iterator iter = messageMap.find( messageType );
	if( iter != messageMap.end() )
		entry = iter->second;
	else
	{
		entry = new MessageMapEntry();
		messageMap.insert( std::pair< std::string, MessageMapEntry* >( messageType, entry ) );
	}

	return entry;
}

//-----------------------------------------------------------------------------------
//                                      Message
//-----------------------------------------------------------------------------------

Message::Message( void )
{
}

/*virtual*/ Message::~Message( void )
{
}

//-----------------------------------------------------------------------------------
//                                StageCompleteMessage
//-----------------------------------------------------------------------------------

StageCompleteMessage::StageCompleteMessage( void )
{
}

/*virtual*/ StageCompleteMessage::~StageCompleteMessage( void )
{
}

/*virtual*/ std::string StageCompleteMessage::GetType( void ) const
{
	return "StageComplete";
}

//-----------------------------------------------------------------------------------
//                              PropertyModifiedMessage
//-----------------------------------------------------------------------------------

PropertyModifiedMessage::PropertyModifiedMessage( int blobHandle, int propertyHandle, int modifierHandle )
{
	this->blobHandle = blobHandle;
	this->propertyHandle = propertyHandle;
	this->modifierHandle = modifierHandle;
}

/*virtual*/ PropertyModifiedMessage::~PropertyModifiedMessage( void )
{
}

/*virtual*/ std::string PropertyModifiedMessage::GetType( void ) const
{
	return "PropertyModified";
}

// Message.cpp