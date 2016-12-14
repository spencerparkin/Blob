// Camera.cpp

#include "Camera.h"
#include "Controller.h"
#include "Application.h"
#include <wx/glcanvas.h>
#include <gl/GLU.h>

Camera::Camera( void )
{
	mode = MODE_FREE_CAM;
	subject = nullptr;
	followingDistance = 5.0;
	hoverHeight = 2.0;
	viewAngle = M_PI / 3.0;
	viewTransform.linearTransform.Identity();
	viewTransform.translation.Set( 0.0, 5.0, 10.0 );
	maxStrafeSpeed = 5.0;
	maxRotateSpeed = M_PI / 3.0;
	lastTime = 0.0;
}

/*virtual*/ Camera::~Camera( void )
{
}

void Camera::Update( double currentTime )
{
	if( lastTime == 0.0 )
		lastTime = currentTime;
	double deltaTime = ( currentTime - lastTime ) / 1000.0;
	deltaTime = 0.017;		// debug...
	lastTime = currentTime;

	switch( mode )
	{
		case MODE_FREE_CAM:
		{
			Controller* controller = wxGetApp().controller;

			_3DMath::Vector unitDir;
			double mag;

			controller->GetAnalogJoyStick( Controller::LEFT_SIDE, unitDir, mag );

			double maxStrafeDelta = maxStrafeSpeed * deltaTime;

			_3DMath::Vector strafeDelta( 0.0, 0.0, 0.0 );
			strafeDelta.AddScale( viewTransform.linearTransform.xAxis, unitDir.x * mag * maxStrafeDelta );
			strafeDelta.AddScale( viewTransform.linearTransform.zAxis, -unitDir.y * mag * maxStrafeDelta );

			if( controller->ButtonDown( Controller::BUTTON_R_SHOULDER ) )
				strafeDelta.AddScale( _3DMath::Vector( 0.0, 1.0, 0.0 ), maxStrafeDelta );

			double triggerValue = 0.0;
			controller->GetAnalogTrigger( Controller::RIGHT_SIDE, triggerValue );
			if( triggerValue > 0.0 )
				strafeDelta.AddScale( _3DMath::Vector( 0.0, 1.0, 0.0 ), -maxStrafeDelta );

			viewTransform.translation.Add( strafeDelta );

			controller->GetAnalogJoyStick( Controller::RIGHT_SIDE, unitDir, mag );

			double maxRotateAngle = maxRotateSpeed * deltaTime;

			double yAxisRotationAngle = -unitDir.x * mag * maxRotateAngle;
			double xAxisRotationAngle = unitDir.y * mag * maxRotateAngle;

			viewTransform.linearTransform.yAxis.Rotate( viewTransform.linearTransform.xAxis, xAxisRotationAngle );
			viewTransform.linearTransform.zAxis.Rotate( viewTransform.linearTransform.xAxis, xAxisRotationAngle );

			viewTransform.linearTransform.xAxis.Rotate( _3DMath::Vector( 0.0, 1.0, 0.0 ), yAxisRotationAngle );
			viewTransform.linearTransform.zAxis.Rotate( _3DMath::Vector( 0.0, 1.0, 0.0 ), yAxisRotationAngle );

			viewTransform.linearTransform.Orthogonalize();

			break;
		}
		case MODE_FOLLOW_SUBJECT:
		{
			if( !subject )
				break;

			_3DMath::AffineTransform targetTransform;
			subject->GetLocation( targetTransform.translation );

			// TODO: Track last move direction of subject and use that instead of this to build our frame.
			_3DMath::Vector targetToCamera;
			targetToCamera.Subtract( viewTransform.translation, targetTransform.translation );

			targetTransform.linearTransform.yAxis.Set( 0.0, 1.0, 0.0 );
			targetTransform.linearTransform.zAxis.Cross( targetToCamera, targetTransform.linearTransform.yAxis );
			targetTransform.linearTransform.zAxis.Normalize();
			targetTransform.linearTransform.xAxis.Cross( targetTransform.linearTransform.yAxis, targetTransform.linearTransform.zAxis );

			_3DMath::Vector targetCameraPosition( sqrt( followingDistance * followingDistance - hoverHeight * hoverHeight ), hoverHeight, 0.0 );
			targetTransform.Transform( targetCameraPosition );

			_3DMath::Vector cameraToDesiredLocation;
			cameraToDesiredLocation.Subtract( targetCameraPosition, viewTransform.translation );
			
			double distance = cameraToDesiredLocation.Length();
			if( distance > 0.0 )
			{
				double cameraMoveSpeed = 1.0 * distance;
				double cameraMoveDistance = cameraMoveSpeed * deltaTime;

				_3DMath::Vector cameraMoveDirection;
				cameraMoveDirection.SetNormalized( cameraToDesiredLocation );

				_3DMath::Vector cameraMoveDelta;
				cameraMoveDelta.SetScaled( cameraMoveDirection, cameraMoveDistance );

				viewTransform.translation.Add( cameraMoveDelta );
			}

			targetToCamera.Subtract( viewTransform.translation, targetTransform.translation );

			viewTransform.linearTransform.zAxis.SetNormalized( targetToCamera );
			viewTransform.linearTransform.xAxis.Cross( _3DMath::Vector( 0.0, 1.0, 0.0 ), viewTransform.linearTransform.zAxis );
			viewTransform.linearTransform.xAxis.Normalize();
			viewTransform.linearTransform.yAxis.Cross( viewTransform.linearTransform.zAxis, viewTransform.linearTransform.xAxis );

			break;
		}
	}
}

void Camera::SetupOpenGLViewingMatrices( void )
{
	GLint viewport[4];
	glGetIntegerv( GL_VIEWPORT, viewport );

	double aspectRatio = double( viewport[2] ) / double( viewport[3] );

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( viewAngle * 180.0 / M_PI, aspectRatio, 0.1, 1000.0 );

	_3DMath::AffineTransform viewTransformInverse;
	viewTransform.GetTranspose( viewTransformInverse );

	double viewMatrix[16];

	viewMatrix[0] = viewTransformInverse.linearTransform.xAxis.x;
	viewMatrix[1] = viewTransformInverse.linearTransform.xAxis.y;
	viewMatrix[2] = viewTransformInverse.linearTransform.xAxis.z;
	viewMatrix[3] = 0.0;

	viewMatrix[4] = viewTransformInverse.linearTransform.yAxis.x;
	viewMatrix[5] = viewTransformInverse.linearTransform.yAxis.y;
	viewMatrix[6] = viewTransformInverse.linearTransform.yAxis.z;
	viewMatrix[7] = 0.0;

	viewMatrix[8] = viewTransformInverse.linearTransform.zAxis.x;
	viewMatrix[9] = viewTransformInverse.linearTransform.zAxis.y;
	viewMatrix[10] = viewTransformInverse.linearTransform.zAxis.z;
	viewMatrix[11] = 0.0;

	viewMatrix[12] = viewTransformInverse.translation.x;
	viewMatrix[13] = viewTransformInverse.translation.y;
	viewMatrix[14] = viewTransformInverse.translation.z;
	viewMatrix[15] = 1.0;

	glMatrixMode( GL_MODELVIEW );
	glLoadMatrixd( viewMatrix );
}

// Camera.cpp