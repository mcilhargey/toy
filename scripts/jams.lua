-- toy engine
-- toy jam example applications

function jam_project(name, deps)
    project("ex_" .. name)
        kind "ConsoleApp"

        includedirs {
            path.join(TOY_DIR, "example"),
            path.join(MUD_DIR, "example"),
        }

        mud.jams[name] = mud_module(nil, "_" .. name, path.join(TOY_DIR, "jams"), name, nil, nil, nil, toy.all)
        
        mud_refl(mud.jams[name])
        mud_module_decl(mud.jams[name].refl, false)
        
        mud.jams[name].decl(mud.jams[name], false)
        
		for _, depname in ipairs(deps or {}) do
            mud.examples[depname].decl(mud.examples[depname], false)
        end
        
        toy_binary(name)
        
        configuration { "asmjs" }
            linkoptions {
                "--preload-file ../../../data/examples/ex_" .. name .. "@data/",
            }
        
        configuration {}
end

mud.jams = {}

jam_project("space")
jam_project("platform")--, "05_character")
jam_project("blocks")--, "05_character")

