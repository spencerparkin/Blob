// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Controller.h"
#include "Stage.h"
#include "Canvas.h"
#include "Sound.h"
#include "Message.h"
#include <wx/image.h>

Application::Application( void )
{
	frame = nullptr;
	controller = nullptr;
	stage = nullptr;
	sound = nullptr;
	messageSystem = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete controller;
	delete stage;
	delete sound;
	delete messageSystem;
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	wxInitAllImageHandlers();

	messageSystem = new MessageSystem();

	sound = new Sound();

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