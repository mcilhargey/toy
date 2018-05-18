//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <platform/Generated/Forward.h>
#include <toy/toy.h>

using namespace mud;
using namespace toy;

class _refl_ _EX_PLATFORM_EXPORT Worldblock : public Construct, public Updatable
{
public:
	_constr_ Worldblock(Id id, Entity& parent, const vec3& position, const uvec3& size, const vec3& period, WaveTileset& tileset);
	~Worldblock();

	_comp_ _attr_ Entity m_entity;
	_comp_ _attr_ Emitter m_emitter;
	_comp_ _attr_ WorldPage m_world_page;
	_comp_ _attr_ Navblock m_navblock;
	_attr_ Tileblock m_tileblock;

	virtual void next_frame(size_t frame, size_t delta) final;
};

class _refl_ _EX_PLATFORM_EXPORT TileWorld : public Construct
{
public:
	_constr_ TileWorld(const std::string& name);
	~TileWorld();

	_comp_ _attr_ World m_world;
	_comp_ _attr_ BulletWorld m_bulletWorld;
	_comp_ _attr_ Navmesh m_navmesh;

	Worldblock* m_tileblock = nullptr;
};

class _refl_ _EX_PLATFORM_EXPORT Human2 : public Construct, public Updatable, public ColliderObject
{
public:
	_constr_ Human2(Id id,
		Entity& parent,
		const vec3& position,
		const std::string& name);
	~Human2();

	_comp_ _attr_ Entity m_entity;
	_comp_ Emitter m_emitter;
	_comp_ Receptor m_receptor;
	_comp_ Active m_active;

	_attr_ _mut_ std::string m_name;
	//Gauge m_minerals;
	Solid m_solid;

	vec3 m_force;
	vec3 m_torque;

	void next_frame(size_t tick, size_t delta);
};
