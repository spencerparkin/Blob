// Blob.cpp

#include "Blob.h"
#include "Texture.h"
#include "Driver.h"
#include "Frame.h"
#include "Application.h"
#include "InventoryItem.h"
#include <ListFunctions.h>

using namespace _3DMath;

Blob::Blob( void ) : maxTorque( 5.0 ), friction( 1.0 ), gravity( -4.0 )
{
	gravityForceHandle = 0;

	texture = nullptr;
	driver = nullptr;

	axleAngle = 0.0;

	inventoryItemList = new InventoryItemList();

	occupiedReceptacleHandle = 0;
}

/*virtual*/ Blob::~Blob( void )
{
	_3DMath::FreeList< InventoryItem >( *inventoryItemList );

	delete texture;
	delete driver;
	delete inventoryItemList;
}

void Blob::Render( _3DMath::Renderer& renderer )
{
	if( !texture )
	{
		texture = new Texture();
		texture->Load( "Data/Globe.png" );
	}

	texture->Bind();

	if( wxGetApp().frame->debugDrawFlags & Frame::DRAW_FORCES )
		renderer.DrawParticleSystem( particleSystem, Renderer::DRAW_FORCES );
	else
		renderer.DrawTriangleMesh( triangleMesh, Renderer::UV_CORRECTION );

	if( wxGetApp().frame->debugDrawFlags & Frame::DRAW_COLLISION_OBJECTS )
	{
		glDisable( GL_TEXTURE_2D );
		glColor3f( 1.f, 1.f, 1.f );
		boundingBox.Render( renderer );
	}
}

void Blob::Simulate( const _3DMath::TimeKeeper& timeKeeper )
{
	if( driver )
		driver->Drive( this, timeKeeper );

	friction.Simulate( timeKeeper );
	maxTorque.Simulate( timeKeeper );
	gravity.Simulate( timeKeeper );

	_3DMath::ParticleSystem::GravityForce* gravityForce = ( _3DMath::ParticleSystem::GravityForce* )_3DMath::HandleObject::Dereference( gravityForceHandle );
	if( gravityForce )
		gravity.GetValue( gravityForce->accelDueToGravity.y );

	triangleMesh.GenerateBoundingBox( boundingBox );

	double particleFriction = 1.0;
	friction.GetValue( particleFriction );

	_3DMath::ParticleSystem::ParticleList::iterator iter = particleSystem.particleList->begin();
	while( iter != particleSystem.particleList->end() )
	{
		_3DMath::ParticleSystem::Particle* particle = ( _3DMath::ParticleSystem::Particle* )*iter;
		particle->friction = particleFriction;
		iter++;
	}

	// TODO: By adjusting forces, I may be able to get rid of this hack.
	timeKeeper.fixedDeltaTimeMilliseconds = 50.0;
	particleSystem.Simulate( timeKeeper );
	timeKeeper.fixedDeltaTimeMilliseconds = 0.0;
}

void Blob::GetAxleAxis( _3DMath::Vector& unitAxleAxis ) const
{
	unitAxleAxis.Set( cos( axleAngle ), 0.0, sin( axleAngle ) );
}

/*virtual*/ void Blob::GetLocation( _3DMath::Vector& location ) const
{
	location = particleSystem.centerOfMass;
}

/*virtual*/ void Blob::GetFacingDirection( _3DMath::Vector& facingUnitDir ) const
{
	GetAxleAxis( facingUnitDir );

	facingUnitDir.Rotate( _3DMath::Vector( 0.0, 1.0, 0.0 ), M_PI / 2.0 );
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

	std::vector< int > particleHandles;

	for( int i = 0; i < ( int )triangleMesh.vertexArray->size(); i++ )
	{
		ParticleSystem::MeshVertexParticle* particle = new ParticleSystem::MeshVertexParticle();
		particle->index = i;
		particle->mesh = &triangleMesh;
		particle->mass = 1.0;

		particleSystem.particleList->push_back( particle );
		particle->GetPosition( particle->previousPosition );
		particleHandles.push_back( particle->GetHandle() );
	}

	TriangleMesh::EdgeSet edgeSet;
	triangleMesh.GenerateEdgeSet( edgeSet );

	double stiffness = 60.0;

	for( TriangleMesh::EdgeSet::iterator iter = edgeSet.begin(); iter != edgeSet.end(); iter++ )
	{
		uint64_t edgePair = *iter;

		int index0, index1;
		TriangleMesh::GetEdgePair( edgePair, index0, index1 );

		MakeSpring( index0, index1, particleHandles, stiffness );
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
				MakeSpring( i, j, particleHandles, stiffness );
				edgeSet.insert( edgePair );
			}
		}
	}

	ParticleSystem::GravityForce* gravityForce = new ParticleSystem::GravityForce( &particleSystem );
	gravityForce->accelDueToGravity.Set( 0.0, -5.0, 0.0 );
	particleSystem.forceList->push_back( gravityForce );
	gravityForceHandle = gravityForce->GetHandle();
}

void Blob::Teleport( const _3DMath::AffineTransform& transform )
{
	triangleMesh.Transform( transform );
	particleSystem.ResetMotion();
}

void Blob::RegisterGroundCollisionObject( _3DMath::BoundingBoxTree* boxTree, double friction )
{
	ParticleSystem::BoundingBoxTreeCollisionObject* collisionObject = new ParticleSystem::BoundingBoxTreeCollisionObject();
	collisionObject->boxTree = boxTree;
	collisionObject->friction = friction;
	particleSystem.collisionObjectList->push_back( collisionObject );
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

void Blob::MakeSpring( int index0, int index1, std::vector< int >& particleHandles, double stiffness )
{
	Vector position0, position1;

	ParticleSystem::Particle* particle0 = ( ParticleSystem::Particle* )HandleObject::Dereference( particleHandles[ index0 ] );
	particle0->GetPosition( position0 );

	ParticleSystem::Particle* particle1 = ( ParticleSystem::Particle* )HandleObject::Dereference( particleHandles[ index1 ] );
	particle1->GetPosition( position1 );

	Vector vector;
	vector.Subtract( position1, position0 );
	double length = vector.Length();

	ParticleSystem::SpringForce* springForce = new ParticleSystem::SpringForce( &particleSystem );
	springForce->endPointParticleHandles[0] = particle0->GetHandle();
	springForce->endPointParticleHandles[1] = particle1->GetHandle();
	springForce->equilibriumLength = length;
	springForce->stiffness = stiffness;

	particleSystem.forceList->push_back( springForce );
}

// Blob.cpp