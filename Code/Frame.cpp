// Frame.cpp

#include "Frame.h"
#include "Canvas.h"
#include "Application.h"
#include "Panel.h"
#include "CanvasPanel.h"
#include "InventoryPanel.h"
#include "ModifiersPanel.h"
#include "Camera.h"
#include <wx/menu.h>
#include <wx/sizer.h>
#include <wx/aboutdlg.h>

Frame::Frame( void ) : wxFrame( 0, wxID_ANY, "Blob", wxDefaultPosition, wxSize( 700, 700 ) ), timer( this, ID_Timer )
{
	debugDrawFlags = 0;

	auiManager = new wxAuiManager( this, wxAUI_MGR_LIVE_RESIZE | wxAUI_MGR_DEFAULT );

	wxMenu* gameMenu = new wxMenu();
	wxMenuItem* toggleFreeCamMenuItem = new wxMenuItem( gameMenu, ID_FreeCam, "Free Cam", "Toggle the free-cam.", wxITEM_CHECK );
	wxMenuItem* drawForcesMenuItem = new wxMenuItem( gameMenu, ID_DrawForces, "Draw Forces", "Toggle the drawing of forces.", wxITEM_CHECK );
	wxMenuItem* drawCollisionObjectsMenuItem = new wxMenuItem( gameMenu, ID_DrawCollisionObjects, "Draw Collision Objects", "Toggle the drawing of collision objects.", wxITEM_CHECK );
	wxMenuItem* exitMenuItem = new wxMenuItem( gameMenu, ID_Exit, "Exit", "Exit the program." );
	gameMenu->Append( toggleFreeCamMenuItem );
	gameMenu->AppendSeparator();
	gameMenu->Append( drawForcesMenuItem );
	gameMenu->Append( drawCollisionObjectsMenuItem );
	gameMenu->AppendSeparator();
	gameMenu->Append( exitMenuItem );

	wxMenu* panelMenu = new wxMenu();
	wxMenuItem* inventoryPanelMenuItem = new wxMenuItem( panelMenu, ID_InventoryPanelToggle, wxT( "Inventory" ), wxT( "Toggle the inventory panel." ), wxITEM_CHECK );
	wxMenuItem* modifiersPanelMenuItem = new wxMenuItem( panelMenu, ID_ModifiersPanelToggle, wxT( "Modifiers" ), wxT( "Toggle the modifiers panel." ), wxITEM_CHECK );
	panelMenu->Append( inventoryPanelMenuItem );
	panelMenu->Append( modifiersPanelMenuItem );

	wxMenu* helpMenu = new wxMenu();
	wxMenuItem* aboutMenuItem = new wxMenuItem( helpMenu, ID_About, wxT( "About" ), wxT( "Popup a dialog giving information about this program." ) );
	helpMenu->Append( aboutMenuItem );

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append( gameMenu, "Game" );
	menuBar->Append( panelMenu, "Panel" );
	menuBar->Append( helpMenu, "Help" );
	SetMenuBar( menuBar );

	wxStatusBar* statusBar = new wxStatusBar( this );
	SetStatusBar( statusBar );

	Bind( wxEVT_MENU, &Frame::OnExit, this, ID_Exit );
	Bind( wxEVT_MENU, &Frame::OnAbout, this, ID_About );
	Bind( wxEVT_MENU, &Frame::OnToggleFreeCam, this, ID_FreeCam );
	Bind( wxEVT_MENU, &Frame::OnToggleDrawForces, this, ID_DrawForces );
	Bind( wxEVT_MENU, &Frame::OnToggleDrawCollisionObjects, this, ID_DrawCollisionObjects );
	Bind( wxEVT_MENU, &Frame::OnToggleInventoryPanel, this, ID_InventoryPanelToggle );
	Bind( wxEVT_MENU, &Frame::OnToggleModifiersPanel, this, ID_ModifiersPanelToggle );
	Bind( wxEVT_TIMER, &Frame::OnTimer, this, ID_Timer );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_FreeCam );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_DrawForces );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_DrawCollisionObjects );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_InventoryPanelToggle );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_ModifiersPanelToggle );

	TogglePanel( "Canvas" );

	timer.Start(1);
}

/*virtual*/ Frame::~Frame( void )
{
	auiManager->UnInit();
	delete auiManager;
}

void Frame::OnAbout( wxCommandEvent& event )
{
	wxAboutDialogInfo aboutDialogInfo;
	
	aboutDialogInfo.SetName( "Blob" );
	aboutDialogInfo.SetVersion( "1.0" );
	aboutDialogInfo.SetDescription( "This program is free software and distributed under the MIT license." );
	aboutDialogInfo.SetCopyright( "Copyright (C) 2017, Spencer T. Parkin (spencertparkin@gmail.com)" );
	//aboutDialogInfo.SetWebSite( "http://spencerparkin.github.io/Blob" );

	wxAboutBox( aboutDialogInfo );
}

void Frame::OnToggleFreeCam( wxCommandEvent& event )
{
	Camera* camera = wxGetApp().GetCamera();
	if( camera->mode != Camera::MODE_FREE_CAM )
		camera->mode = Camera::MODE_FREE_CAM;
	else
		camera->mode = Camera::MODE_FOLLOW_SUBJECT;
}

void Frame::OnUpdateUI( wxUpdateUIEvent& event )
{
	switch( event.GetId() )
	{
		case ID_FreeCam:
		{
			event.Check( wxGetApp().GetCamera()->mode == Camera::MODE_FREE_CAM ? true : false );
			break;
		}
		case ID_DrawForces:
		{
			event.Check( ( debugDrawFlags & DRAW_FORCES ) ? true : false );
			break;
		}
		case ID_DrawCollisionObjects:
		{
			event.Check( ( debugDrawFlags & DRAW_COLLISION_OBJECTS ) ? true : false );
			break;
		}
		case ID_InventoryPanelToggle:
		{
			event.Check( IsPanelInUse( "Inventory" ) );
			break;
		}
		case ID_ModifiersPanelToggle:
		{
			event.Check( IsPanelInUse( "Modifiers" ) );
			break;
		}
	}
}

void Frame::OnToggleDrawForces( wxCommandEvent& event )
{
	debugDrawFlags ^= DRAW_FORCES;
}

void Frame::OnToggleDrawCollisionObjects( wxCommandEvent& event )
{
	debugDrawFlags ^= DRAW_COLLISION_OBJECTS;
}

void Frame::OnExit( wxCommandEvent& event )
{
	timer.Stop();

	Close( true );
}

void Frame::OnTimer( wxTimerEvent& event )
{
	UpdateAllPanels();
}

void Frame::OnToggleInventoryPanel( wxCommandEvent& event )
{
	TogglePanel( "Inventory" );
}

void Frame::OnToggleModifiersPanel( wxCommandEvent& event )
{
	TogglePanel( "Modifiers" );
}

Canvas* Frame::GetCanvas( void )
{
	wxAuiPaneInfo* paneInfo = nullptr;
	if( !IsPanelInUse( "Canvas", &paneInfo ) )
		return nullptr;

	CanvasPanel* canvasPanel = ( CanvasPanel* )paneInfo->window;
	return canvasPanel->canvas;
}

bool Frame::IsPanelInUse( const wxString& panelTitle, wxAuiPaneInfo** foundPaneInfo /*= nullptr*/ )
{
	if( foundPaneInfo )
		*foundPaneInfo = nullptr;

	wxAuiPaneInfoArray& paneInfoArray = auiManager->GetAllPanes();
	for( int i = 0; i < ( signed )paneInfoArray.GetCount(); i++ )
	{
		wxAuiPaneInfo& paneInfo = paneInfoArray[i];
		if( paneInfo.name == panelTitle )
		{
			if( foundPaneInfo )
				*foundPaneInfo = &paneInfo;
			return paneInfo.IsShown();
		}
	}

	return false;
}

void Frame::UpdateAllPanels( void )
{
	wxAuiPaneInfoArray& paneInfoArray = auiManager->GetAllPanes();
	for( int i = 0; i < ( signed )paneInfoArray.GetCount(); i++ )
	{
		wxAuiPaneInfo& paneInfo = paneInfoArray[i];
		Panel* panel = ( Panel* )paneInfo.window;
		panel->Update();
	}
}

bool Frame::TogglePanel( const wxString& panelTitle )
{
	wxAuiPaneInfo* foundPaneInfo = nullptr;
	if( IsPanelInUse( panelTitle, &foundPaneInfo ) )
	{
		Panel* panel = ( Panel* )foundPaneInfo->window;
		auiManager->DetachPane( panel );
		panel->Destroy();
		auiManager->Update();
	}
	else
	{
		Panel* panel = nullptr;

		if( panelTitle == "Canvas" )
			panel = new CanvasPanel();
		else if( panelTitle == "Inventory" )
			panel = new InventoryPanel();
		else if( panelTitle == "Modifiers" )
			panel = new ModifiersPanel();

		if( panel )
		{
			panel->Create( this );
			panel->CreateControls();

			wxAuiPaneInfo paneInfo;
			panel->SetupPaneInfo( paneInfo );
			paneInfo.CloseButton( true );
			paneInfo.Caption( panelTitle ).Name( panelTitle );
			paneInfo.Dockable().Show();
			paneInfo.DestroyOnClose();

			auiManager->AddPane( panel, paneInfo );
			auiManager->Update();
		}
	}

	return true;
}

// Frame.cpp