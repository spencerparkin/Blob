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

	void GetViewParameters( _3DMath::Vector& eye, _3DMath::Vector& subjectLocation, _3DMath::Vector& up ) const;
	void GetViewTransform( _3DMath::AffineTransform& viewTransform ) const;

	Subject* subject;
	double distanceToSubject;
	double theta, phi;
	double viewAngle;
	double lastUpdateTime;
	double zoomRate;
	double thetaChangeRate;
	double phiChangeRate;
};

// Camera.h
