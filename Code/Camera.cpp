// Camera.cpp

#include "Camera.h"
#include "Controller.h"
#include "Application.h"
#include <wx/glcanvas.h>
#include <gl/GLU.h>

Camera::Camera( void )
{
	subject = nullptr;
	distanceToSubject = 10.0;
	theta = 0.0;
	phi = 0.0;
	viewAngle = M_PI / 3.0;
}

/*virtual*/ Camera::~Camera( void )
{
}

void Camera::Update( double currentTime )
{
	// TODO: If in free-cam mode, use the controller to free-cam around.
	//       If in follow mode, follow the configured subject.
}

void Camera::GetViewParameters( _3DMath::Vector& eye, _3DMath::Vector& subjectLocation, _3DMath::Vector& up ) const
{
	up.Set( 0.0, 1.0, 0.0 );

	subjectLocation.Set( 0.0, 0.0, 0.0 );
	if( subject )
		subject->GetLocation( subjectLocation );

	eye.x = cos( theta ) * cos( phi );
	eye.y = sin( phi );
	eye.z = sin( theta ) * cos( phi );
	eye.Scale( distanceToSubject );
	eye.Add( subjectLocation );
}

void Camera::GetViewTransform( _3DMath::AffineTransform& viewTransform ) const
{
	_3DMath::Vector eye, subjectLocation, up;
	GetViewParameters( eye, subjectLocation, up );

	viewTransform.linearTransform.zAxis.Subtract( eye, subjectLocation );
	viewTransform.linearTransform.zAxis.Normalize();
	viewTransform.linearTransform.xAxis.Cross( viewTransform.linearTransform.zAxis, up );
	viewTransform.linearTransform.xAxis.Normalize();
	viewTransform.linearTransform.yAxis.Cross( viewTransform.linearTransform.zAxis, viewTransform.linearTransform.xAxis );
	viewTransform.translation = eye;
}

void Camera::SetupOpenGLViewingMatrices( void )
{
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	double aspectRatio = double( viewport[2] ) / double( viewport[3] );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( viewAngle * 180.0 / M_PI, aspectRatio, 0.1, 1000.0 );

	_3DMath::Vector eye, subjectLocation, up;
	GetViewParameters( eye, subjectLocation, up );

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	gluLookAt( eye.x, eye.y, eye.z, subjectLocation.x, subjectLocation.y, subjectLocation.z, up.x, up.y, up.z );
}

// Camera.cpp