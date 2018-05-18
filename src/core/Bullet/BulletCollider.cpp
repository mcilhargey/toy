//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Bullet/BulletCollider.h>

#include <obj/Util/DispatchDecl.h>

#include <geom/Shapes.h>
#include <geom/ShapesComplex.h>
#include <geom/Mesh.h>
#include <geom/Primitive.h>
#define TOY_PRIVATE
#include <core/Bullet.h>

#include <core/World/World.h>
#include <core/Entity/Entity.h>
#include <core/Bullet/BulletWorld.h>
#include <core/Physic/PhysicWorld.h>
#include <core/Physic/Medium.h>

/* bullet */

#if _MSC_VER
#	pragma warning (push)
#	pragma warning (disable : 4127)
#	pragma warning (disable : 4100)
#endif

#include <BulletCollision/CollisionDispatch/btGhostObject.h>
#include <BulletCollision/CollisionShapes/btConvexHullShape.h>
#include <BulletCollision/CollisionShapes/btSphereShape.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <btBulletCollisionCommon.h>

#if _MSC_VER
#	pragma warning (pop)
#endif

using namespace mud; namespace toy
{
	BulletShape::BulletShape(unique_ptr<btCollisionShape> shape)
		: shape(std::move(shape))
	{}

	BulletShape::BulletShape(unique_ptr<btCollisionShape> shape, unique_ptr<btStridingMeshInterface> mesh)
		: shape(std::move(shape))
		, mesh(std::move(mesh))
	{}

	BulletShape::BulletShape(BulletShape&& other)
		: shape(std::move(other.shape))
		, mesh(std::move(other.mesh))
	{}

	BulletShape& BulletShape::operator=(BulletShape&& other)
	{
		this->shape = std::move(other.shape);
		this->mesh = std::move(other.mesh);
		return *this;
	}

	BulletShape::~BulletShape()
	{}

	BulletShape createGeometryShape(Geometry& geometry)
	{
		unique_ptr<btTriangleMesh> trimesh = make_unique<btTriangleMesh>();

		btVector3 vertex[3];
		for(const Tri& triangle : geometry.m_triangles)
		{
			for(int i = 0; i < 3; ++i)
			{
				uint32_t index = (&triangle.a)[i];
				vertex[i] = to_btvec3(geometry.m_vertices[index].m_position);
			}

			trimesh->addTriangle(vertex[0], vertex[1], vertex[2]);
		}

		const bool useQuantizedAABB = true;
		unique_ptr<btCollisionShape> meshShape(make_unique<btBvhTriangleMeshShape>(trimesh.get(), useQuantizedAABB));

		return BulletShape(std::move(meshShape), std::move(trimesh));
	}

	BulletShape createConvexHullShape(ConvexHull& hull)
	{
		unique_ptr<btConvexHullShape> convexHull = make_unique<btConvexHullShape>();
		for(const vec3& point : hull.m_vertices)
			convexHull->addPoint(to_btvec3(point));
		return BulletShape(std::move(convexHull));
	}

	DispatchBulletShape::DispatchBulletShape()
	{
		dispatch_branch<Sphere>(*this, [](Sphere& sphere) -> BulletShape { return{ make_unique<btSphereShape>(sphere.m_radius) }; });
		dispatch_branch<Capsule>(*this, [](Capsule& capsule) -> BulletShape { return{ make_unique<btCapsuleShape>(capsule.m_radius, capsule.m_height) }; });
		dispatch_branch<Cylinder>(*this, [](Cylinder& cylinder) -> BulletShape { return{ make_unique<btCylinderShape>(btVector3(cylinder.m_radius, cylinder.m_height / 2.f, cylinder.m_radius)) }; });
		dispatch_branch<Cube>(*this, [](Cube& box) -> BulletShape { return{ make_unique<btBoxShape>(to_btvec3(box.m_extents)) }; });
		dispatch_branch<ConvexHull>(*this, &createConvexHullShape);
		dispatch_branch<Geometry>(*this, &createGeometryShape);
	};

	BulletShape DispatchBulletShape::dispatch(CollisionShape& collisionShape)
	{
		Ref shape = collisionShape.m_shape.get();
		return Dispatch::dispatch(shape); 
	}

	class ContactCheck : public btCollisionWorld::ContactResultCallback
	{
	public:
		ContactCheck(std::vector<Collider*>& collisions, float margin = 0.f)
			: m_collisions(collisions)
			, m_margin(margin)
		{}

		ContactCheck& operator=(const ContactCheck&) = delete;

		virtual bool needsCollision(btBroadphaseProxy* proxy) const
		{
			return btCollisionWorld::ContactResultCallback::needsCollision(proxy);
		}

		btScalar addSingleResult(btManifoldPoint &cp, const btCollisionObjectWrapper *colObj0, int partId0, int index0, const btCollisionObjectWrapper *colObj1, int partId1, int index1)
		{
			UNUSED(colObj0); UNUSED(index0); UNUSED(index1); UNUSED(partId0); UNUSED(partId1);
			if(cp.getDistance() < m_margin)
				m_collisions.push_back((Collider*)colObj1->m_collisionObject->getUserPointer());

			return 0.f;
		}

	private:
		std::vector<Collider*>& m_collisions;
		float m_margin;
	};

	BulletCollider::BulletCollider(SubBulletWorld& bulletWorld, Collider& collider, bool create)
		: m_bulletWorld(bulletWorld)
		, m_collider(collider)
		, m_collisionShape(DispatchBulletShape::me().dispatch(collider.m_collisionShape))
	{
		if(create)
			this->setupObject(make_unique<btCollisionObject>());
	}

	BulletCollider::~BulletCollider()
	{}

	void BulletCollider::setupObject(unique_ptr<btCollisionObject> collisionObject)
	{
		vec3 position = m_collider.m_entity.m_position + m_collider.m_collisionShape.m_center;

		m_collisionObject = std::move(collisionObject);
		m_collisionObject->setUserPointer(&m_collider);
		m_collisionObject->setWorldTransform(btTransform(to_btquat(m_collider.m_entity.m_rotation), to_btvec3(position)));
		m_collisionObject->setCollisionFlags(0);
		m_collisionObject->setCollisionShape(m_collisionShape.shape.get());
		//m_collisionObject->setContactProcessingThreshold(0.1f);
	}

	void BulletCollider::updateTransform(const vec3& position, const quat& rotation)
	{
		m_collisionObject->setWorldTransform(btTransform(to_btquat(rotation), to_btvec3(position)));
	}

	void BulletCollider::forceUpdate()
	{
		this->updateTransform(m_collider.m_entity.absolutePosition(), m_collider.m_entity.absoluteRotation());
		m_bulletWorld.m_bulletWorld->updateSingleAabb(m_collisionObject.get());
		m_bulletWorld.updateContacts();
	}

	void BulletCollider::project(const vec3& position, ContactList& collisions, short int mask)
	{
		btTransform transform(m_collisionObject->getWorldTransform());
		m_collisionObject->setWorldTransform(btTransform(to_btquat(m_collider.m_entity.m_rotation), to_btvec3(position)));

		ContactCheck callback(collisions);
		callback.m_collisionFilterGroup = mask;
		callback.m_collisionFilterMask = mask;
		//mGhostObject->checkCollideWith();
		
		m_bulletWorld.m_bulletWorld->updateSingleAabb(m_collisionObject.get());
		m_bulletWorld.m_bulletWorld->contactTest(m_collisionObject.get(), callback);

		m_collisionObject->setWorldTransform(transform);
	}

	void BulletCollider::raycast(Collider& other, ContactList& obstacles, short int mask)
	{
		btVector3 from = to_btvec3(m_collider.m_entity.m_position);
		btVector3 to = to_btvec3(other.m_entity.m_position);

		if(from == to)
			return;

		btCollisionWorld::AllHitsRayResultCallback rayCallback(from, to);
		rayCallback.m_collisionFilterMask = mask;
		rayCallback.m_collisionFilterGroup = btBroadphaseProxy::AllFilter;
		m_bulletWorld.m_bulletWorld->rayTest(from, to, rayCallback);
		//printf << "raycast from " << from.x() << " , " << from.y() << " , " << from.z() << " to " << to.x() << " , " << to.y() << " , " << to.z() << std::endl;
		
		for(int i = 0; i != rayCallback.m_collisionObjects.size(); ++i)
		{
			Collider* physic = static_cast<Collider*>(rayCallback.m_collisionObjects[i]->getUserPointer());
			obstacles.push_back(physic);
		}
	}
}
