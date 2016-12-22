// Frame.h

#pragma once

#include <wx/frame.h>
#include <wx/timer.h>
#include <wx/aui/aui.h>

class Canvas;

class Frame : public wxFrame
{
public:

	Frame( void );
	virtual ~Frame( void );

	Canvas* GetCanvas( void );

	enum DebugDrawFlag
	{
		DRAW_FORCES				= 0x00000001,
		DRAW_COLLISION_OBJECTS	= 0x00000002,
	};

	int debugDrawFlags;

	void UpdateAllPanels( void );
	bool IsPanelInUse( const wxString& panelTitle, wxAuiPaneInfo** foundPaneInfo = nullptr );
	bool TogglePanel( const wxString& panelTitle );

private:

	enum
	{
		ID_Exit = wxID_HIGHEST,
		ID_FreeCam,
		ID_DrawForces,
		ID_DrawCollisionObjects,
		ID_InventoryPanelToggle,
		ID_ModifiersPanelToggle,
		ID_SoundToggle,
		ID_About,
		ID_Timer,
	};

	void OnExit( wxCommandEvent& event );
	void OnTimer( wxTimerEvent& event );
	void OnToggleFreeCam( wxCommandEvent& event );
	void OnToggleDrawForces( wxCommandEvent& event );
	void OnToggleDrawCollisionObjects( wxCommandEvent& event );
	void OnToggleInventoryPanel( wxCommandEvent& event );
	void OnToggleModifiersPanel( wxCommandEvent& event );
	void OnToggleSound( wxCommandEvent& event );
	void OnUpdateUI( wxUpdateUIEvent& event );
	void OnAbout( wxCommandEvent& event );

	wxAuiManager* auiManager;
	wxTimer timer;
};

// Frame.h
