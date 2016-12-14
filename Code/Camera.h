// Camera.h

#pragma once

#include <Vector.h>
#include <AffineTransform.h>

class Camera
{
public:

	Camera( void );
	virtual ~Camera( void );
	
	class Subject
	{
	public:
		virtual void GetLocation( _3DMath::Vector& location ) const = 0;
	};

	void Update( double currentTime );
	void SetupOpenGLViewingMatrices( void );

	enum Mode
	{
		MODE_FREE_CAM,
		MODE_FOLLOW_SUBJECT,
	};

	Mode mode;
	Subject* subject;
	double followingDistance;
	double hoverHeight;
	double viewAngle;
	_3DMath::AffineTransform viewTransform;
	double maxStrafeSpeed;
	double maxRotateSpeed;
	double lastTime;
};

// Camera.h
