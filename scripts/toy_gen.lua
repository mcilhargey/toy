-- toy engine
-- toy gen module

project "toy_gen"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
        path.join(TOY_3RDPARTY_DIR, "FastNoise"),
	}

	files {
        path.join(TOY_GEN_DIR, "**.h"),
        path.join(TOY_GEN_DIR, "**.cpp"),
        path.join(TOY_3RDPARTY_DIR, "FastNoise", "**.h"),
        path.join(TOY_3RDPARTY_DIR, "FastNoise", "**.cpp"),
	}
                        
    defines { "TOY_GEN_LIB" }
    
    links {
		"toy_core",
	}
    
    uses_mud()
