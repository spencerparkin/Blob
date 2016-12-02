// Blob.cpp

#include "Blob.h"
#include "Texture.h"

using namespace _3DMath;

Blob::Blob( void )
{
	texture = nullptr;
}

/*virtual*/ Blob::~Blob( void )
{
	delete texture;
}

void Blob::Render( _3DMath::Renderer& renderer )
{
	if( !texture )
	{
		texture = new Texture();
		texture->Load( "Textures/Globe.png" );
	}

	texture->Bind();

	renderer.DrawTriangleMesh( triangleMesh );
}

/*virtual*/ void Blob::GetLocation( _3DMath::Vector& location ) const
{
	location = particleSystem.centerOfMass;
}

void Blob::MakePolyhedron( Polyhedron polyhedron )
{
	triangleMesh.Clear();
	particleSystem.Clear();

	switch( polyhedron )
	{
		case HEXADRON:
		{
			MakeSpring( Vector( 1.0, 1.0, 1.0 ) );
			MakeSpring( Vector( -1.0, 1.0, 1.0 ) );
			MakeSpring( Vector( 1.0, -1.0, 1.0 ) );
			MakeSpring( Vector( -1.0, -1.0, 1.0 ) );

			MakeSpring( 0, 2 );
			MakeSpring( 2, 6 );
			MakeSpring( 6, 4 );
			MakeSpring( 4, 0 );

			MakeSpring( 7, 5 );
			MakeSpring( 5, 1 );
			MakeSpring( 1, 3 );
			MakeSpring( 3, 7 );

			MakeSpring( 0, 7 );
			MakeSpring( 2, 5 );
			MakeSpring( 6, 1 );
			MakeSpring( 4, 3 );

			break;
		}
		case ICOSAHEDRON:
		{
			MakeSpring( Vector( 0.0, 1.0, PHI ) );
			MakeSpring( Vector( 0.0, -1.0, PHI ) );

			MakeSpring( Vector( 1.0, PHI, 0.0 ) );
			MakeSpring( Vector( -1.0, PHI, 0.0 ) );

			MakeSpring( Vector( PHI, 0.0, 1.0 ) );
			MakeSpring( Vector( PHI, 0.0, -1.0 ) );

			break;
		}
		case DODECAHEDRON:
		{
			MakeSpring( Vector( 1.0, 1.0, 1.0 ) );
			MakeSpring( Vector( -1.0, 1.0, 1.0 ) );
			MakeSpring( Vector( 1.0, -1.0, 1.0 ) );
			MakeSpring( Vector( -1.0, -1.0, 1.0 ) );

			MakeSpring( Vector( 0.0, 1.0 / PHI, PHI ) );
			MakeSpring( Vector( 0.0, -1.0 / PHI, PHI ) );

			MakeSpring( Vector( 1.0 / PHI, PHI, 0.0 ) );
			MakeSpring( Vector( -1.0 / PHI, PHI, 0.0 ) );

			MakeSpring( Vector( PHI, 0.0, 1.0 / PHI ) );
			MakeSpring( Vector( PHI, 0.0, -1.0 / PHI ) );

			break;
		}
	}

	triangleMesh.FindConvexHull();
	triangleMesh.CalculateNormals();
	triangleMesh.CalculateSphericalUVs();
}

void Blob::MakeSpring( const Vector& vector )
{
	Vector negativeVector;
	vector.GetNegated( negativeVector );

	Vertex vertex;
	
	vertex.position = vector;
	triangleMesh.vertexArray->push_back( vertex );

	vertex.position = negativeVector;
	triangleMesh.vertexArray->push_back( vertex );

	int index0 = triangleMesh.vertexArray->size() - 2;
	int index1 = triangleMesh.vertexArray->size() - 1;

	MakeSpring( index0, index1 );
}

void Blob::MakeSpring( int index0, int index1 )
{
	Vector position0, position1;
	triangleMesh.GetVertexPosition( index0, position0 );
	triangleMesh.GetVertexPosition( index1, position1 );

	Vector vector;
	vector.Subtract( position1, position0 );
	double length = vector.Length();

	ParticleSystem::SpringForce* springForce = new ParticleSystem::SpringForce();
	springForce->endPointParticleIds[0] = index0;
	springForce->endPointParticleIds[1] = index1;
	springForce->equilibriumLength = length;
	springForce->stiffness = 1.0;

	particleSystem.forceCollection.AddObject( springForce );
}

// Blob.cpp