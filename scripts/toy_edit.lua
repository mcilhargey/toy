-- toy engine
-- toy edit module

project "toy_edit"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
	}

	files {
        path.join(TOY_EDIT_DIR, "**.h"),
        path.join(TOY_EDIT_DIR, "**.cpp"),
    }                                                 

    defines { "TOY_EDIT_LIB" }
    
    links {
		"toy_core",
		"toy_visu",
	}
    
    uses_mud_gfx()
    uses_mud()