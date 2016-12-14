// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Controller.h"
#include "Race.h"
#include "Canvas.h"
#include <wx/image.h>

Application::Application( void )
{
	frame = nullptr;
	controller = nullptr;
	race = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete controller;
	delete race;
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

	frame = new Frame();
	frame->Show();

	return true;
}

Camera* Application::GetCamera( void )
{
	return frame->GetCanvas()->GetCamera();
}

wxIMPLEMENT_APP( Application );

// Application.cpp