-- toy engine

-- known build issues :
-- glfw3 on ubuntu is installed as libglfw.so (and not libglfw3.so)
-- if yacc is not found it will generate an empty glcpp-lexer.c file

group "3rdparty"
dofile(path.join(MUD_DIR, "scripts/3rdparty/ogg/ogg.lua"))
dofile(path.join(MUD_DIR, "scripts/3rdparty/vorbis/vorbis.lua"))
dofile(path.join(MUD_DIR, "scripts/3rdparty/vorbis/vorbisfile.lua"))
dofile(path.join(TOY_DIR, "scripts/3rdparty/bullet.lua"))

dofile(path.join(MUD_DIR, "scripts/mud.lua"))

group "lib"
if _OPTIONS["toy-libs"] then
    group "lib/toy"
    print 'CACA'
    dofile(path.join(TOY_DIR, "scripts/toy_core.lua"))
    if _OPTIONS["toy-sound"] then
        dofile(path.join(TOY_DIR, "scripts/toy_snd.lua"))
    end
    dofile(path.join(TOY_DIR, "scripts/toy_edit.lua"))
    dofile(path.join(TOY_DIR, "scripts/toy_visu.lua"))
    dofile(path.join(TOY_DIR, "scripts/toy_block.lua"))
    dofile(path.join(TOY_DIR, "scripts/toy_gen.lua"))
    dofile(path.join(TOY_DIR, "scripts/toy_shell.lua"))
    group "lib"
else
    function uses_toy()
        includedirs {
            path.join(TOY_SRC_DIR),
        }
        
        if project().name ~= "toy" then
            links { "toy" }
        end
        
        links {
            "LinearMath",
            "BulletCollision",
            "BulletDynamics",
        }
        
        uses_mud_gfx()
        uses_mud()
    end

    project "toy"
        kind "SharedLib"
        
        includedirs {
            path.join(TOY_SRC_DIR),
            path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include"),
            path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include"),
            path.join(TOY_3RDPARTY_DIR, "bullet", "src"),
            path.join(MUD_3RDPARTY_DIR, "sqlite3"),
        }

        mud_module("util",   TOY_SRC_DIR, "util",   "TOY_UTIL")
        mud_module("db",     TOY_SRC_DIR, "db",     "TOY_DB")
        mud_module("core",   TOY_SRC_DIR, "core",   "TOY_CORE")
        mud_module("visu",   TOY_SRC_DIR, "visu",   "TOY_VISU")
        mud_module("editor", TOY_SRC_DIR, "editor", "TOY_EDITOR")
        mud_module("block",  TOY_SRC_DIR, "block",  "TOY_BLOCK")
        mud_module("shell",  TOY_SRC_DIR, "shell",  "TOY_SHELL")
        
        files {
            path.join(TOY_SRC_DIR, "toy",    "**.h"),
            path.join(MUD_3RDPARTY_DIR, "sqlite3", "*.c"),
            path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include", "*.h"),
            path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Source", "*.cpp"),
            path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include", "*.h"),
            path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Source", "*.cpp"),
        }
        
        removefiles {
            path.join(TOY_EDITOR_DIR, "main.cpp"),
        }
    
        defines { "TOY_LIB" }
        
        uses_toy()

        configuration { "sound" }
            defines { "TOY_SOUND" }
            
        configuration {}
end

function toy_binary(name)
    mud_binary(name)
    
    configuration { "not linux", "not asmjs" }
        defines {
            "TOY_RESOURCE_PATH=\"" .. path.join(TOY_DIR, "data") .. "/\"",
        }

    configuration { "linux", "not asmjs" }
        defines {
            "TOY_RESOURCE_PATH=\\\"" .. path.join(TOY_DIR, "data") .. "/\\\"",
        }

	configuration {}
end

group "bin"
dofile(path.join(TOY_DIR, "scripts/shell.lua"))
