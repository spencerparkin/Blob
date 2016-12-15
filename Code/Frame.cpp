// Frame.cpp

#include "Frame.h"
#include "Canvas.h"
#include "Application.h"
#include "Camera.h"
#include <wx/menu.h>
#include <wx/sizer.h>

Frame::Frame( void ) : wxFrame( 0, wxID_ANY, "Blob", wxDefaultPosition, wxSize( 700, 700 ) ), timer( this, ID_Timer )
{
	wxMenu* gameMenu = new wxMenu();
#ifdef _DEBUG
	wxMenuItem* toggleFreeCamMenuItem = new wxMenuItem( gameMenu, ID_FreeCam, "Free Cam", "Toggle the free-cam.", wxITEM_CHECK );
#endif
	wxMenuItem* exitMenuItem = new wxMenuItem( gameMenu, ID_Exit, "Exit", "Exit the program." );
#ifdef _DEBUG
	gameMenu->Append( toggleFreeCamMenuItem );
	gameMenu->AppendSeparator();
#endif
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
	Bind( wxEVT_TIMER, &Frame::OnTimer, this, ID_Timer );
	Bind( wxEVT_UPDATE_UI, &Frame::OnUpdateUI, this, ID_FreeCam );

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
	}
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