-- toy engine

-- known build issues :
-- glfw3 on ubuntu is installed as libglfw.so (and not libglfw3.so)
-- if yacc is not found it will generate an empty glcpp-lexer.c file

group "3rdparty"
dofile(path.join(TOY_DIR, "scripts/3rdparty/bullet.lua"))

dofile(path.join(MUD_DIR, "scripts/mud.lua"))

function toy_core()
	includedirs {
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include"),
        path.join(TOY_3RDPARTY_DIR, "bullet", "src"),
	}

	files {
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Include", "*.h"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Recast", "Source", "*.cpp"),
        
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Include", "*.h"),
        path.join(TOY_3RDPARTY_DIR, "recast", "Detour", "Source", "*.cpp"),
	}
    
    links {
        "LinearMath",
        "BulletCollision",
        "BulletDynamics",
	}
end

function uses_toy_core()
	includedirs {
        TOY_SRC_DIR,
	}

    links {
        "LinearMath",
        "BulletCollision",
        "BulletDynamics",
    }
end

function toy_visu()
    if not _OPTIONS["sound"] then
        removefiles {
            path.join(TOY_VISU_DIR, "Sound", "**.h"),
            path.join(TOY_VISU_DIR, "Sound", "**.cpp"), 
        }
    end
end

toy = {}

--                           base   name        root path    sub path   decl        self decl       decl transitive     dependencies
toy.util        = mud_module("toy", "util",     TOY_SRC_DIR, "util",    nil,        nil,            nil,                { mud.obj, mud.math })
toy.core        = mud_module("toy", "core",     TOY_SRC_DIR, "core",    nil,        toy_core,       uses_toy_core,      { mud.obj, mud.proto, mud.math, mud.geom, toy.util })
if _OPTIONS["sound"] then
    toy.visu    = mud_module("toy", "visu",     TOY_SRC_DIR, "visu",    nil,        toy_visu,       nil,                { mud.obj, mud.snd, mud.gfx, toy.util, toy.core })
else
    toy.visu    = mud_module("toy", "visu",     TOY_SRC_DIR, "visu",    nil,        toy_visu,       nil,                { mud.obj, mud.gfx, toy.util, toy.core })
end
toy.editor      = mud_module("toy", "editor",   TOY_SRC_DIR, "editor",  nil,        nil,            nil,                { mud.obj, toy.util, toy.core, toy.visu }) -- table.merge(mud.all, 
toy.block       = mud_module("toy", "block",    TOY_SRC_DIR, "block",   nil,        nil,            nil,                { mud.obj, toy.core, toy.visu, toy.edit })

toy.modules = { toy.util, toy.core, toy.visu, toy.editor, toy.block }
table.extend(toy.modules, mud_refls({ toy.util, toy.core, toy.visu, toy.editor, toy.block }, FORCE_REFL_PROJECTS))

toy.core.aliases = { ['toy::string'] = 'std::string' }

toy.shell       = mud_module("toy", "shell",    TOY_SRC_DIR, "shell",   nil,        nil,            nil,                table.merge(mud.all, { toy.core, toy.visu, toy.edit, toy.block }))
table.insert(toy.modules, toy.shell)
table.insert(toy.modules, mud_refl(toy.shell, false))

group "lib"
if _OPTIONS["as-libs"] then
    group "lib/toy"
        for _, m  in ipairs(toy.modules) do
            m.decl(m, true)
        end
    group "lib"
else
    project "toy"
        if MUD_STATIC then
            kind "StaticLib"
        else
            kind "SharedLib"
        end
        
        for _, m  in ipairs(toy.modules) do
            m.decl(m, false)
        end
        
        files {
            path.join(TOY_SRC_DIR, "toy",    "**.h"),
        }
end

toy.all = {}
table.extend(toy.all, mud.all)
table.extend(toy.all, toy.modules)

group "bin"
--dofile(path.join(TOY_DIR, "scripts/shell.lua"))
