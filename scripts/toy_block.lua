-- toy engine
-- toy block module

project "toy_block"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
	}

	files {
        path.join(TOY_BLOCK_DIR, "**.h"),
        path.join(TOY_BLOCK_DIR, "**.cpp"),
	}
                        
    defines { "TOY_BLOCK_LIB" }
    
    links {
		"toy_core",
		"toy_visu",
		"toy_edit",
	}
    
    uses_mud_gfx()
    uses_mud()
    