-- toy engine
-- toy core module

project "toy_core"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include"),
        path.join(TOY_3RDPARTY_DIR, "bullet", "src"),
	}

	files {
        path.join(TOY_CORE_DIR, "**.h"),
        path.join(TOY_CORE_DIR, "**.cpp"),
        
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include", "*.h"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Source", "*.cpp"),
        
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include", "*.h"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Source", "*.cpp"),
	}
                        
    defines { "TOY_CORE_LIB" }
    
    uses_mud()
    
    links {
        "LinearMath",
        "BulletCollision",
        "BulletDynamics",
	}
    
