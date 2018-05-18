-- toy engine
-- toy jam example applications

function jamProject(name, ...)
    project("ex_" .. name)
        kind "ConsoleApp"

        includedirs {
            path.join(TOY_DIR, "example"),
            path.join(MUD_DIR, "example"),
        }

        mud_module(name, path.join(TOY_DIR, "jams"), name, "_EX_" .. name:upper())
    
		for _, depname in ipairs({...}) do
            mud_module(depname, path.join(TOY_DIR, "jams"), depname, "_EX_" .. depname:upper())
        end
    
        uses_toy()
        toy_binary(name)
        
        configuration { "asmjs" }
            linkoptions {
                "--preload-file ../../../data/examples/ex_" .. name .. "@data/",
            }
        
        configuration {}
end

jamProject("space")
jamProject("platform")--, "05_character")
jamProject("blocks")--, "05_character")

