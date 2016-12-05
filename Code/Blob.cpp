// Blob.cpp

#include "Blob.h"
#include "Texture.h"
#include "Driver.h"

using namespace _3DMath;

Blob::Blob( void )
{
	texture = nullptr;
	driver = nullptr;

	maxTorque = 50.0;
}

/*virtual*/ Blob::~Blob( void )
{
	delete texture;
	delete driver;
}

void Blob::Render( _3DMath::Renderer& renderer )
{
	if( !texture )
	{
		texture = new Texture();
		texture->Load( "Textures/Globe.png" );
	}

	texture->Bind();

	renderer.DrawTriangleMesh( triangleMesh, Renderer::UV_CORRECTION );

	//renderer.DrawParticleSystem( particleSystem, Renderer::DRAW_FORCES );
}

void Blob::Simulate( double currentTime )
{
	if( driver )
		driver->Drive( this );

	particleSystem.Simulate( currentTime );
}

/*virtual*/ void Blob::GetLocation( _3DMath::Vector& location ) const
{
	location = particleSystem.centerOfMass;
}

void Blob::MakePolyhedron( Polyhedron polyhedron, bool subDivide )
{
	triangleMesh.Clear();
	particleSystem.Clear();

	double radius = 1.0;

	switch( polyhedron )
	{
		case HEXADRON:
		{
			radius = sqrt( 3.0 );

			AddVertexPair( Vector( 1.0, 1.0, 1.0 ) );
			AddVertexPair( Vector( -1.0, 1.0, 1.0 ) );
			AddVertexPair( Vector( 1.0, -1.0, 1.0 ) );
			AddVertexPair( Vector( -1.0, -1.0, 1.0 ) );

			break;
		}
		case ICOSAHEDRON:
		{
			radius = sqrt( 1.0 + PHI * PHI );

			AddVertexPair( Vector( 0.0, 1.0, PHI ) );
			AddVertexPair( Vector( 0.0, -1.0, PHI ) );

			AddVertexPair( Vector( 1.0, PHI, 0.0 ) );
			AddVertexPair( Vector( -1.0, PHI, 0.0 ) );

			AddVertexPair( Vector( PHI, 0.0, 1.0 ) );
			AddVertexPair( Vector( PHI, 0.0, -1.0 ) );

			break;
		}
		case DODECAHEDRON:
		{
			radius = sqrt( 3.0 );

			AddVertexPair( Vector( 1.0, 1.0, 1.0 ) );
			AddVertexPair( Vector( -1.0, 1.0, 1.0 ) );
			AddVertexPair( Vector( 1.0, -1.0, 1.0 ) );
			AddVertexPair( Vector( -1.0, -1.0, 1.0 ) );

			AddVertexPair( Vector( 0.0, 1.0 / PHI, PHI ) );
			AddVertexPair( Vector( 0.0, -1.0 / PHI, PHI ) );

			AddVertexPair( Vector( 1.0 / PHI, PHI, 0.0 ) );
			AddVertexPair( Vector( -1.0 / PHI, PHI, 0.0 ) );

			AddVertexPair( Vector( PHI, 0.0, 1.0 / PHI ) );
			AddVertexPair( Vector( PHI, 0.0, -1.0 / PHI ) );

			break;
		}
	}

	triangleMesh.FindConvexHull();

	if( subDivide )
		triangleMesh.SubdivideAllTriangles( radius );

	triangleMesh.CalculateNormals();
	triangleMesh.CalculateSphericalUVs();

	std::vector< int > particleIds;

	for( int i = 0; i < ( int )triangleMesh.vertexArray->size(); i++ )
	{
		ParticleSystem::MeshVertexParticle* particle = new ParticleSystem::MeshVertexParticle();
		particle->index = i;
		particle->mesh = &triangleMesh;

		particleSystem.particleCollection.AddObject( particle );

		particleIds.push_back( particle->id );
	}

	TriangleMesh::EdgeSet edgeSet;
	triangleMesh.GenerateEdgeSet( edgeSet );

	double stiffness = 100.0;

	for( TriangleMesh::EdgeSet::iterator iter = edgeSet.begin(); iter != edgeSet.end(); iter++ )
	{
		uint64_t edgePair = *iter;

		int index0, index1;
		TriangleMesh::GetEdgePair( edgePair, index0, index1 );

		MakeSpring( index0, index1, particleIds, stiffness );
	}

	edgeSet.clear();

	for( int i = 0; i < ( int )triangleMesh.vertexArray->size(); i++ )
	{
		Vertex& vertex = ( *triangleMesh.vertexArray )[i];

		Vector negatedPosition;
		vertex.position.GetNegated( negatedPosition );

		int j = triangleMesh.FindIndex( negatedPosition );
		if( j >= 0 )
		{
			uint64_t edgePair;
			TriangleMesh::SetEdgePair( edgePair, i, j );

			TriangleMesh::EdgeSet::iterator iter = edgeSet.find( edgePair );
			if( iter == edgeSet.end() )
			{
				MakeSpring( i, j, particleIds, stiffness );
				edgeSet.insert( edgePair );
			}
		}
	}

	ParticleSystem::GravityForce* gravityForce = new ParticleSystem::GravityForce( &particleSystem );
	gravityForce->accelDueToGravity.Set( 0.0, -9.8, 0.0 );
	particleSystem.forceCollection.AddObject( gravityForce );

	ParticleSystem::CollisionPlane* collisionPlane = new ParticleSystem::CollisionPlane();
	collisionPlane->plane.SetCenterAndNormal( Vector( 0.0, -2.5, 0.0 ), Vector( 0.0, 1.0, 0.0 ) );
	collisionPlane->friction = 1.0;
	particleSystem.collisionObjectCollection.AddObject( collisionPlane );
}

void Blob::AddVertexPair( const Vector& vector )
{
	Vector negatedVector;
	vector.GetNegated( negatedVector );

	Vertex vertex;
	
	vertex.position = vector;
	triangleMesh.vertexArray->push_back( vertex );

	vertex.position = negatedVector;
	triangleMesh.vertexArray->push_back( vertex );
}

void Blob::MakeSpring( int index0, int index1, std::vector< int >& particleIds, double stiffness )
{
	Vector position0, position1;

	ParticleSystem::Particle* particle0 = ( ParticleSystem::Particle* )particleSystem.particleCollection.FindObject( particleIds[ index0 ] );
	particle0->GetPosition( position0 );

	ParticleSystem::Particle* particle1 = ( ParticleSystem::Particle* )particleSystem.particleCollection.FindObject( particleIds[ index1 ] );
	particle1->GetPosition( position1 );

	Vector vector;
	vector.Subtract( position1, position0 );
	double length = vector.Length();

	ParticleSystem::SpringForce* springForce = new ParticleSystem::SpringForce( &particleSystem );
	springForce->endPointParticleIds[0] = particle0->id;
	springForce->endPointParticleIds[1] = particle1->id;
	springForce->equilibriumLength = length;
	springForce->stiffness = stiffness;

	particleSystem.forceCollection.AddObject( springForce );
}

// Blob.cpp