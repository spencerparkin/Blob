// InventoryPanel.cpp

#include "InventoryPanel.h"
#include <wx/sizer.h>

InventoryPanel::InventoryPanel( void )
{
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
}

/*virtual*/ void InventoryPanel::Update( void )
{
}

// InventoryPanel.cpp