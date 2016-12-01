// Canvas.cpp

#include "Canvas.h"
#include "Application.h"
#include "GLRenderer.h"
#include "Blob.h"
#include "Camera.h"

int Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

Canvas::Canvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	context = nullptr;
	renderer = new GLRenderer();
	camera = new Camera();

	Bind( wxEVT_PAINT, &Canvas::OnPaint, this );
	Bind( wxEVT_SIZE, &Canvas::OnSize, this );
}

/*virtual*/ Canvas::~Canvas( void )
{
	delete context;
	delete renderer;
	delete camera;
}

void Canvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

void Canvas::OnPaint( wxPaintEvent& event )
{
	BindContext();

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	wxGetApp().blob->Render( *renderer );

	glFlush();

	SwapBuffers();
}

void Canvas::OnSize( wxSizeEvent& event )
{
	wxSize size = event.GetSize();
	glViewport( 0, 0, size.GetWidth(), size.GetHeight() );

	Refresh();
}

// Canvas.cpp