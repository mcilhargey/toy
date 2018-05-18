
#include <blocks/Generated/Types.h>
#include <blocks/Generator.h>
#include <blocks/ex_blocks.h>

#include <toy/toy.h>

void paint_block_height(Block& block, Image256& image)
{
	block.reset();

	size_t subdiv = block.subdiv();
	for(size_t y = 0; y < subdiv; ++y)
		for(size_t x = 0; x < subdiv; ++x)
		{
			size_t height = image.at(x, y);
			for(size_t z = 0; z <= height; ++z)
				block.chunk(x, z, y, Earth::me());
		}
}

void paint_block_elements(Block& block, Image256& image)
{
	size_t subdiv = block.subdiv();
	for(size_t y = 0; y < subdiv; ++y)
		for(size_t x = 0; x < subdiv; ++x)
			for(size_t z = 0; z < subdiv; ++z)
			{
				size_t colour = image.at(x, y);
				Element* element = block.m_chunks.gridAt(x, z, y);

				if(element != &Earth::me())
					continue;

				if(colour == 0)
					block.chunk(x, z, y, Gas::me());
				else if(colour == 1)
					block.chunk(x, z, y, Minerals::me());
				else if(colour == 2)
					block.chunk(x, z, y, Fungus::me());
			}
}

void block_generator(GameShell& shell, VisualScript& script)
{
	using Var = Valve;

	script.lock();

	// Create sectors

	// @BUG doesn't work with 1
	Var& gridSize = script.value(2U);
	Var& gridSize3 = script.create<uvec3>({ &gridSize, &script.value(1U), &gridSize });
		
	Var& sectorSize = script.value(50.f);
	Var& sectorSize2 = script.create<vec2>({ &sectorSize });

	Var& minusHalfSectorSize = script.function(divide<float>, { &sectorSize, &script.value(-2.f) });

	Var& sectorCoords = script.function(grid, { &gridSize3 });
	Var& sectorPositions = script.function(grid_center, { &gridSize3, &sectorSize });

	Var& origin = script.getInput("origin");

	Var& sectors = script.create<Sector>({ &script.value(0), &origin, &sectorPositions, &sectorCoords, &sectorSize });
	Var& blocks = script.create<OBlock>({ &script.value(0), &sectors, &script.value(Zero3), &script.reference(type<Block>()), &script.value(0), &sectorSize });

	Var& grid = script.get(&BlockWorld::m_grid, { &script.getInput("world") });
	script.function(index_blocks, { &script.value(ivec3(2, 1, 2)), &grid, &blocks, &sectors }, { SM_NONE, SM_NONE, SM_FLATTEN, SM_FLATTEN });

	script.set(&BlockWorld::m_sectors, { &script.getInput("world"), &sectors }, { SM_NONE, SM_FLATTEN });

	// Create chunks

	// Fractal

	Var& blockSide = script.value(20U);

	Var& chunkSize = script.function(multiply<float>, { &gridSize, &blockSide });

	Var& levels = script.value(2U);
	Var& precision = script.value(0.5f);

	Var& tabs = script.value(75U);

	// Height
	Process* blockheight = nullptr;
	{
		Var& fract = script.create<Fract>({ &tabs });

		Var& palette = script.create<Palette>({ &script.value(SPECTRUM_GRAYSCALE), &levels });

		Var& pattern = script.create<Pattern>({ &palette, &script.value(PATTERN_X), &precision });

		{
			Var& sample = script.method(&Fract::renderWhole, { &fract, &pattern, &chunkSize, &chunkSize });

			script.node<ProcessDisplay>().plug({ &sample });
		}


		// One Fractal Sample per Block

		{
			Var& sample = script.method(&Fract::renderGrid, { &fract, &gridSize, &pattern, &blockSide, &blockSide });

			blockheight = &script.node<ProcessFunction>(function(paint_block_height)).plug({ &blocks, &sample });
		}

		//Process& finishedblocks = script.method(&Block::commit).plug({ &blocks });
	}

	// image node
	// synthesis node

	// Create heaps
	if(1)
	{
		Var& fract = script.create<Fract>({ &tabs }); // .flow(blockheight->out_flow())

		Var& elements = script.value(std::vector<Element*>{ &Gas::me(), &Minerals::me(), &Fungus::me() });

		Var& colours = script.get(&Element::m_colour, { &elements });

		// @todo : solve how to read 3 stream branches as one vector
		//Var& palette = script.create<Palette>(1)
		//	.pipe({ &colours }, { SM_FLATTEN });

		Var& palette = script.value(Palette({ Gas::me().m_colour, Minerals::me().m_colour, Fungus::me().m_colour }));

		Var& pattern = script.create<Pattern>({ &palette, &script.value(PATTERN_X), &precision });
			
		{
			Var& sample = script.method(&Fract::renderWhole, { &fract, &pattern, &chunkSize, &chunkSize });

			script.node<ProcessDisplay>().plug({ &sample });
		}

		Var& sample = script.method(&Fract::renderGrid, { &fract, &gridSize, &pattern, &blockSide, &blockSide });

		//Process& blockelements = script.function(paintBlockElements).plug({ &blocks, &sample });

		// Process& finishedblocks = script.method(&Block::commit).plug({ &blocks }, {}, &blockelements.flow());

		script.method(&Block::commit, { &blocks }); // Process& finishedblocks = 

		//m_heapsFract.cullUnder(m_heapsFract.mainSample(), m_levelFract.mainSample(), 0);

		Var& circlify = script.create<Circlifier>({ &sample });

		Var& blockScale = script.function(divide<float>, { &sectorSize, &blockSide });

		Var& circles = script.method(&Circlifier::compute, { &circlify, &colours, &blockScale }); //.combine_flow(0, 1)
				
		// deactivated since Shape::m_center is not exposed by reflection
#if 0
		Var& pos0 = script.get(&Circle::m_center, { &circles });

		Var& size_offset = script.create<vec3>({ &minusHalfSectorSize });
		Var& pos1 = script.function(add<vec3>, { &pos0, &size_offset });

		Var& radiuses = script.get(&Circle::m_radius, { &circles });

		Var& pos2 = script.method(&WorldPage::groundPoint, { &sectors, &pos1, &script.value(true) }); //.flow(finishedblocks.out_flow()

		Var& height = script.get(&vec3::y, { &pos2 });

		Var& halfRadius = script.function(divide<float>, { &radiuses, &script.value(5.f) });

		Var& newheight = script.function(add<float>, { &height, &halfRadius });

		//Var& pos3 = script.set(&vec3::y{ &pos2, &newheight });

		Var& heaps = script.create<Heap>({ &script.value(0), &sectors, &pos2, &elements, &radiuses });
#endif
	}

	// Create crates
	//if(0)
	{
		Var& radius = script.value(15.f);
		Var& positions = script.function(distribute_poisson, { &script.value(vec2{ 50.f }), &radius }); // .flow(sectors

		Var& extents = script.value(vec3(0.75f, 0.5f, 0.4f));
		Var& crates = script.create<Crate>({ &script.value(0), &sectors, &positions, &extents });
	}

	// Create characters
	//if(0)
	{
		Var& radius = script.value(20.f);
		//Var& positions = script.function(distribute_poisson{ &sectorSize2, &radius });
		Var& positions = script.function(distribute_poisson, { &script.value(vec2{ 50.f }), &radius }); // .flow(sectors
			
		Var& crates = script.create<Human>({ &script.value(0), &sectors, &positions, &script.value(0.35f), &script.value(2.f),  
													&script.value(std::string("Human")), &script.value(std::string("X")) });
	}

	// Create houses

	// Create objects

	script.unlock();
}

VisualScript& block_generator(GameShell& shell)
{
	static Signature signature = { { { "world", Ref(type<World>()) },{ "origin", Ref(type<Origin>()) } } };
	static VisualScript generator = { "Generator", signature };
	block_generator(shell, generator);
	return generator;
}
