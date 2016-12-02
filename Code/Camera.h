// Camera.h

#pragma once

#include <Vector.h>

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
