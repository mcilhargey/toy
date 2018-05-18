//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#pragma once

#include <blocks/Generated/Forward.h>
#include <toy/toy.h>

using namespace mud;
using namespace toy;

_EX_BLOCKS_EXPORT void ex_blocks_pump(GameShell& app, Game& game, Widget& parent, Dockbar& dockbar);

class _refl_ _EX_BLOCKS_EXPORT Crate : public Construct, public ColliderObject
{
public:
	_constr_ Crate(Id id, Entity& parent, const vec3& position, const vec3& extents);

	_comp_ _attr_ Entity m_entity;
	_comp_ Movable m_movable;
	_comp_ Emitter m_emitter;
	_comp_ Active m_active;

	_attr_ vec3 m_extents;
	Solid m_solid;
};

class _refl_ _EX_BLOCKS_EXPORT BlockWorld : public Construct
{
public:
	_constr_ BlockWorld(const std::string& name);
	~BlockWorld();

	_comp_ _attr_ World m_world;
	_comp_ BulletWorld m_bulletWorld;
	_comp_ Navmesh m_navmesh;

	_attr_ _mut_ std::vector<Sector*> m_sectors;

	_attr_ _mut_ Grid<Block*> m_grid;

	_attr_ _mut_ std::vector<Element*> m_elements;
};