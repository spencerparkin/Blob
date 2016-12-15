// Frame.h

#pragma once

#include <wx/frame.h>
#include <wx/timer.h>

class Canvas;

class Frame : public wxFrame
{
public:

	Frame( void );
	virtual ~Frame( void );

	Canvas* GetCanvas( void ) { return canvas; }

private:

	enum
	{
		ID_Exit = wxID_HIGHEST,
		ID_FreeCam,
		ID_Timer,
	};

	void OnExit( wxCommandEvent& event );
	void OnTimer( wxTimerEvent& event );
	void OnToggleFreeCam( wxCommandEvent& event );
	void OnUpdateUI( wxUpdateUIEvent& event );

	Canvas* canvas;

	wxTimer timer;
};

// Frame.h
