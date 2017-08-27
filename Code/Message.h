// Message.h

#pragma once

#include <string>
#include <functional>
#include <list>
#include <map>

class Message;

class MessageSystem
{
public:

	MessageSystem( void );
	virtual ~MessageSystem( void );

	typedef std::function< void( Message* ) > MessageHandler;
	typedef std::list< Message* > MessageList;
	typedef std::list< MessageHandler > MessageHandlerList;

	struct MessageMapEntry
	{
		MessageList messageList;
		MessageHandlerList messageHandlerList;
	};

	typedef std::map< std::string, MessageMapEntry* > MessageMap;

	void SendAMessage( Message* message );
	void RegisterMessageHandler( const std::string& messageType, MessageHandler messageHandler );
	//void UnregisterMessageHandler(...);		// An object should unregister itself before it goes out of scope.
	void DispatchAllMessages( void );
	void Clear( void );

private:

	MessageMapEntry* GetMessageMapEntry( const std::string& messageType );

	MessageMap messageMap;	
};

class Message
{
public:

	Message( void );
	virtual ~Message( void );

	virtual std::string GetType( void ) const = 0;
};

class StageCompleteMessage : public Message
{
public:

	StageCompleteMessage( void );
	virtual ~StageCompleteMessage( void );

	virtual std::string GetType( void ) const override;
};

class PropertyModifiedMessage : public Message
{
public:

	PropertyModifiedMessage( int blobHandle, int propertyHandle, int modifierHandle );
	virtual ~PropertyModifiedMessage( void );

	virtual std::string GetType( void ) const override;

	int blobHandle;
	int propertyHandle;
	int modifierHandle;
};

class InventoryChangedMessage : public Message
{
public:

	InventoryChangedMessage( int blobHandle );
	virtual ~InventoryChangedMessage( void );

	virtual std::string GetType( void ) const override;

	int blobHandle;
};

// Message.h
