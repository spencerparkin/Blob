// Canvas.cpp

#include "Canvas.h"
#include "Application.h"
#include "GLRenderer.h"
#include "Stage.h"
#include "Camera.h"
#include "Controller.h"
#include "Message.h"

int Canvas::attributeList[] = { WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0 };

Canvas::Canvas( wxWindow* parent ) : wxGLCanvas( parent, wxID_ANY, attributeList )
{
	timeKeeper.fixedDeltaTime = 50.0;

	context = nullptr;
	renderer = new GLRenderer();
	camera = new Camera();

	Bind( wxEVT_PAINT, &Canvas::OnPaint, this );
	Bind( wxEVT_SIZE, &Canvas::OnSize, this );

	// TODO: Figure this out.
	//MessageSystem::MessageHandler messageHandler = std::bind( &Canvas::StageCompleteMessageHandler, *this, std::placeholders::_1 );
	//wxGetApp().messageSystem->RegisterMessageHandler( "StageComplete", messageHandler );
}

/*virtual*/ Canvas::~Canvas( void )
{
	delete context;
	delete renderer;
	delete camera;
}

void Canvas::StageCompleteMessageHandler( Message* message )
{
}

void Canvas::BindContext( void )
{
	if( !context )
		context = new wxGLContext( this );

	SetCurrent( *context );
}

void Canvas::Advance( void )
{
	timeKeeper.MarkCurrentTime();

	if( !wxGetApp().stage )
	{
		// TODO: Remember where the user left off and start from there.
		wxGetApp().stage = new Stage();
		wxGetApp().stage->Load( "Data/Stage1.xml" );
	}

	wxGetApp().controller->UpdateState();

	camera->Update( timeKeeper );

	wxGetApp().stage->Simulate( timeKeeper );
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

	wxGetApp().stage->Render( *renderer, timeKeeper );

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