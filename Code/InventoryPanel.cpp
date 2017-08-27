// InventoryPanel.cpp

#include "InventoryPanel.h"
#include "Application.h"
#include "Message.h"
#include "Blob.h"
#include "Stage.h"
#include "HumanDriver.h"
#include "Controller.h"
#include <wx/sizer.h>
#include <wx/regex.h>
#include <strstream>

InventoryPanel::InventoryPanel( void )
{
	listBox = nullptr;
}

/*virtual*/ InventoryPanel::~InventoryPanel( void )
{
}

/*virtual*/ void InventoryPanel::SetupPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Bottom();
	paneInfo.BestSize( -1, 50 );
}

/*virtual*/ void InventoryPanel::CreateControls( void )
{
	listBox = new wxListBox( this, wxID_ANY );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( listBox, 1, wxGROW | wxALL, 0 );
	SetSizer( boxSizer );

	MessageSystem::MessageHandler messageHandler = std::bind( &InventoryPanel::InventoryChangedMessageHandler, this, std::placeholders::_1 );
	wxGetApp().messageSystem->RegisterMessageHandler( "InventoryChanged", messageHandler );
}

/*virtual*/ void InventoryPanel::Update( void )
{
	Controller* controller = wxGetApp().controller;

	int selection = -1;

	if( controller->ButtonPressed( Controller::BUTTON_DPAD_DN ) )
	{
		selection = listBox->GetSelection();
		if( selection == wxNOT_FOUND )
			selection = 0;
		else if( --selection < 0 )
			selection = listBox->GetCount() - 1;
	}
	else if( controller->ButtonPressed( Controller::BUTTON_DPAD_UP ) )
	{
		selection = listBox->GetSelection();
		if( selection == wxNOT_FOUND )
			selection = listBox->GetCount() - 1;
		else if( ++selection >= ( signed )listBox->GetCount() )
			selection = 0;
	}

	if( selection >= 0 && selection < ( signed )listBox->GetCount() )
		listBox->SetSelection( selection );

	if( controller->ButtonPressed( Controller::BUTTON_L_SHOULDER ) )
	{
		selection = listBox->GetSelection();
		if( selection != wxNOT_FOUND )
		{
			wxString inventoryItemString = listBox->GetString( selection );

			wxRegEx regex( "(.*)\\([0-9]+\\)" );
			if( regex.Matches( inventoryItemString ) )
			{
				inventoryItemString = regex.GetMatch( inventoryItemString, 1 );
				inventoryItemString.Trim();

				std::string displayName( inventoryItemString.c_str() );

				Blob* blob = wxGetApp().stage->GetHumanDrivenBlob();
				blob->UseInventoryItem( displayName );
			}
		}
	}
}

void InventoryPanel::InventoryChangedMessageHandler( Message* message )
{
	InventoryChangedMessage* inventoryChangedMessage = dynamic_cast< InventoryChangedMessage* >( message );
	if( !inventoryChangedMessage )
		return;

	Blob* blob = ( Blob* )_3DMath::HandleObject::Dereference( inventoryChangedMessage->blobHandle );
	if( !blob || !dynamic_cast< HumanDriver* >( blob->driver ) )
		return;

	std::map< std::string, int > inventoryMap;

	for( InventoryItemList::const_iterator iter = blob->inventoryItemList->cbegin(); iter != blob->inventoryItemList->cend(); iter++ )
	{
		const InventoryItem* inventoryItem = *iter;
		std::string displayName = inventoryItem->GetDisplayName();
		std::map< std::string, int >::iterator mapIter = inventoryMap.find( displayName );
		if( mapIter == inventoryMap.end() )
			inventoryMap.insert( std::pair< std::string, int >( displayName, 1 ) );
		else
			mapIter->second++;
	}

	wxArrayString stringArray;
	for( std::map< std::string, int >::iterator iter = inventoryMap.begin(); iter != inventoryMap.end(); iter++ )
	{
		std::strstream stream;
		stream << iter->first << " (" << iter->second << ")" << std::ends;
		stringArray.Add( stream.str() );
	}
	stringArray.Sort();

	listBox->Clear();
	if( stringArray.Count() > 0 )
		listBox->InsertItems( stringArray, 0 );
}

// InventoryPanel.cpp