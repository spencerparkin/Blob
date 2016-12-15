// Canvas.h

#pragma once

#include <wx/glcanvas.h>
#include <TimeKeeper.h>

class GLRenderer;
class Camera;

class Canvas : public wxGLCanvas
{
public:

	static int attributeList[];

	Canvas( wxWindow* parent );
	virtual ~Canvas( void );

	void Advance( void );

	Camera* GetCamera( void ) { return camera; }

private:

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );

	void BindContext( void );

	wxGLContext* context;
	GLRenderer* renderer;
	Camera* camera;
	_3DMath::TimeKeeper timeKeeper;
};

// Canvas.h
