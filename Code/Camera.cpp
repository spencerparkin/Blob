// Camera.cpp

#include "Camera.h"
#include "Application.h"
#include "Controller.h"
#include <wx/glcanvas.h>
#include <gl/GLU.h>

Camera::Camera( void )
{
	subject = nullptr;
	distanceToSubject = 10.0;
	theta = 0.0;
	phi = 0.0;
	viewAngle = M_PI / 3.0;
	lastUpdateTime = 0.0;
	zoomRate = 0.01;
	thetaChangeRate = M_PI / 2000.0;
	phiChangeRate = M_PI / 2000.0;
}

/*virtual*/ Camera::~Camera( void )
{
}

void Camera::Update( double currentTime )
{
	double deltaTime = 0.0;
	if( lastUpdateTime != 0.0 )
		deltaTime = currentTime - lastUpdateTime;
	if( deltaTime > 100.0 )
		deltaTime = 100.0;
	lastUpdateTime = currentTime;

	Controller* controller = wxGetApp().controller;

	// TODO: Probably need to revisit this and rework the design.  There's surely a better way to control the camera.

	double zoomDelta = 0.0;
	if( controller->ButtonDown( Controller::BUTTON_L_SHOULDER ) )
		zoomDelta += zoomRate * deltaTime;
	if( controller->ButtonDown( Controller::BUTTON_R_SHOULDER ) )
		zoomDelta -= zoomRate * deltaTime;

	distanceToSubject += zoomDelta;

	_3DMath::Vector unitDir;
	double mag;
	controller->GetAnalogJoyStick( Controller::LEFT_SIDE, unitDir, mag );

	theta += thetaChangeRate * deltaTime * unitDir.x * mag;
	phi += phiChangeRate * deltaTime * unitDir.y * mag;
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