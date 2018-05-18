//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.

#include <core/WorldPage/BufferPage.h>

#include <obj/Reflect/Meta.h>
#include <util/Loader/Loader.h>

#include <core/Entity/Entity.h>
#include <core/Physic/Scope.h>

#include <core/World/World.h>
#include <core/Physic/PhysicWorld.h>

#include <core/Physic/Collider.h>
#include <core/Physic/CollisionShape.h>
#include <core/Physic/Solid.h>

#include <core/WorldPage/WorldPage.h>

using namespace mud; namespace toy
{
	BufferPage::BufferPage(Entity& entity, WorldPage& worldPage, bool loaded)
        : m_entity(entity)
		, m_worldPage(worldPage)
		, m_loaded(loaded)
		, m_receptors(0)
    {
		//printf("Creating BufferPage %u at %f, %f, %f\n", m_entity.m_id, m_entity.m_position.x(), m_entity.m_position.y(), m_entity.m_position.z());
		m_worldPage.m_scope.m_scope.observe(*this);
	}

    BufferPage::~BufferPage()
    {
		m_worldPage.m_scope.m_scope.unobserve(*this);
	}

	void BufferPage::handleAdd(Entity& entity)
	{
		this->add(entity);
	}

	void BufferPage::handleRemove(Entity& entity)
	{
		this->remove(entity);
	}

	void BufferPage::add(Entity& entity)
	{
		UNUSED(entity);

		//for(WorldPage* page : m_worldPage.adjacentPages().store())
		//	page->m_entity.part<BufferPage>().increment();

		this->increment();
	}

	void BufferPage::increment()
	{
		//if(++m_receptors && !m_loaded)
		//	this->load();
	}

	void BufferPage::remove(Entity& entity)
	{
		UNUSED(entity);

		//for(WorldPage* page : m_worldPage.adjacentPages().store())
		//	page->m_entity.part<BufferPage>().decrement();

		this->decrement();
	}

	void BufferPage::decrement()
	{
		//if(!--m_receptors && m_loaded)
		//	this->unload();
	}

	void BufferPage::load()
	{
		printf("BufferPage %u loaded\n", m_entity.m_id);
		m_loaded = true;
		ObjectLoader& loader = GlobalLoader::me().getLoader(type<Entity>());
		loader.fill(&m_worldPage.m_entity, m_worldPage.m_entity.m_id);
	}

	void BufferPage::unload()
	{
		printf("BufferPage %u unloaded\n", m_entity.m_id);
		m_loaded = false;
		ObjectLoader& loader = GlobalLoader::me().getLoader(type<Entity>());
		loader.save(&m_worldPage.m_entity, m_worldPage.m_entity.m_id);
		this->clear();
	}

	void BufferPage::clear()
	{
		//m_worldPage.m_entity.part<EntityPool>().clear();
	}

	void BufferPage::fill()
	{
		ObjectLoader& loader = GlobalLoader::me().getLoader(type<Entity>());
		loader.fill(&m_worldPage.m_entity, m_worldPage.m_entity.m_id);
	}
}
