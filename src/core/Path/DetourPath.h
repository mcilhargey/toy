//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

/* toy */
#include <obj/Proto.h>
#include <math/Math.h>
#include <math/Vec.h>
#include <obj/NonCopy.h>
#include <core/Generated/Forward.h>
#include <core/Entity/Entity.h>

/* std */
#include <vector>
#include <memory>

using namespace mud; namespace toy
{
	typedef unsigned int dtPolyRef;

	class _refl_ TOY_CORE_EXPORT OWaypoint : public Construct
	{
	public:
		_constr_ OWaypoint(Id id, Entity& parent, const vec3& position);

		_comp_ _attr_ Entity m_entity;
	};

    class _refl_ TOY_CORE_EXPORT DetourPath
    {
	public:
		DetourPath(Entity& entity, Pathfinder& pathfinder, const vec3& origin, const vec3& destination);

		Entity& m_entity;
		Pathfinder& m_pathfinder;

		vec3 m_origin;
		vec3 m_destination;

		std::vector<vec3> m_path;
		std::vector<vec3> m_waypoints;
		std::vector<dtPolyRef> m_poly_path;

		void clear();
		bool compute();

	private:
		dtPolyRef m_currentPoly;
    };
}
