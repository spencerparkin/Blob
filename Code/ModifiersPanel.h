// ModifiersPanel.h

#pragma once

#include "Panel.h"

class ModifiersPanel : public Panel
{
public:

	ModifiersPanel( void );
	virtual ~ModifiersPanel( void );

	virtual void SetupPaneInfo( wxAuiPaneInfo& paneInfo ) override;
	virtual void CreateControls( void ) override;
	virtual void Update( void ) override;

	//...
};

// ModifiersPanel.h