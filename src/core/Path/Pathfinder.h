//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <proto/Complex.h>
#include <math/Math.h>
#include <core/Forward.h>
#include <core/Path/DetourPath.h>

#ifndef MUD_CPP_20
#include <vector>
#endif

class dtNavMesh;
class dtNavMeshQuery;
class dtQueryFilter;

using namespace mud; namespace toy
{
	class refl_ TOY_CORE_EXPORT Pathfinder : public NonCopy
    {
	public:
		constr_ Pathfinder(Navmesh& navmesh);
		~Pathfinder();

		dtNavMesh& m_navmesh;

		unique_ptr<dtNavMeshQuery> m_query;
		unique_ptr<dtQueryFilter> m_filter;

		size_t m_max_polys = 20;
		size_t m_max_waypoints = 50;

		bool validity(const vec3& pos);
		void nearestValid(vec3& destination, float margin);
    };
}
