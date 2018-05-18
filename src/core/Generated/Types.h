#pragma once

#include <core/Generated/Forward.h>

#include <obj/Generated/Types.h>
#include <math/Generated/Types.h>
#include <geom/Generated/Types.h>
#include <util/Generated/Types.h>

#include <obj/Type.h>
#include <obj/Proto.h>

#include <string>
#include <vector>
#include <stdint.h>


namespace mud
{
    // Exported types
    template <> TOY_CORE_EXPORT Type& type<toy::CollisionGroup>();
    template <> TOY_CORE_EXPORT Type& type<toy::SamplePolyAreas>();
    template <> TOY_CORE_EXPORT Type& type<toy::SamplePolyFlags>();
    template <> TOY_CORE_EXPORT Type& type<toy::Task>();
    template <> TOY_CORE_EXPORT Type& type<std::vector<mud::Symbol>>();
    
    template <> TOY_CORE_EXPORT Type& type<toy::Active>();
    template <> TOY_CORE_EXPORT Type& type<toy::Actor>();
    template <> TOY_CORE_EXPORT Type& type<toy::Array<toy::Action>>();
    template <> TOY_CORE_EXPORT Type& type<toy::Array<toy::Entity>>();
    template <> TOY_CORE_EXPORT Type& type<toy::Behavior>();
    template <> TOY_CORE_EXPORT Type& type<toy::BufferPage>();
    template <> TOY_CORE_EXPORT Type& type<toy::BulletShape>();
    template <> TOY_CORE_EXPORT Type& type<toy::Camera>();
    template <> TOY_CORE_EXPORT Type& type<toy::Collider>();
    template <> TOY_CORE_EXPORT Type& type<toy::ColliderImpl>();
    template <> TOY_CORE_EXPORT Type& type<toy::ColliderObject>();
    template <> TOY_CORE_EXPORT Type& type<toy::CollisionShape>();
    template <> TOY_CORE_EXPORT Type& type<toy::Core>();
    template <> TOY_CORE_EXPORT Type& type<toy::DetourPath>();
    template <> TOY_CORE_EXPORT Type& type<toy::Effect>();
    template <> TOY_CORE_EXPORT Type& type<toy::Emitter>();
    template <> TOY_CORE_EXPORT Type& type<toy::EventFilter>();
    template <> TOY_CORE_EXPORT Type& type<toy::EventRelay>();
    template <> TOY_CORE_EXPORT Type& type<toy::GroundMotion>();
    template <> TOY_CORE_EXPORT Type& type<toy::LightReflector>();
    template <> TOY_CORE_EXPORT Type& type<toy::LightSource>();
    template <> TOY_CORE_EXPORT Type& type<toy::Manoeuvrable>();
    template <> TOY_CORE_EXPORT Type& type<toy::Movable>();
    template <> TOY_CORE_EXPORT Type& type<toy::Navblock>();
    template <> TOY_CORE_EXPORT Type& type<toy::Navmesh>();
    template <> TOY_CORE_EXPORT Type& type<toy::Obstacle>();
    template <> TOY_CORE_EXPORT Type& type<toy::Pathfinder>();
    template <> TOY_CORE_EXPORT Type& type<toy::PhysicWorld>();
    template <> TOY_CORE_EXPORT Type& type<toy::Player>();
    template <> TOY_CORE_EXPORT Type& type<toy::Reactive>();
    template <> TOY_CORE_EXPORT Type& type<toy::Receptor>();
    template <> TOY_CORE_EXPORT Type& type<toy::Selector>();
    template <> TOY_CORE_EXPORT Type& type<toy::SoundMedium>();
    template <> TOY_CORE_EXPORT Type& type<toy::State>();
    template <> TOY_CORE_EXPORT Type& type<toy::SubBulletWorld>();
    template <> TOY_CORE_EXPORT Type& type<toy::Symbolic>();
    template <> TOY_CORE_EXPORT Type& type<toy::TaskSection>();
    template <> TOY_CORE_EXPORT Type& type<toy::User>();
    template <> TOY_CORE_EXPORT Type& type<toy::View>();
    template <> TOY_CORE_EXPORT Type& type<toy::Vision>();
    template <> TOY_CORE_EXPORT Type& type<toy::VisualMedium>();
    template <> TOY_CORE_EXPORT Type& type<toy::WorldClock>();
    template <> TOY_CORE_EXPORT Type& type<toy::WorldMedium>();
    template <> TOY_CORE_EXPORT Type& type<toy::Entity>();
    template <> TOY_CORE_EXPORT Type& type<toy::World>();
    template <> TOY_CORE_EXPORT Type& type<toy::NavmeshShape>();
    template <> TOY_CORE_EXPORT Type& type<toy::Action>();
    template <> TOY_CORE_EXPORT Type& type<toy::BulletWorld>();
    template <> TOY_CORE_EXPORT Type& type<toy::QueueSection>();
    template <> TOY_CORE_EXPORT Type& type<toy::MonoSection>();
    template <> TOY_CORE_EXPORT Type& type<toy::ParallelSection>();
    template <> TOY_CORE_EXPORT Type& type<toy::BulletCollider>();
    template <> TOY_CORE_EXPORT Type& type<toy::Area>();
    template <> TOY_CORE_EXPORT Type& type<toy::WorldPage>();
    template <> TOY_CORE_EXPORT Type& type<toy::Solid>();
    template <> TOY_CORE_EXPORT Type& type<toy::PhysicScope>();
    template <> TOY_CORE_EXPORT Type& type<toy::ObstacleBody>();
    template <> TOY_CORE_EXPORT Type& type<toy::BulletSolid>();
    template <> TOY_CORE_EXPORT Type& type<toy::EmitterSphere>();
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorSphere>();
    template <> TOY_CORE_EXPORT Type& type<toy::EventEmitter>();
    template <> TOY_CORE_EXPORT Type& type<toy::EmitterScope>();
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorScope>();
    template <> TOY_CORE_EXPORT Type& type<toy::EventReceptor>();
    template <> TOY_CORE_EXPORT Type& type<toy::StoreView>();
    template <> TOY_CORE_EXPORT Type& type<toy::ReceptorView>();
    template <> TOY_CORE_EXPORT Type& type<toy::PlayerVision>();
    template <> TOY_CORE_EXPORT Type& type<toy::OmniVision>();
    template <> TOY_CORE_EXPORT Prototype& proto<toy::Origin>();
    template <> TOY_CORE_EXPORT Type& type<toy::Origin>();
    template <> TOY_CORE_EXPORT Prototype& proto<toy::DefaultWorld>();
    template <> TOY_CORE_EXPORT Type& type<toy::DefaultWorld>();
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OCamera>();
    template <> TOY_CORE_EXPORT Type& type<toy::OCamera>();
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OLight>();
    template <> TOY_CORE_EXPORT Type& type<toy::OLight>();
    template <> TOY_CORE_EXPORT Prototype& proto<toy::OWaypoint>();
    template <> TOY_CORE_EXPORT Type& type<toy::OWaypoint>();
}
