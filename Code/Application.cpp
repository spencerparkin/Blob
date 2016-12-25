// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Controller.h"
#include "Stage.h"
#include "Canvas.h"
#include "Sound.h"
#include "Message.h"
#include "ModelCache.h"
#include <wx/image.h>

Application::Application( void )
{
	frame = nullptr;
	controller = nullptr;
	stage = nullptr;
	sound = nullptr;
	messageSystem = nullptr;
	modelCache = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete controller;
	delete stage;
	delete sound;
	delete messageSystem;
	delete modelCache;
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	wxInitAllImageHandlers();

	messageSystem = new MessageSystem();
	modelCache = new ModelCache();
	sound = new Sound();

	// TODO: Add way to redetect controller input type from the menu-bar.
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