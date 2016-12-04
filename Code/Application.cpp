// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Blob.h"
#include "Controller.h"
#include <wx/image.h>

Application::Application( void )
{
	frame = nullptr;
	blob = nullptr;
	controller = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete blob;
	delete controller;
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	wxInitAllImageHandlers();

	controller = new XboxController();
	if( !controller->SetupAndConnect() )
	{
		delete controller;

		controller = new JoyStickController();
		if( !controller->SetupAndConnect() )
		{
			delete controller;

			controller = new KeyboardMouseController();
			if( !controller->SetupAndConnect() )
				return false;
		}
	}

	blob = new Blob();
	blob->MakePolyhedron( Blob::HEXADRON );

	frame = new Frame();
	frame->Show();

	return true;
}

wxIMPLEMENT_APP( Application );

// Application.cpp