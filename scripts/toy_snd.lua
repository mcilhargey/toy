-- toy engine
-- toy snd module

project "toy_snd"
	kind "SharedLib"

	includedirs {
		path.join(TOY_SRC_DIR),
        path.join(TOY_3RDPARTY_DIR, "vorbis", "include"),
        path.join(TOY_3RDPARTY_DIR, "ogg", "include"),
        path.join(MUD_DIR, "scripts/3rdparty/ogg/"),
	}

	files {
        path.join(TOY_SND_DIR, "**.h"),
        path.join(TOY_SND_DIR, "**.cpp"),
	}
                        
    defines { "TOY_SOUND_LIB" }
    
    uses_mud()
    
    links {
		"vorbis",
		"vorbisfile",
		"ogg",
        "OpenAL32",
	}
    
