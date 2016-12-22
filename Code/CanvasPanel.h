// CanvasPanel.h

#pragma once

#include "Panel.h"

class Canvas;

class CanvasPanel : public Panel
{
public:

	CanvasPanel( void );
	virtual ~CanvasPanel( void );

	virtual void SetupPaneInfo( wxAuiPaneInfo& paneInfo ) override;
	virtual void CreateControls( void ) override;
	virtual void Update( void ) override;

	Canvas* canvas;
};

// CanvasPanel.h