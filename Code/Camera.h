// Camera.h

#pragma once

#include <Vector.h>
#include <AffineTransform.h>
#include <wx/glcanvas.h>

class Camera
{
public:

	Camera( void );
	virtual ~Camera( void );
	
	void TrackSubject( double currentTime );
	void SetupOpenGLViewingMatrices( void );

	_3DMath::Vector subjectPosition;
	double followingDistance;
	double followingHoverHeight;

	_3DMath::AffineTransform viewTransform;
	double viewAngle;
};

// Camera.h
