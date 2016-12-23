// ModifiersPanel.cpp

#include "ModifiersPanel.h"
#include <wx/sizer.h>

ModifiersPanel::ModifiersPanel( void )
{
}

/*virtual*/ ModifiersPanel::~ModifiersPanel( void )
{
}

/*virtual*/ void ModifiersPanel::SetupPaneInfo( wxAuiPaneInfo& paneInfo )
{
	paneInfo.Bottom();
}

/*virtual*/ void ModifiersPanel::CreateControls( void )
{
	// TODO: Listen for property modifier messages.
}

/*virtual*/ void ModifiersPanel::Update( void )
{
}

// ModifiersPanel.cpp