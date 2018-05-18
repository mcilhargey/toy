
#include <platform/Generated/Types.h>
#include <platform/Generator.h>
#include <platform/ex_platform.h>

#include <toy/toy.h>

void platform_generator(GameShell& shell, VisualScript& script)
{
	script.lock();

	Valve& height = script.value(4);
	Valve& tiles = script.value(20);
	Valve& gridSize = script.create<uvec3>({ &tiles, &height, &tiles });

	Valve& coords = script.function(grid, { &gridSize });

	std::string path = shell.m_visu_system->m_gfx_system->locate_file("models/platform/platform.tls", { "" });
	std::string filepath = path + "models/platform/platform.tls";
	Valve& tileset = script.function(parse_json_wave_tileset, { &script.value(filepath), &script.value(std::string("")) });

	//Valve& empty_wave = script.create<TileWave>({ &tileset, &tiles, &height, &tiles, &script.value(false) });
	//Valve& solve_steps = script.value(0);
	//Process& solve = script.method(&Wave::solve).plug({ &empty_wave, &solve_steps });
	//Valve& solve_result = *solve.m_outputs[0];
	//Valve& wave = *solve.m_outputs[1];
	//Valve& wave = empty_wave;

	Valve& empty_world = script.create<Worldblock>({ &script.value(0U), &script.input("origin"), &script.value(Zero3), &gridSize, &script.value(Unit3), &tileset });
	//Valve& world = script.method(&Worldblock::update, { &empty_world, &wave });

	script.unlock();
}

VisualScript& platform_generator(GameShell& shell)
{
	static Signature signature = { { { "world", Ref(type<World>()) },{ "origin", Ref(type<Origin>()) } } };
	static VisualScript generator = { "Generator", signature };
	platform_generator(shell, generator);
	return generator;
}
