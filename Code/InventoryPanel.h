// InventoryPanel.h

#pragma once

#include "Panel.h"
#include <wx/listbox.h>

class Message;

class InventoryPanel : public Panel
{
public:

	InventoryPanel( void );
	virtual ~InventoryPanel( void );

	virtual void SetupPaneInfo( wxAuiPaneInfo& paneInfo ) override;
	virtual void CreateControls( void ) override;
	virtual void Update( void ) override;

	void InventoryChangedMessageHandler( Message* message );

	wxListBox* listBox;
};

// InventoryPanel.h