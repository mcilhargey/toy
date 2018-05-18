
#include <blocks/Scene.h>
#include <blocks/ex_blocks.h>
#include <toy/toy.h>

void paint_crate(Gnode& parent, Crate& crate)
{
	static Material& material = *parent.m_scene->m_gfx_system.fetch_material("crate");
	Item& item = gfx::shape(parent, Cube(crate.m_extents), Symbol(Colour::None, Colour::White));
	item.m_material = &material;
}

ParticleGenerator shoot_particles(Gnode& parent)
{
	ParticleSystem& particle_system = parent.m_scene->m_gfx_system.m_pipeline.block<BlockParticles>()->m_particle_system;

	ParticleGenerator emitter;
	float radius = 2.f;
	float scale = 0.5f;
	//emitter.m_shape = Ring(0.01f, 0.f, 1.f);
	emitter.m_shape = Ring(0.01f, 0.f, 1.f);
	emitter.m_direction = EmitterDirection::Absolute;
	emitter.m_rate = { 10, 10 };
	emitter.m_colour = { Colour::White, Colour::White };
	emitter.m_scale = { scale, scale };
	//emitter.m_scale = { 0.1f, 0.1f };
	//emitter.m_speed = { 0.f, 0.f };
	emitter.m_speed = { 20.f, 5.f };
	emitter.m_lifetime = { 2.f, 2.f };
	return emitter;
}

#define TOY_GI 0

void paint_scene(Gnode& parent)
{
	gfx::directional_light_node(parent);
	gfx::radiance(parent, "radiance/tiber_1_1k.hdr", BackgroundMode::None);

#if TOY_GI
	GIProbe& probe = gfx::gi_probe(parent);
	//probe.debug_draw(parent, GIBaker::DEBUG_LIGHT);

	static size_t counter = 0;
	if(++counter == 50)
		probe.bake();
#endif
}