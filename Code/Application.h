// Application.h

#pragma once

#include <wx/app.h>

class Frame;
class Blob;

class Application : public wxApp
{
public:

	Application( void );
	virtual ~Application( void );

	virtual bool OnInit( void ) override;

	Frame* frame;
	Blob* blob;
};

wxDECLARE_APP( Application );

// Application.h
