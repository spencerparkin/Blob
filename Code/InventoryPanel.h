// InventoryPanel.h

#pragma once

#include "Panel.h"

class InventoryPanel : public Panel
{
public:

	InventoryPanel( void );
	virtual ~InventoryPanel( void );

	virtual void SetupPaneInfo( wxAuiPaneInfo& paneInfo ) override;
	virtual void CreateControls( void ) override;
	virtual void Update( void ) override;

	//...
};

// InventoryPanel.h