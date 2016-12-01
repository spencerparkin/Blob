// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Blob.h"

Application::Application( void )
{
	frame = nullptr;
	blob = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete blob;
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	blob = new Blob();
	blob->MakeRegularHexadron();

	frame = new Frame();
	frame->Show();

	return true;
}

wxIMPLEMENT_APP( Application );

// Application.cpp