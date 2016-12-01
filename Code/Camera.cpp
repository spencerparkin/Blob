// Camera.cpp

#include "Camera.h"

Camera::Camera( void )
{
	subjectPosition.Set( 0.0, 0.0, 0.0 );
	followingDistance = 10.0;
	followingHoverHeight = 5.0;
	viewTransform.linearTransform.Identity();
	viewTransform.translation.Set( 0.0, 0.0, 0.0 );
}

/*virtual*/ Camera::~Camera( void )
{
}

void Camera::TrackSubject( double currentTime )
{
}

void Camera::SetupOpenGLViewingMatrices( void )
{
}

// Camera.cpp