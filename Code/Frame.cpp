// Frame.cpp

#include "Frame.h"
#include "Canvas.h"
#include <wx/menu.h>
#include <wx/sizer.h>

Frame::Frame( void ) : wxFrame( 0, wxID_ANY, "Blob", wxDefaultPosition, wxSize( 700, 700 ) )
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
}

/*virtual*/ Frame::~Frame( void )
{
}

void Frame::OnExit( wxCommandEvent& event )
{
	Close( true );
}

// Frame.cpp