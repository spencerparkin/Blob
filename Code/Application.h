// Application.h

#pragma once

#include <wx/app.h>

class Frame;
class Controller;
class Race;
class Camera;

class Application : public wxApp
{
public:

	Application( void );
	virtual ~Application( void );

	virtual bool OnInit( void ) override;

	Camera* GetCamera( void );

	Frame* frame;
	Controller* controller;
	Race* race;
};

wxDECLARE_APP( Application );

// Application.h
