// Frame.cpp

#include "Frame.h"
#include "Canvas.h"
#include <wx/menu.h>
#include <wx/sizer.h>

Frame::Frame( void ) : wxFrame( 0, wxID_ANY, "Blob", wxDefaultPosition, wxSize( 700, 700 ) ), timer( this, ID_Timer )
{
	wxMenu* gameMenu = new wxMenu();
	wxMenuItem* exitMenuItem = new wxMenuItem( gameMenu, ID_Exit, "Exit", "Exit the program." );
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
	Bind( wxEVT_TIMER, &Frame::OnTimer, this, ID_Timer );

	timer.Start(1);
}

/*virtual*/ Frame::~Frame( void )
{
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