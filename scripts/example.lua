-- toy engine
-- toy example modules

project "toy_example"
	kind "ConsoleApp"
    
    includedirs {
        path.join(TOY_DIR, "example"),
        path.join(TOY_DIR, "src"),
        path.join(TOY_DIR, "jams"),
    }
    
    --mud_module(name, path.join(TOY_DIR, "example"), name)
        
	files {
        path.join(TOY_DIR, "example", "**.h"),
        path.join(TOY_DIR, "example", "**.cpp"),
	}
    
    removefiles {
        path.join(MUD_DIR, "example", "ui_example.h"),
        path.join(MUD_DIR, "example", "ui_example.cpp"),
    }
    
    defines { "TOY_VG_RENDERER" }
    
    uses_toy()
    toy_binary("toy_example")
    
function toy_example(name, deps)
    project(name)
        kind "ConsoleApp"

        mud_module(false, "", name, path.join(TOY_DIR, "example"), name)
    
		for _, depname in ipairs(deps) do
            mud_module(false, "", depname, path.join(TOY_DIR, "example"), depname)
        end
    
        uses_toy()
        toy_binary(name)
end

toy_example("05_character", { "03_materials" })
--toy_example("15_script", { "01_shapes", "03_materials" })
--toy_example("16_visual_script", { "01_shapes", "03_materials" })
--toy_example("17_proc_gen")
--toy_example("18_pathfinding")
--toy_example("19_multi_viewport")
--toy_example("20_meta", { "01_shapes", "03_materials" })

