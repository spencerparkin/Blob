// Canvas.cpp

#include "Canvas.h"
#include "Application.h"
#include "GLRenderer.h"
#include "Race.h"
#include "Camera.h"
#include "Controller.h"
#include <wx/time.h>

int Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

Canvas::Canvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	baseTime = wxGetLocalTimeMillis().ToDouble();

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

void Canvas::Advance( void )
{
	if( !wxGetApp().race )
	{
		wxGetApp().race = new Race();
		wxGetApp().race->Load( "Data/Track1.xml" );
	}

	wxGetApp().controller->UpdateState();

	double currentTime = wxGetLocalTimeMillis().ToDouble() - baseTime;

	camera->Update( currentTime );

	wxGetApp().race->Simulate( currentTime );
}

void Canvas::OnPaint( wxPaintEvent& event )
{
	BindContext();

	Advance();

	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
	glEnable( GL_CULL_FACE );
	glFrontFace( GL_CCW );
	glCullFace( GL_BACK );

	camera->SetupOpenGLViewingMatrices();

	wxGetApp().race->Render( *renderer );

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