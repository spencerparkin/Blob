// Application.cpp

#include "Application.h"
#include "Frame.h"
#include "Controller.h"
#include "Stage.h"
#include "Canvas.h"
#include "Sound.h"
#include <wx/image.h>

Application::Application( void )
{
	frame = nullptr;
	controller = nullptr;
	stage = nullptr;
	sound = nullptr;
}

/*virtual*/ Application::~Application( void )
{
	delete controller;
	delete stage;
	
	if( sound )
		sound->Shutdown();

	delete sound;
}

/*virtual*/ bool Application::OnInit( void )
{
	if( !wxApp::OnInit() )
		return false;

	wxInitAllImageHandlers();

	sound = new Sound();
	if( !sound->Setup() )
	{
	}

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