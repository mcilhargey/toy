

#include <core/Generated/Types.h>
#include <core/Generated/Module.h>
#include <obj/Proto.h>

namespace mud
{
    // Exported types
    template <> TOY_CORE_EXPORT Type& type<toy::CollisionGroup>() { static Type ty("toy::CollisionGroup"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Behavior::Priority>() { static Type ty("toy::Behavior::Priority"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::SamplePolyAreas>() { static Type ty("toy::SamplePolyAreas"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::SamplePolyFlags>() { static Type ty("toy::SamplePolyFlags"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Task>() { static Type ty("toy::Task"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<std::vector<mud::Symbol>>() { static Type ty("std::vector<mud::Symbol>"); return ty; }
    
    template <> TOY_CORE_EXPORT Type& type<toy::Active>() { static Type ty("Active"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Actor>() { static Type ty("Actor"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Array<toy::Action>>() { static Type ty("Array<toy::Action>"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Array<toy::Entity>>() { static Type ty("Array<toy::Entity>"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Behavior>() { static Type ty("Behavior"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::BufferPage>() { static Type ty("BufferPage"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::BulletShape>() { static Type ty("BulletShape"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Camera>() { static Type ty("Camera"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Collider>() { static Type ty("Collider"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ColliderImpl>() { static Type ty("ColliderImpl"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ColliderObject>() { static Type ty("ColliderObject"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::CollisionShape>() { static Type ty("CollisionShape"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Core>() { static Type ty("Core"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::DetourPath>() { static Type ty("DetourPath"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Effect>() { static Type ty("Effect"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Emitter>() { static Type ty("Emitter"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EventFilter>() { static Type ty("EventFilter"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EventRelay>() { static Type ty("EventRelay"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::GroundMotion>() { static Type ty("GroundMotion"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::LightReflector>() { static Type ty("LightReflector"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::LightSource>() { static Type ty("LightSource"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Manoeuvrable>() { static Type ty("Manoeuvrable"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Movable>() { static Type ty("Movable"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Navblock>() { static Type ty("Navblock"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Navmesh>() { static Type ty("Navmesh"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Obstacle>() { static Type ty("Obstacle"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Pathfinder>() { static Type ty("Pathfinder"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::PhysicWorld>() { static Type ty("PhysicWorld"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Player>() { static Type ty("Player"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Reactive>() { static Type ty("Reactive"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Receptor>() { static Type ty("Receptor"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Selector>() { static Type ty("Selector"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::SoundMedium>() { static Type ty("SoundMedium"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::State>() { static Type ty("State"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::SubBulletWorld>() { static Type ty("SubBulletWorld"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Symbolic>() { static Type ty("Symbolic"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::TaskSection>() { static Type ty("TaskSection"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::User>() { static Type ty("User"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::View>() { static Type ty("View"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Vision>() { static Type ty("Vision"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::VisualMedium>() { static Type ty("VisualMedium"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::WorldClock>() { static Type ty("WorldClock"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::WorldMedium>() { static Type ty("WorldMedium"); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Entity>() { static Type ty("Entity", type<mud::Complex>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::World>() { static Type ty("World", type<mud::Complex>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::NavmeshShape>() { static Type ty("NavmeshShape", type<mud::Shape>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Action>() { static Type ty("Action", type<toy::Procedure>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::BulletWorld>() { static Type ty("BulletWorld", type<toy::PhysicWorld>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::QueueSection>() { static Type ty("QueueSection", type<toy::TaskSection>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::MonoSection>() { static Type ty("MonoSection", type<toy::TaskSection>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ParallelSection>() { static Type ty("ParallelSection", type<toy::TaskSection>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::BulletCollider>() { static Type ty("BulletCollider", type<toy::ColliderImpl>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Area>() { static Type ty("Area", type<toy::ColliderObject>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::WorldPage>() { static Type ty("WorldPage", type<toy::ColliderObject>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Solid>() { static Type ty("Solid", type<toy::Collider>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::PhysicScope>() { static Type ty("PhysicScope", type<toy::Collider>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ObstacleBody>() { static Type ty("ObstacleBody", type<toy::Collider>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::BulletSolid>() { static Type ty("BulletSolid", type<toy::BulletCollider>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EmitterSphere>() { static Type ty("EmitterSphere", type<toy::EmitterScope>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorSphere>() { static Type ty("ReceptorSphere", type<toy::ReceptorScope>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EventEmitter>() { static Type ty("EventEmitter", type<toy::Emitter>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EmitterScope>() { static Type ty("EmitterScope", type<toy::PhysicScope>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorScope>() { static Type ty("ReceptorScope", type<toy::PhysicScope>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::EventReceptor>() { static Type ty("EventReceptor", type<toy::Receptor>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::StoreView>() { static Type ty("StoreView", type<toy::View>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorView>() { static Type ty("ReceptorView", type<toy::View>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::PlayerVision>() { static Type ty("PlayerVision", type<toy::Vision>()); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::OmniVision>() { static Type ty("OmniVision", type<toy::Vision>()); return ty; }
    template <> TOY_CORE_EXPORT Prototype& proto<toy::Origin>() { static Prototype ty("toy::Origin", type<toy::Entity>(), {  }); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::Origin>() { return proto<toy::Origin>(); }
    template <> TOY_CORE_EXPORT Prototype& proto<toy::DefaultWorld>() { static Prototype ty("toy::DefaultWorld", type<toy::World>(), { &type<toy::BulletWorld>(), &type<toy::Navmesh>() }); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::DefaultWorld>() { return proto<toy::DefaultWorld>(); }
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OCamera>() { static Prototype ty("toy::OCamera", type<toy::Entity>(), { &type<toy::Movable>(), &type<toy::Receptor>(), &type<toy::Camera>() }); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::OCamera>() { return proto<toy::OCamera>(); }
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OLight>() { static Prototype ty("toy::OLight", type<toy::Entity>(), { &type<toy::Emitter>(), &type<toy::LightSource>() }); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::OLight>() { return proto<toy::OLight>(); }
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OWaypoint>() { static Prototype ty("toy::OWaypoint", type<toy::Entity>(), {  }); return ty; }
    template <> TOY_CORE_EXPORT Type& type<toy::OWaypoint>() { return proto<toy::OWaypoint>(); }
}
