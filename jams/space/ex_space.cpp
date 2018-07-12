
#include <space/ex_space.h>
#include <toy/toy.h>

#include <meta/space/Convert.h>
#include <meta/space/Module.h>
#include <space/Api.h>
#include <shell/Shell.h>

#include <space/Generator.h>
#include <space/Scene.h>
#include <space/Ui.h>

using namespace mud;
using namespace toy;

float g_hullWeight[8] = { 2.f, 6.f, 20.f, 50.f, 100.f, 150.f, 200.f, 250.f };

void fleet_potential(const std::vector<CombatFleet>& flotilla, float* damage, float* metal)
{
	for(const CombatFleet& combat_fleet : flotilla)
	{
		Fleet& fleet = *combat_fleet.m_fleet;

		for(auto& kv : fleet.m_ships)
		{
			ShipSchema& ship = *kv.first;
			int number = kv.second;

			for(size_t i = 0; i < 8; ++i)
			{
				float race_modifier = 100.f - c_race_factors[size_t(fleet.m_commander->m_race)].m_spatial_combat * 100.f;
				float stance_modifier = fleet_stance_modifier(fleet.m_stance, CombatType::Spatial, fleet.m_fought);

				float modifier = 100.f + fleet.m_commander->m_command
					+ race_modifier
					+ stance_modifier
					+ fleet.m_commander->level(Technology::Piloting) * 5.f
					+ fleet.m_experience;

				damage[i] += ship.m_spatial.m_values[i] * modifier;
				metal[i] += g_hullWeight[ship.m_size];
			}
		}
	}
}

void fleet_losses(std::vector<CombatFleet>& flotilla, float ratio)
{
	for(CombatFleet& combat_fleet : flotilla)
	{
		combat_fleet.m_damage = ratio;

		for(auto& kv : combat_fleet.m_fleet->m_ships)
		{
			ShipSchema& ship = *kv.first;

			float damage = max(0.f, ratio - ship.m_resistance * 0.01f);
			uint32_t losses = uint32_t(combat_fleet.m_fleet->m_ships[&ship] * damage);

			combat_fleet.m_losses[&ship] = losses;
			combat_fleet.m_hull_losses[ship.m_size] += losses;
		}
	}
}

void resolve_combat(SpatialCombat& combat)
{
	float first_damage[8] = {};
	float first_metal[8] = {};

	float second_damage[8] = {};
	float second_metal[8] = {};

	fleet_potential(combat.m_allies, first_damage, first_metal);
	fleet_potential(combat.m_enemies, second_damage, second_metal);

	float first = 0.f;
	float second = 0.f;

	for(size_t i = 0; i < 8; ++i)
	{
		first += first_damage[i] / second_metal[i];
		second += second_damage[i] / first_metal[i];
	}

	float total = first + second;

	fleet_losses(combat.m_allies, first / total);
	fleet_losses(combat.m_enemies, second / total);
}

void SpatialCombat::apply_losses()
{
	auto apply = [](CombatFleet& combat_fleet)
	{
		for(auto& kv : combat_fleet.m_fleet->m_ships)
		{
			ShipSchema& ship = *kv.first;
			combat_fleet.m_fleet->add_ships(ship, -combat_fleet.m_losses[&ship]);
		}
	};

	for(CombatFleet& combat_fleet : m_allies)
		apply(combat_fleet);
	for(CombatFleet& combat_fleet : m_enemies)
		apply(combat_fleet);
}

void gather_allies(Fleet& fleet, CombatType combat_type, std::vector<CombatFleet>& allies, std::vector<CombatFleet>& enemies)
{
	for(Fleet* other : fleet.galaxy().m_grid.m_fleets[fleet.m_coord])
	{
		if(other->m_commander->allied(*fleet.m_commander))
			allies.push_back({ *other });
		else
			enemies.push_back({ *other });
	}
}

void spatial_combat(Turn& turn, Galaxy& galaxy, Fleet& fleet)
{
	SpatialCombat combat = { fleet.m_coord, fleet.m_entity.m_last_tick };

	gather_allies(fleet, CombatType::Spatial, combat.m_allies, combat.m_enemies);

	if(!combat.m_enemies.empty())
	{
		resolve_combat(combat);
		galaxy.m_combats.push_back(combat);
		turn.m_spatial_combats.push_back(&galaxy.m_combats.back());
	}
}

void planetary_combat(Turn& turn, Galaxy& galaxy, Fleet& fleet)
{
	UNUSED(turn); UNUSED(galaxy); UNUSED(fleet);
}

void extract_resources(Turn& turn, Commander& commander, Star& star)
{
	auto mining_factor = [&](Commander& commander)
	{
		return 1.f + commander.level(Technology::Mining) * 0.10f + c_race_factors[size_t(commander.m_race)].m_mining;
	};

	for(Resource resource = Resource(0); resource != Resource::Count; resource = Resource(size_t(resource) + 1))
	{
		bool is_mining = (resource == Resource::Minerals || resource == Resource::Andrium);
		int extracted = star.m_resources[size_t(resource)] * star.m_extractors[size_t(resource)];
		extracted = int(extracted * (is_mining ? mining_factor(commander) : 1.f));
		if(extracted > 0)
		{
			star.m_stocks[size_t(resource)] += extracted;
			turn.add_item(Stage::Systems, commander, "System " + star.m_name + " extracted " + to_string(extracted) + " " + to_string(resource));
		}
	}
}

void commit_construction(Turn& turn, Commander& commander, Star& star, Construction& construction)
{
	int number = construction.m_number; // @todo clamp number depending on actual resources

	star.m_stocks[size_t(Resource::Minerals)] -= int(construction.m_schema->m_minerals * number);
	star.m_stocks[size_t(Resource::Andrium)] -= int(construction.m_schema->m_andrium * number);
	commander.m_centaures -= construction.m_schema->m_cost * number;

	if(construction.m_destination)
	{
		construction.m_destination->add_ships(*static_cast<ShipSchema*>(construction.m_schema), number);
		turn.add_item(Stage::Constructions, commander, "Construction of " + to_string(number) + " " + construction.m_schema->m_code + " for fleet " + construction.m_destination->m_name);
	}
	else
	{
		star.m_buildings[static_cast<BuildingSchema*>(construction.m_schema)] += number;
		turn.add_item(Stage::Constructions, commander, "Construction of " + to_string(number) + " " + construction.m_schema->m_code + " on system " + star.m_name);
	}
}

void advance_constructions(Turn& turn, Commander& commander, Star& star)
{
	for(Construction& construction : star.m_constructions)
	{
		construction.m_turns -= 1;
		if(construction.m_turns == 0)
			commit_construction(turn, commander, star, construction);
	}
}

void grow_population(Turn& turn, Commander& commander, Star& star)
{
	int growth = int(star.m_population * star.m_environment / 100.f * c_race_factors[size_t(commander.m_race)].m_growth);
	growth = min(growth, star.m_max_population - star.m_population);
	if(growth > 0)
	{
		star.m_population += growth;
		turn.add_item(Stage::Systems, commander, "Population of " + star.m_name + " grew by " + to_string(growth) + " units");
	}
}

void collect_taxes(Turn& turn, Commander& commander, Star& star)
{
	float revenue = star.m_population * (star_revenue(star.m_taxation) + commander.level(Technology::Administration) * 0.02f);
	commander.m_centaures += revenue;
	turn.add_item(Stage::Systems, commander, "System " + star.m_name + " collected " + to_string(revenue) + "C in taxes");
}

void service_fleet(Turn& turn, Commander& commander, Fleet& fleet)
{
	float upkeep_factor = (100.f + eco_energy(commander.level(Technology::EcoEnergy)) / 100.f);
	float upkeep = fleet.m_upkeep * upkeep_factor;
	commander.m_centaures -= upkeep;
	turn.add_item(Stage::Payments, commander, "Fleet " + fleet.m_name + " servicing cost " + to_string(upkeep) + "C");
}

void advance_technology(Turn& turn, Commander& commander, Technology tech, TechDomain& domain)
{
	commander.m_centaures -= domain.m_budget;
	domain.m_points += int(domain.m_budget * c_race_factors[size_t(commander.m_race)].m_research);
	int level = techno_level(domain.m_points);
	if(level != domain.m_level)
	{
		domain.m_level = level;
		turn.add_item(Stage::Technology, commander, "Technology " + to_string(tech) + " has reached level " + to_string(level));
	}
}

void next_turn(Turn& turn, Galaxy& galaxy)
{
	for(Commander* commander : galaxy.m_commanders)
		for(Fleet* fleet : commander->m_fleets)
			if(fleet->m_jump.m_state == FleetJump::START)
			{
				fleet->jump();
			}

	for(Commander* commander : galaxy.m_commanders)
		for(Fleet* fleet : commander->m_fleets)
		{
			if(fleet->m_stance == FleetStance::SpatialAttack)
				spatial_combat(turn, galaxy, *fleet);

			if(fleet->m_stance == FleetStance::PlanetaryAttack)
			{
				spatial_combat(turn, galaxy, *fleet);
				planetary_combat(turn, galaxy, *fleet);
			}
		}

	for(Commander* commander : galaxy.m_commanders)
	{
		for(Star* star : commander->m_stars)
		{
			if(star->m_revolt)
				continue;

			grow_population(turn, *commander, *star);
			collect_taxes(turn, *commander, *star);
			extract_resources(turn, *commander, *star);
			advance_constructions(turn, *commander, *star);
		}

		for(Fleet* fleet : commander->m_fleets)
			service_fleet(turn, *commander, *fleet);

		//for(TechDomain& tech : commander->m_technology)
		for(Technology tech = Technology(0); tech != Technology::Count; tech = Technology(size_t(tech) + 1))
			advance_technology(turn, *commander, tech, commander->m_technology[size_t(tech)]);

		commander->update_scans();
	}
}

BuildingDatabase::BuildingDatabase()
	: SchemaDatabase<BuildingSchema>()
{
	m_schemas.push_back({ "ARME", "Usine d'armement", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "CENTRECULT", "Centre culturel", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "CHANTIER", "Chantier naval", "Galactacorp", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "CHORDI", "Chaine d'assemblage d'ordinateurs", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "CHROBOT", "Chaine d'assemblage de robots", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "COLOMBIE", "Ferme de Colombie", "-", 1U, 100.00f, 80.f, 0.f });
	m_schemas.push_back({ "COUTURE", "Maison de haute couture", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "DISTIL", "Distillerie", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "DRESSAGE", "Centre de dressage", "-", 1U, 200.00f, 100.f, 0.f });
	m_schemas.push_back({ "ENRICH", "Usine d'enrichissement des métaux", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "FERME", "Ferme", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "MEDIC", "Laboratoire médical", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "MINE", "Mine de minerai", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "POSTE", "Poste commercial orbital", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "RAFFIN", "Raffinerie", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "RETRAIT", "Usine de retraitement du minerai", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "SPHERE", "Sphère de vie", "-", 1U, 200.00f, 40.f, 0.f });
	m_schemas.push_back({ "SYNTHANDRI", "Usine de synthétisation d'Andrium", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "USINEPLAST", "Usine de matériaux plastiques", "-", 1U, 50.00f, 10.f, 0.f });
	m_schemas.push_back({ "FLEUR", "Maison des fleurs", "-", 2U, 300.00f, 30.f, 0.f });
	m_schemas.push_back({ "BOUCITE", "Boucliers de cités", "-", 3U, 200.00f, 20.f, 0.f });
	m_schemas.push_back({ "RECONV", "Centre de reconversion", "-", 3U, 300.00f, 50.f, 0.f });
	m_schemas.push_back({ "ACADEMIE", "Académie navale", "-", 4U, 800.00f, 300.f, 0.f });
}

ShipDatabase::ShipDatabase()
	: SchemaDatabase<ShipSchema>()
{
	m_schemas.push_back({ 1U, "sonde", "Sonde standard", "Galactacorp", 1U,
		19.00f, 25.00f, 0.f,
		6.00f, {}, 1.00f, 9U, 15U });

	m_schemas.push_back({ 1U, "bom", "Bombardier standard", "Galactacorp", 1U,
		19.00f, 25.00f, 0.f,
		6.00f, {}, 1.00f, 8U, 0U });

	m_schemas.push_back({ 1U, "car", "Cargo standard", "Galactacorp", 1U,
		43.00f, 41.00f, 0.f,
		0.00f, {}, 1.00f, 8U, 0U });

	m_schemas.push_back({ 1U, "cha", "Chasseur standard", "Galactacorp", 1U,
		11.00f, 25.00f, 0.f,
		1.00f, { 10.f, 8.f, 6.f, 4.f, 2.f, 0.f, 0.f, 0.f }, 1.00f, 8U, 0U, { 0, 0, 2, 0, 0 } });

	m_schemas.push_back({ 1U, "chabom", "Chasseur-bombardier standard", "Galactacorp", 1U,
		15.00f, 25.00f, 0.f,
		3.50f, { 5.f, 4.f, 3.f, 2.f, 1.f, 0.f, 0.f, 0.f }, 1.00f, 8U, 0U, { 0, 0, 1, 0, 0 } });

	m_schemas.push_back({ 1U, "colo", "Colonisateur", "Galactacorp", 1U,
		3.00f, 21.00f, 0.f,
		6.00f, {}, 1.00f, 8U, 0U });

	m_schemas.push_back({ 2U, "cor", "Corvette standard", "Galactacorp", 1U,
		87.00f, 69.00f, 0.f,
		4.00f, { 10.f, 8.f, 9.f, 13.f, 20.f, 24.f, 27.f, 30.f }, 4.00f, 7U, 0U, { 0, 0, 2, 2, 2 } });

	m_schemas.push_back({ 2U, "fregimp", "Corvette standard", "Galactacorp", 1U,
		369.00f, 104.00f, 0.f,
		16.00f, { 50.f, 46.f, 50.f, 62.f, 82.f, 52.f, 36.f, 26.f }, 10.00f, 7U, 4U, { 0, 0, 10, 10, 0 } });

	m_schemas.push_back({ 2U, "sbom", "Super-bombardier standard", "Galactacorp", 1U,
		53.00f, 63.00f, 0.f,
		18.00f, {}, 2.00f, 7U, 0U });

	m_schemas.push_back({ 2U, "sta", "Station spatiale Rakrarior", "n/a", 1U,
		1206.00f, 295.00f, 0.f,
		110.50f, { 100.f, 103.f, 152.f, 247.f, 388.f, 322.f, 276.f, 253.f }, 22.00f, 7U, 4U });

	m_schemas.push_back({ 3U, "cro", "Croiseur standard", "Galactacorp", 2U,
		408.00f, 194.00f, 0.f,
		13.00f, { 10.f, 16.f, 32.f, 58.f, 94.f, 64.f, 42.f, 28.f }, 14.00f, 7U, 4U });

	m_schemas.push_back({ 3U, "des", "Destroyer standard", "Galactacorp", 2U,
		308.00f, 192.00f, 0.f,
		11.00f, { 30.f, 32.f, 43.f, 63.f, 92.f, 56.f, 33.f, 18.f }, 10.00f, 7U, 4U });

	m_schemas.push_back({ 3U, "freg", "Fregate standard", "Galactacorp", 2U,
		296.00f, 192.00f, 0.f,
		11.00f, { 50.f, 44.f, 43.f, 47.f, 56.f, 32.f, 21.f, 14.f }, 10.00f, 7U, 4U });

	m_schemas.push_back({ 4U, "scro", "Supercroiseur standard", "Galactacorp", 2U,
		766.00f, 461.00f, 0.f,
		58.50f, { 30.f, 39.f, 69.f, 120.f, 192.f, 138.f, 99.f, 75.f }, 20.00f, 6U, 4U });

	m_schemas.push_back({ 7U, "STA2", "Station Spatiale Rakrarior Evolution", "n/a", 2U,
		1300.00f, 920.00f, 0.f,
		110.50f, { 310.f, 315.f, 455.f, 731.f, 1142.f, 945.f, 809.f, 742.f }, 29.80f, 11U, 10U });

}

Universe::Universe(const std::string& name)
	: Complex(0, type<Universe>(), m_bullet_world, *this)
	, m_world(0, *this, name)
	, m_bullet_world(m_world)
{}

Universe::~Universe()
{
	m_world.destroy();
}

Galaxy::Galaxy(Id id, Entity& parent, const vec3& position, const uvec2& size)
	: Complex(id, type<Galaxy>(), *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_size(size)
	, m_scale(1)
	, m_plane(Y3, 0.5f)
	, m_highlight(UINT32_MAX)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);

	as<Universe>(m_entity.m_world.m_complex).m_galaxies.push_back(this);
}

uvec2 Galaxy::intersect_coord(Ray ray)
{
	vec3 intersect = plane_segment_intersection(m_plane, { ray.m_start, ray.m_end });
	uvec3 coord = floor(intersect); // / m_period;
	return{ coord.x, coord.z };
}

Quadrant::Quadrant(Id id, Entity& parent, const vec3& position, const uvec2& coord, float size)
	: Complex(id, type<Quadrant>(), m_emitter, m_world_page, m_buffer_page, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_world_page(m_entity, m_emitter, true, vec3(size, size, size))
	, m_buffer_page(m_entity, m_world_page, true)
	, m_coord(coord)
	, m_size(size)
{
	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);

	as<Galaxy>(m_entity.m_parent->m_complex).m_quadrants.push_back(this);
}

static size_t star_count = 0;

Star::Star(Id id, Entity& parent, const vec3& position, const uvec2& coord, const std::string& name)
	: Complex(id, type<Star>(), m_emitter, m_receptor, m_active, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_coord(coord)
	, m_name(name)
	, m_resources{}
{
	m_entity.m_world.add_task(this, short(Task::GameObject));

	m_emitter.addSphere(VisualMedium::me(), 0.1f);
	//m_receptor.addSphere(VisualMedium::me(), 50.f);

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);

	this->galaxy().m_stars.push_back(this);
	//as<Quadrant>(*m_entity.m_parent->m_construct).m_stars.push_back(this);
}

Star::~Star()
{
	m_entity.m_world.remove_task(this, short(Task::GameObject));
}

Galaxy& Star::galaxy() { return m_entity.m_parent->part<Galaxy>(); } // as<Galaxy>(*m_entity.m_parent->m_construct)

void Star::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick); UNUSED(delta);
	float speed = 0.001f;
	m_visu.m_period = fmod(m_visu.m_period + delta * speed, 2 * c_pi);

	for(VisuPlanet& planet : m_visu.m_planets)
	{
		planet.m_period = fmod(planet.m_period + delta * speed * planet.m_speed, 2 * c_pi);
	}
}

void Star::add_construction(Schema& schema, int number, Fleet* destination)
{
	m_constructions.push_back({ &schema, number, destination, construction_time(schema.m_level) });
}

static size_t fleet_count = 0;

Fleet::Fleet(Id id, Entity& parent, const vec3& position, Commander& commander, const uvec2& coord, const std::string& name)
	: Complex(id, type<Fleet>(), m_emitter, m_receptor, m_active, *this)
	, m_entity(id, *this, parent, position, ZeroQuat)
	, m_emitter(m_entity)
	, m_receptor(m_entity)
	, m_active(m_entity)
	, m_commander(&commander)
	, m_coord(coord)
	, m_name(name)
{
	m_entity.m_world.add_task(this, 3); // TASK_GAMEOBJECT

	// @5603 : adding to nested only when object is finish -> in prototype
	m_entity.m_parent->m_contents.add(m_entity);

	m_commander->m_fleets.push_back(this);

	this->galaxy().m_fleets.push_back(this);
	this->galaxy().m_grid.add_fleet(*this, coord);
}

Fleet::~Fleet()
{
	m_entity.m_world.remove_task(this, 3);

	vector_remove(m_commander->m_fleets, this);
}

Galaxy& Fleet::galaxy() { return m_entity.m_parent->part<Galaxy>(); } // as<Galaxy>(*m_entity.m_parent->m_construct)

vec3 Fleet::base_position()
{
	return vec3{ m_coord.x, 0.f, m_coord.y } + 0.5f + Y3;
}

void Fleet::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick); UNUSED(delta); 
	float speed = 0.001f;
	m_visu.m_angle = fmod(m_visu.m_angle + delta * speed, 2 * c_pi);
	m_visu.m_offset = sin(tick * speed);
	m_visu.m_dilate.z = remap_trig(m_visu.m_offset, 0.7f, 1.f);
}

void Fleet::set_ships(ShipSchema& schema, size_t number)
{
	if(number == 0)
		m_ships.erase(&schema);
	else
		m_ships[&schema] = number;

	m_ships_updated = m_entity.m_last_tick + 1;

	this->update_stats();
}

void Fleet::add_ships(ShipSchema& schema, size_t number)
{
	this->set_ships(schema, m_ships[&schema] + number);
}

void Fleet::set_ships(const std::string& code, size_t number)
{
	this->set_ships(ShipDatabase::me().schema(code), number);
}

void Fleet::add_ships(const std::string& code, size_t number)
{
	this->add_ships(ShipDatabase::me().schema(code), number);
}

void Fleet::update_stats()
{
	m_spatial = {};
	m_planetary_power = 0.f;
	m_speed = SIZE_MAX;
	m_scan = 0U;
	m_upkeep = 0.f;

	for(auto& ship_number : m_ships)
	{
		ShipSchema& ship = *ship_number.first;
		float number = ship_number.second;

		//m_spatial_power += ship.m_spatial;
		m_spatial += ship.m_spatial * number;
		m_planetary_power += ship.m_planetary * number;
		m_speed = min(m_speed, ship.m_speed);
		m_scan = max(m_scan, ship.m_scan);
		m_upkeep += ship.m_cost * ship.m_upkeep_factor * 0.1f;
	}
}

static inline FleetJump::State jump_done(Fleet& fleet, float elapsed) { UNUSED(fleet); return FleetJump::DONE; }
static inline FleetJump::State jump_start(Fleet& fleet, float elapsed) { if(elapsed > 0.5f) { fleet.jump(); return FleetJump::JUMP; } else return FleetJump::START; }
static inline FleetJump::State jump_jump(Fleet& fleet, float elapsed) { UNUSED(fleet); if(elapsed > 0.25f) return FleetJump::END; else return FleetJump::JUMP; }
static inline FleetJump::State jump_end(Fleet& fleet, float elapsed) { UNUSED(fleet); if(elapsed > 0.5f) return FleetJump::DONE; else return FleetJump::END; }

using StateFunc = FleetJump::State(*)(Fleet&, float);
static constexpr StateFunc s_fleet_states[4] = { jump_done, jump_start, jump_jump, jump_end };

void FleetJump::update(Fleet& fleet, size_t tick)
{
	float elapsed = float(tick - m_state_updated) * c_tick_interval;
	State old_state = m_state;
	m_state = s_fleet_states[static_cast<size_t>(m_state)](fleet, elapsed);
	m_state_updated = old_state == m_state ? m_state_updated : tick;
}

void Fleet::order_jump(vec2 coord, FleetStance stance)
{
	printf("Fleet %s from commander %s : jump to [%i,%i] in directive %s\n", m_name.c_str(), m_commander->m_name.c_str(), int(coord.x), int(coord.y), to_string(stance).c_str());
	m_jump = { coord, stance, m_entity.m_last_tick };
}

void Fleet::order_split(cstring name, FleetStance stance, std::map<ShipSchema*, size_t> ships)
{
	printf("Fleet %s from commander %s : split fleet %s in directive %s\n", m_name.c_str(), m_commander->m_name.c_str(), name, to_string(stance).c_str());
	m_split = { name, stance, ships, m_entity.m_last_tick };
}

void Fleet::order_attack(Star& star)
{
	printf("Fleet %s from commander %s : attack star %s\n", m_name.c_str(), m_commander->m_name.c_str(), star.m_name.c_str());
}

void Fleet::jump()
{
	uvec2 coord = m_jump.m_destination;
	this->galaxy().m_grid.move_fleet(*this, m_coord, coord);
	m_stance = m_jump.m_stance;

	vec3 position = vec3{ coord.x, 0.f, coord.y } + 0.5f + Y3;
	animate(Ref(&m_entity), member(&Entity::m_position), var(position), 0.25f);

	//m_entity.set_position(m_jump.m_destination);
}

Commander::Commander(Id id, const std::string& name, Race race, int command, int commerce, int diplomacy)
	: m_id(index(type<Commander>(), id, Ref(this)))
	, m_name(name)
	, m_race(race)
	, m_command(command + c_race_factors[size_t(race)].m_command)
	, m_commerce(commerce + c_race_factors[size_t(race)].m_commerce)
	, m_diplomacy(diplomacy + c_race_factors[size_t(race)].m_diplomacy)
{}

Commander::~Commander()
{
	unindex(type<Commander>(), m_id);
}

void Commander::next_frame(size_t tick, size_t delta)
{
	UNUSED(tick); UNUSED(delta);
}

void Commander::update_scans()
{
	m_scans.m_fleets.clear();
	m_scans.m_stars.clear();

	auto scan = [](Galaxy& galaxy, uvec2 coord, uint scan, std::set<Fleet*>& fleets, std::set<Star*>& stars)
	{
		uvec2 lo = coord - min(uvec2(scan), coord);
		uvec2 hi = coord + scan;

		auto intersect = [](uvec2 coord, uvec2 lo, uvec2 hi)
		{
			return coord.x > lo.x && coord.x < hi.x
				&& coord.y > lo.y && coord.y < hi.y;
		};

		for(Fleet* fleet : galaxy.m_fleets)
			if(intersect(fleet->m_coord, lo, hi))
				fleets.insert(fleet);

		for(Star* star : galaxy.m_stars)
			if(intersect(star->m_coord, lo, hi))
				stars.insert(star);
	};

	Galaxy& galaxy = m_fleets[0]->galaxy();
	for(Fleet* fleet : m_fleets)
		scan(galaxy, fleet->m_coord, fleet->m_scan, m_scans.m_fleets, m_scans.m_stars);
	for(Star* star : m_stars)
		scan(galaxy, star->m_coord, star->m_scan, m_scans.m_fleets, m_scans.m_stars);
}

void Commander::take_star(Star& star)
{
	if(star.m_commander)
		vector_remove(star.m_commander->m_stars, &star);
	m_stars.push_back(&star);
	star.m_commander = this;
}

void Commander::take_fleet(Fleet& fleet)
{
	if(fleet.m_commander)
		vector_remove(fleet.m_commander->m_fleets, &fleet);
	m_fleets.push_back(&fleet);
	fleet.m_commander = this;
}

CommanderBrush::CommanderBrush(ToolContext& context)
	: Brush(context, "Commander", type<CommanderBrush>())
	, m_commander(nullptr)
	, m_radius(3.f)
{}

Colour rgb_to_rgba(const Colour& colour, float a)
{
	return{ colour.m_r, colour.m_g, colour.m_b, a };
}

void CommanderBrush::paint(Gnode& parent)
{
	if(!m_commander) return;

	gfx::node(parent, {}, m_position);
	gfx::shape(parent, Circle(m_radius, Axis::Y), Symbol(Colour::White, rgb_to_rgba(m_commander->m_colour, 0.3f)));
}

ToolState CommanderBrush::start()
{
	return ToolState();
	//return SpatialTool::start();
}

void CommanderBrush::update(const vec3& position)
{
	if(!m_commander) return;

	//Galaxy& galaxy = *as<Universe>(*m_context.m_origin->m_world.m_construct).m_galaxies[0];
	//for(Star* star : galaxy.m_stars)
	//	if(distance(star->m_entity.m_position, position) <= m_radius)
	//		m_commander->takeStar(*star);
}

void ex_space_lua_check(GameShell& shell, Galaxy& galaxy)
{
	LuaInterpreter& lua = *shell.m_interpreter;

	lua.set("col", Ref(&Colour::Pink));
	lua.call("print('col.r = ' .. col.r)");
	lua.call("print('col.g = ' .. col.g)");
	lua.call("Colour.Pink.g = 5.93");
	lua.call("print('col.g = ' .. col.g)");
	lua.call("print('Colour.Green.g = ' .. Colour.Green.g)");

	lua.set("galaxy", Ref(&galaxy));
	lua.call("star = galaxy.stars[1]");
	lua.call("fleet = galaxy.fleets[1]");
	lua.call("fleet:order_attack(star)");
	lua.set("coord", var(vec2{ 45.f, 12.f }));
	lua.call("coord = vec2(45, 12)");
	lua.set("stance", var(FleetStance::PlanetaryAttack));
	float x = lua.getx<float>(carray<cstring, 2>{ "coord", "x" });
	printf("cpp -> coord.x = %f\n", x);
	lua.call("print('coord = ' .. coord.x .. ', ' .. coord.y)");
	lua.call("print('stance = ' .. tostring(stance))");
	lua.call("fleet:order_jump(coord, stance)");

	//lua.call("objects = Index.me.indexer().objects");
}

void ex_space_scene(GameShell& app, GameScene& scene, Player& player)
{
	scene.m_scene->painter("Galaxy", [&](size_t, VisuScene&, Gnode& parent) {
		paint_galaxy(parent, *player.m_galaxy);
	});
	scene.m_scene->object_painter<Star>("Stars", player.m_galaxy->m_stars, paint_star);
	scene.m_scene->object_painter<Star>("Stars", player.m_commander->m_stars, paint_scan_star);
	scene.m_scene->object_painter<Star>("Stars", player.m_commander->m_scans.m_stars, paint_scan_star);
	scene.m_scene->object_painter<Fleet>("Fleets", player.m_commander->m_fleets, paint_scan_fleet);
	scene.m_scene->object_painter<Fleet>("Fleets", player.m_commander->m_scans.m_fleets, paint_scan_fleet);
	
	scene.m_scene->painter("Combat", [&](size_t, VisuScene&, Gnode& parent) {
		if(player.m_current_combat)
			paint_combat(parent, *player.m_current_combat);
	});

	scene.m_camera.m_entity.m_position = player.m_commander->m_capital->m_entity.m_position;
}

void ex_space_scene(GameShell& app, GameScene& scene)
{
	Player& player = val<Player>(scene.m_player);
	ex_space_scene(app, scene, player);
}

void ex_space_init(GameShell& app, Game& game)
{
	app.m_visu_system->m_gfx_system->add_resource_path("examples/ex_space/");
	game.m_editor.m_custom_brushes.emplace_back(make_unique<CommanderBrush>(game.m_editor.m_tool_context));
}

void ex_space_start(GameShell& app, Game& game)
{
	GlobalPool::me().create_pool<Universe>();
	GlobalPool::me().create_pool<Galaxy>();
	GlobalPool::me().create_pool<Star>();
	GlobalPool::me().create_pool<Commander>();
	GlobalPool::me().create_pool<Fleet>();
	GlobalPool::me().create_pool<OCamera>();

	Universe& universe = GlobalPool::me().pool<Universe>().construct("Arcadia");
	game.m_world = &universe.m_world;

	app.m_core->section(0).add_task(game.m_world);

	VisualScript& generator = space_generator(app);
	generator(carray<Var, 2>{ Ref(game.m_world), Ref(&game.m_world->origin()) });

	Galaxy& galaxy = *universe.m_galaxies[0];

	static Player player = { &galaxy, galaxy.m_commanders[0] };
	game.m_player = Ref(&player);

	for(Commander* commander : galaxy.m_commanders)
		commander->update_scans();
}

Style& menu_style()
{
	static Style style("GameMenu", styles().wedge, [](Layout& l) { l.m_space = { PARAGRAPH, SHRINK, SHRINK };
																   l.m_align = { CENTER, CENTER };
																   l.m_padding = vec4(15.f);
																   l.m_spacing = vec2(10.f); });
	return style;
}

Style& button_style()
{
	static Style style("GameButton", styles().button, [](Layout& l) {},
													  [](InkStyle& s) { s.m_empty = false; s.m_text_colour = Colour::AlphaWhite; s.m_text_size = 24.f; },
													  [](Style& s) { s.decline_skin(HOVERED).m_text_colour = Colour::White; });
	return style;
}

Viewer& ex_space_menu_viewport(Widget& parent, GameShell& app)
{
	Viewer& viewer = ui::scene_viewer(parent);
	Gnode& scene = viewer.m_scene->begin();

#ifdef TOY_SOUND
	scene.m_sound_manager = app.m_visu_system->m_sound_system.get();
#endif

	paint_viewer(viewer);
	paint_scene(scene);

	static Clock clock;

	Gnode& node = gfx::node(scene, {}, -Y3 * 0.45f, angle_axis(fmod(clock.read(), 2.f * c_pi), Y3), Unit3 * 0.05f);
	Item* item = gfx::model(node, "spaceship");

	//toy::sound(node, "complexambient", true);

	return viewer;
}

void ex_space_menu(Widget& parent, Game& game)
{
	Widget& self = ui::board(parent);

	Viewer& viewer = ex_space_menu_viewport(self, *game.m_shell);
	Widget& overlay = ui::screen(viewer);

	Widget& menu = ui::widget(overlay, menu_style());

	if(ui::button(menu, button_style(), "Start game").activated())
	{
		ex_space_start(*game.m_shell, game);
	}

	ui::button(menu, button_style(), "Continue game");
	ui::button(menu, button_style(), "Quit");
}

void ex_space_pump(GameShell& app, Game& game, Widget& parent, Dockbar* dockbar = nullptr)
{
	UNUSED(dockbar);
	if(!game.m_player)
	{
		ex_space_menu(parent, game);
	}
	else
	{
		static GameScene& scene = app.add_scene();
		scene.m_scene->next_frame();

		ex_space_ui(parent, scene);
	}
}

void ex_space_pump(GameShell& app, Game& game)
{
#ifdef _SPACE_TOOLS
	edit_context(app.m_ui->begin(), app.m_editor, true);
	ex_space_pump(app, game, *app.m_editor.m_screen, *app.m_editor.m_dockbar);
#else
	ex_space_pump(app, game, game.m_screen ? *game.m_screen : app.m_ui->begin());
#endif
}

#ifdef _EX_SPACE_EXE
int main(int argc, char *argv[])
{
	cstring example_path = TOY_RESOURCE_PATH "examples/ex_space/";
	GameShell app(carray<cstring, 2>{ TOY_RESOURCE_PATH, example_path }, argc, argv);

	GameModule module = { _space::m(), &ex_space_init, &ex_space_start, &ex_space_pump, &ex_space_scene };
	app.run_game(module);
}
#endif
