// GLRenderer.h

#pragma once

#include <Renderer.h>

class GLRenderer : public _3DMath::Renderer
{
public:

	GLRenderer( void );
	virtual ~GLRenderer( void );

	virtual void BeginDrawMode( DrawMode drawMode ) override;
	virtual void EndDrawMode( void ) override;
	virtual void IssueVertex( const _3DMath::Vertex& vertex ) override;
};

// GLRenderer.h
