// Application.h

#pragma once

#include <wx/app.h>

class Frame;
class Blob;
class Controller;

class Application : public wxApp
{
public:

	Application( void );
	virtual ~Application( void );

	virtual bool OnInit( void ) override;

	Frame* frame;
	Blob* blob;
	Controller* controller;
};

wxDECLARE_APP( Application );

// Application.h
