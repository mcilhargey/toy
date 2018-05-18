-- toy engine
-- toy shell module

project "toy_shell"
	kind "SharedLib"
    
	includedirs {
		path.join(TOY_SRC_DIR),
	}

	files {
        path.join(TOY_SHELL_DIR, "**.h"),
        path.join(TOY_SHELL_DIR, "**.cpp"),
	}
    
    defines { "TOY_SHELL_LIB" }
    
    links {
		"toy_core",
		"toy_edit",
		"toy_visu",
		"toy_block",
		"toy_gen",
	}
    
    configuration { "toy-sound" }
        links {
            "toy_snd",
        }

	configuration {}

    uses_mud_gfx()
    uses_mud()
    
    