-- toy engine
-- toy visu module

project "toy_visu"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
	}

	files {
        path.join(TOY_VISU_DIR, "**.h"),
        path.join(TOY_VISU_DIR, "**.cpp"),
	}
    
    defines { "TOY_VISU_LIB" }
    
    links {
		"toy_core",
	}
    
    configuration { "toy-sound" }
        links {
            "toy_snd",
        }
        
    configuration { "not toy-sound" }
        removefiles {
            path.join(TOY_VISU_DIR, "Sound", "**.h"),
            path.join(TOY_VISU_DIR, "Sound", "**.cpp"), 
        }
        
	configuration {}
    
    uses_mud_gfx()
    uses_mud()
    