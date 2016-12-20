// Frame.cpp

#include "Frame.h"
#include "Canvas.h"
#include "Application.h"
#include "Camera.h"
#include <wx/menu.h>
#include <wx/sizer.h>

Frame::Frame( void ) : wxFrame( 0, wxID_ANY, "Blob", wxDefaultPosition, wxSize( 700, 700 ) ), timer( this, ID_Timer )
{
	debugDrawFlags = 0;

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

	wxMenuBar* menuBar = new wxMenuBar();
	menuBar->Append( gameMenu, "Game" );
	SetMenuBar( menuBar );

	wxStatusBar* statusBar = new wxStatusBar( this );
	SetStatusBar( statusBar );

	canvas = new Canvas( this );

	wxBoxSizer* boxSizer = new wxBoxSizer( wxVERTICAL );
	boxSizer->Add( canvas, 1, wxALL | wxGROW, 0 );
	SetSizer( boxSizer );

	Bind( wxEVT_MENU, &Frame::OnExit, this, ID_Exit );
	Bind( wxEVT_MENU, &Frame::OnToggleFreeCam, this, ID_FreeCam );
	Bind( wxEVT_MENU, &Frame::OnToggleDrawForces, this, ID_DrawForces );
	Bind( wxEVT_MENU, &Frame::OnToggleDrawCollisionObjects, this, ID_DrawCollisionObjects );
	Bind( wxEVT_TIMER, &Frame::OnTimer, this, ID_Timer );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_FreeCam );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_DrawForces );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_DrawCollisionObjects );

	timer.Start(1);
}

/*virtual*/ Frame::~Frame( void )
{
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
	// It shouldn't hurt to call this more often than is needed.
	canvas->Refresh();
}

// Frame.cpp