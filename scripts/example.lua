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
    
function exampleProject(name, ...)
    project(name)
        kind "ConsoleApp"

        mud_module(name, path.join(TOY_DIR, "example"), name, "_" .. name:upper())
    
		for _, depname in ipairs({...}) do
            mud_module(depname, path.join(TOY_DIR, "example"), depname, "_" .. depname:upper())
        end
    
        uses_toy()
        toy_binary(name)
end

exampleProject("05_character", "03_materials")
--exampleProject("15_script", "01_shapes", "03_materials")
--exampleProject("16_visual_script", "01_shapes", "03_materials")
--exampleProject("17_proc_gen")
--exampleProject("18_pathfinding")
--exampleProject("19_multi_viewport")
--exampleProject("20_meta", "01_shapes", "03_materials")

