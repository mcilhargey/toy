//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/Physic/Medium.h>

#include <core/Physic/Collider.h>
#include <core/Physic/Obstacle.h>
#include <core/Physic/Scope.h>

using namespace mud; namespace toy
{
	Medium::Medium(const string& name, bool occlusions)
		: m_name(name)
		, m_occlusions(occlusions)
		, m_solid(false)
	{
		m_masks[CM_SOURCE] = CM_RECEPTOR;
		m_masks[CM_RECEPTOR] = CM_SOURCE;
		m_masks[CM_OBSTACLE] = CM_OBSTACLE;
	}

	short int Medium::mask(CollisionGroup group)
	{
		return m_masks[group];
	}

	float Medium::computeThroughput(EmitterScope& emitter, ReceptorScope& receptor, std::vector<ObstacleBody*>& occluding)
	{
		UNUSED(emitter); UNUSED(receptor);

		float throughput = 1.f;

		for(ObstacleBody* obstacle : occluding)
		{
			if(obstacle->m_throughput <= 0.f)
				return 0.f;
			
			throughput *= obstacle->m_throughput;
		}

		//float distance2 = source->m_position.distance2(receptor->m_position);
		
		//if(distance2 > 1.f)
		//	throughput *= 1/distance2;

		return throughput;
	}
}
