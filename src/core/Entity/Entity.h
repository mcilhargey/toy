//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Complex.h>
#include <core/Store/Array.h>
#include <obj/Util/Updatable.h>
#include <obj/Memory/ObjectPool.h>
#include <math/VecOps.h>
#include <math/Axes.h>
#include <core/Generated/Forward.h>
#include <core/Generated/Types.h> // @kludge because of Array<T>
#include <core/Entity/EntityObserver.h>

/* std */
#include <vector>
#include <memory>

using namespace mud; namespace toy
{
	template class _refl_ TOY_CORE_EXPORT Array<Entity>;

	class _refl_ TOY_CORE_EXPORT Entity : public Complex, public Updatable
    {
	public:
		_constr_ Entity(Id id, Prototype& prototype, Entity& parent, const vec3& position, const quat& rotation);
		_constr_ Entity(Id id, Prototype& prototype, World& world, Entity* parent, const vec3& position, const quat& rotation);
        ~Entity();

		_attr_ World& m_world;
		_attr_ _mut_ _link_ Entity* m_parent;

		_attr_ _mut_ vec3 m_position;
		_attr_ _mut_ quat m_rotation;
		_attr_ _graph_ Array<Entity> m_contents;

		size_t m_lastTick = 0;
		size_t m_lastUpdated = 0;
		size_t m_lastModified = 0;

		bool m_moved;
		bool m_hooked;

		Entity& origin();
		void setParent(Entity* entity);
		bool isChildOf(Entity& entity);

		vec3 absolutePosition();
		quat absoluteRotation();

		inline void setDirty(bool moved) { m_lastUpdated = m_lastTick + 1; m_lastModified = m_lastTick + 1; m_moved = moved; }
		inline void setSyncDirty(bool moved) { m_lastUpdated = m_lastTick + 1; m_moved = moved; }

		inline void setPosition(const vec3& position) { m_position = position; this->setDirty(true); }
		inline void setRotation(const quat& rotation) { m_rotation = rotation; this->setDirty(false); }

		inline void syncPosition(const vec3& position) { m_position = position; this->setSyncDirty(true); }
		inline void syncRotation(const quat& rotation) { m_rotation = rotation; this->setSyncDirty(false); }

		inline vec3 front() const { return mud::rotate(m_rotation, to_vec3(Side::Front)); }
		inline vec3 right() const { return mud::rotate(m_rotation, to_vec3(Side::Right)); }
		inline vec3 up()    const { return mud::rotate(m_rotation, to_vec3(Side::Up)); }
		inline vec3 down()  const { return mud::rotate(m_rotation, to_vec3(Side::Down)); }

		void translate(const vec3& vec);
		void rotate(const vec3& axis, float angle);

		void yaw(float value);
		void fixedYaw(float value);
		void pitch(float value);
		void roll(float value);

		void next_frame(size_t tick, size_t delta);
		
		Entity* spatialRoot();
		Entity* findParent(Type& part_type);

		void remove();

		void detach(Entity& child);
		void detachto(Entity& moveto);

		void hook();
		void unhook();

		typedef std::function<bool(Entity&)> Visitor;
		void visit(const Visitor& visitor);

		void debugContents(size_t depth = 0);

	public:
		void addHookObserver(HookObserver& listener);
		void removeHookObserver(HookObserver& listener);

	public:
		template <class T, class... Types>
		Entity& constructNested(Types&&... args) { return GlobalPool::me().template pool<T>().construct(0, *this, std::forward<Types>(args)...).m_entity; }

    private:
		MotionState* m_motionState;

		std::vector<HookObserver*> m_hookObservers; // @todo study replacing this with other way : check each frame if entity is still hooked
	};
}
