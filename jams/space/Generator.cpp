
#include <space/Generated/Types.h>
#include <space/Generator.h>
#include <space/ex_space.h>
#include <toy/toy.h>

uvec2 to_coord(uvec3 coord)
{
	return{ coord.x, coord.z };
}

static const std::string consonnants = "bcdfghjklmnpqrstvwxz";
static const std::string vowels = "aeiouy";

std::string generate_name()
{
	std::string name = "";

	size_t length = random_integer(5U, 15U);
	bool vowel = false;
	for(size_t i = 0; i < length; ++i)
	{
		vowel = random_scalar(0.f, 1.f) > (vowel ? 0.8f : 0.2f);
		const std::string& chars = vowel ? consonnants : vowels;
		size_t c = random_integer(size_t(0U), chars.size() - size_t(1U));
		name += toupper(chars[c]);
	}

	return name;
}

void generate_avatar(Colour& colour, Image256& avatar)
{
	const uint16_t side = 6;
	avatar.resize(side, side);

	float r = random_scalar(0.f, 1.f);
	float g = random_scalar(0.f, 1.f);
	float b = random_scalar(0.f, 1.f);

	colour = { r, g, b, 1.f };

	Palette palette = { { Colour::None, colour } };
	avatar.m_palette = palette;

	uint16_t halfside = side / 2;
	for(uint16_t x = 0; x < halfside; ++x)
		for(uint16_t y = 0; y < side; ++y)
		{
			bool full = random_scalar(0.f, 1.f) >= 0.5f;
			avatar.at(x, y) = (full ? 1 : 0);
			avatar.at(side - 1 - x, y) = (full ? 1 : 0);
		}
}

Star* generate_system(Entity& galaxy, const uvec3& coord, const vec3& position)
{
	int roll = random_integer(0, 100);
	if(roll < 85)
		return nullptr;

	Star& star = galaxy.constructNested<Star>(position, to_coord(coord), generate_name()).part<Star>();
	return &star;
}

Fleet* generate_fleet(Entity& galaxy, const uvec3& coord, const vec3& position)
{
	return nullptr;
}

Commander* generate_commander(Entity& galaxy, Star& star)
{
	int roll = random_integer(0, 100);
	if(roll < 85)
		return nullptr;

	/*
	static bool once = true;
	if(once)
		once = false;
	else
		return nullptr;
	*/

	int command = random_integer(0, 100);
	int commerce = random_integer(0, 100);
	int diplomacy = random_integer(0, 100);

	vec3 traits = { float(command), float(commerce), float(diplomacy) };
	traits = normalize(traits);

	command = int(traits.x * 90.f);
	commerce = int(traits.y * 90.f);
	diplomacy = int(traits.z * 90.f);

	Commander& commander = GlobalPool::me().pool<Commander>().construct(0, generate_name(), command, commerce, diplomacy);
	commander.take_star(star);

	generate_avatar(commander.m_colour, commander.m_avatar);

	Fleet& fleet = galaxy.constructNested<Fleet>(star.m_entity.m_position + 1.0f * Y3, commander, star.m_coord, generate_name()).part<Fleet>();

	float size = random_scalar(1.f, 3.f);
	fleet.set_ships("cha", 20 * size);// 40);
	fleet.set_ships("chabom", 10 * size);// 40);
	fleet.set_ships("bom", 10 * size);// 40);
	fleet.set_ships("cor", 5 * size);// 40);

	return &commander;
}

Star* assign_system(Entity& galaxy, Star& star, std::vector<Commander*> commanders)
{
	return nullptr;
}

void space_generator(GameShell& shell, VisualScript& script)
{
	script.lock();

	// Sectors
	Valve& parsecs_per_sector = script.value(20);
	Valve& sectors_per_galaxy = script.value(2);
	Valve& parsecs_per_galaxy = script.value(20 * 2);

	Valve& galaxySectors = script.create<uvec3>({ &sectors_per_galaxy, &script.value(1), &sectors_per_galaxy });
	Valve& galaxySize = script.create<uvec3>({ &parsecs_per_galaxy, &script.value(1), &parsecs_per_galaxy });
	Valve& galaxySize2 = script.create<uvec2>({ &parsecs_per_galaxy, &parsecs_per_galaxy });

	Valve& sectorCoords = script.function(grid, { &galaxySectors });
	Valve& sectorPositions = script.function(grid_center, { &sectorCoords, &parsecs_per_sector });

	Valve& origin = script.input("origin");

	Valve& galaxy = script.create<Galaxy>({ &script.value(0U), &origin, &script.value(Zero3), &galaxySize2 });

	// @ todo this test case fails when we remove the virtual destructor in TypeObject. Think of a design that adresses this
	/*
	script.unlock();
	Galaxy& gala = val<Galaxy>(galaxy.m_process.m_outputs[0]->m_stream.m_value);
	Ref ref = { &gala };
	Complex& complex = val<Construct>(ref).m_stem;
	Ref ref2 = { &complex };
	Entity& wrong_address = val<Entity>(ref2);
	Entity& correct_address = *static_cast<Entity*>(&val<Complex>(ref2));
	*/

	//Valve& sectors = script.create<Quadrant>().pipe({ &script.value(0U), &galaxy, &sectorPositions, &sectorCoords, &sectorParsecs });

	Valve& coords = script.function(grid, { &galaxySize });
	Valve& positions = script.function(grid_center, { &coords, &script.value(1) });

	Valve& systems = script.function(generate_system, { &galaxy, &coords, &positions });

	Valve& commanders = script.function(generate_commander, { &galaxy, &systems });

	//Valve& assigned_systems = script.function(assignSystem).pipe({ &galaxy, &systems, &commanders });

	//Valve& fleets = script.function(generateFleet).pipe({ &galaxy, &coords, &positions });

	// Create sectors

	script.unlock();
}

VisualScript& space_generator(GameShell& shell)
{
	static Signature signature = { { { "world", Ref(type<World>()) }, { "origin", Ref(type<Origin>()) } } };
	static VisualScript generator = { "Generator", signature };
	space_generator(shell, generator);
	return generator;
}
