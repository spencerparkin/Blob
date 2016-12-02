// Canvas.h

#pragma once

#include <wx/glcanvas.h>

class GLRenderer;
class Camera;

class Canvas : public wxGLCanvas
{
public:

	static int attributeList[];

	Canvas( wxWindow* parent );
	virtual ~Canvas( void );

	void Advance( void );

private:

	void OnPaint( wxPaintEvent& event );
	void OnSize( wxSizeEvent& event );

	void BindContext( void );

	wxGLContext* context;
	GLRenderer* renderer;
	Camera* camera;
	double baseTime;
};

// Canvas.h
