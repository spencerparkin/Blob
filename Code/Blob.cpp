// Blob.cpp

#include "Blob.h"
#include "Texture.h"
#include "Driver.h"

using namespace _3DMath;

Blob::Blob( void )
{
	texture = nullptr;
	driver = nullptr;

	maxTorque = 5.0;
	axleAngle = 0.0;
	maxTurnRate = M_PI / 4.0;
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
		texture->Load( "Data/Globe.png" );
	}

	texture->Bind();

	renderer.DrawTriangleMesh( triangleMesh, Renderer::UV_CORRECTION );

	//renderer.DrawParticleSystem( particleSystem, Renderer::DRAW_FORCES );
}

void Blob::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	if( driver )
		driver->Drive( this, timeKeeper );

	particleSystem.Simulate( timeKeeper );
}

void Blob::GetAxleAxis( _3DMath::Vector& unitAxleAxis ) const
{
	unitAxleAxis.Set( cos( axleAngle ), 0.0, sin( axleAngle ) );
}

/*virtual*/ void Blob::GetLocation( _3DMath::Vector& location ) const
{
	location = particleSystem.centerOfMass;
}

void Blob::MakePolyhedron( Polyhedron polyhedron, bool subDivide, const _3DMath::AffineTransform& transform )
{
	triangleMesh.Clear();
	particleSystem.Clear();

	double radius = 1.0;

	switch( polyhedron )
	{
		case HEXADRON:
		{
			radius = sqrt( 3.0 );

			AddSymmetricVertices( Vector( 1.0, 1.0, 1.0 ) );

			break;
		}
		case ICOSAHEDRON:
		{
			radius = sqrt( 1.0 + PHI * PHI );

			AddSymmetricVertices( Vector( 0.0, 1.0, PHI ) );
			AddSymmetricVertices( Vector( 1.0, PHI, 0.0 ) );
			AddSymmetricVertices( Vector( PHI, 0.0, 1.0 ) );

			break;
		}
		case DODECAHEDRON:
		{
			radius = sqrt( 3.0 );

			AddSymmetricVertices( Vector( 1.0, 1.0, 1.0 ) );
			AddSymmetricVertices( Vector( 0.0, 1.0 / PHI, PHI ) );
			AddSymmetricVertices( Vector( 1.0 / PHI, PHI, 0.0 ) );
			AddSymmetricVertices( Vector( PHI, 0.0, 1.0 / PHI ) );

			break;
		}
	}

	triangleMesh.FindConvexHull();

	if( subDivide )
		triangleMesh.SubdivideAllTriangles( radius );

	triangleMesh.CalculateSphericalUVs();
	triangleMesh.Transform( transform );
	triangleMesh.CalculateNormals();

	std::vector< int > particleIds;

	for( int i = 0; i < ( int )triangleMesh.vertexArray->size(); i++ )
	{
		ParticleSystem::MeshVertexParticle* particle = new ParticleSystem::MeshVertexParticle();
		particle->index = i;
		particle->mesh = &triangleMesh;
		particle->mass = 1.0;

		particleSystem.particleCollection.AddObject( particle );

		particle->GetPosition( particle->previousPosition );

		particleIds.push_back( particle->id );
	}

	TriangleMesh::EdgeSet edgeSet;
	triangleMesh.GenerateEdgeSet( edgeSet );

	double stiffness = 60.0;

	for( TriangleMesh::EdgeSet::iterator iter = edgeSet.begin(); iter != edgeSet.end(); iter++ )
	{
		uint64_t edgePair = *iter;

		int index0, index1;
		TriangleMesh::GetEdgePair( edgePair, index0, index1 );

		MakeSpring( index0, index1, particleIds, stiffness );
	}

	edgeSet.clear();

	Vector center;
	triangleMesh.CalculateCenter( center );

	for( int i = 0; i < ( int )triangleMesh.vertexArray->size(); i++ )
	{
		Vertex& vertex = ( *triangleMesh.vertexArray )[i];

		Vector oppositePosition;
		oppositePosition.AddScale( center, 2.0, vertex.position, -1.0 );

		int j = triangleMesh.FindIndex( oppositePosition );
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
	gravityForce->accelDueToGravity.Set( 0.0, -5.0, 0.0 );
	particleSystem.forceCollection.AddObject( gravityForce );
}

void Blob::RegisterTrackCollisionObject( _3DMath::BoundingBoxTree* boxTree, double friction )
{
	ParticleSystem::BoundingBoxTreeCollisionObject* collisionObject = new ParticleSystem::BoundingBoxTreeCollisionObject();
	collisionObject->boxTree = boxTree;
	collisionObject->friction = friction;
	particleSystem.collisionObjectCollection.AddObject( collisionObject );
}

void Blob::AddSymmetricVertices( const Vector& vector )
{
	for( int i = 0; i < 8; i++ )
	{
		if( ( ( i & 1 ) && vector.x == 0.0 ) ||
			( ( i & 2 ) && vector.y == 0.0 ) ||
			( ( i & 4 ) && vector.z == 0.0 ) )
		{
			continue;
		}
		
		Vertex vertex;
		vertex.position = vector;
		
		vertex.position.x = ( i & 1 ) ? -vector.x : vector.x;
		vertex.position.y = ( i & 2 ) ? -vector.y : vector.y;
		vertex.position.z = ( i & 4 ) ? -vector.z : vector.z;
	
		triangleMesh.vertexArray->push_back( vertex );
	}
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