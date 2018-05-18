-- toy toolchain

TOY_DIR        = path.getabsolute("..")
MUD_DIR        = path.join(TOY_DIR, "mud")

TOY_SRC_DIR    = path.join(TOY_DIR, "src")
TOY_CORE_DIR   = path.join(TOY_SRC_DIR, "core")
TOY_SND_DIR    = path.join(TOY_SRC_DIR, "snd")
TOY_EDIT_DIR   = path.join(TOY_SRC_DIR, "editor")
TOY_VISU_DIR   = path.join(TOY_SRC_DIR, "visu")
TOY_BLOCK_DIR  = path.join(TOY_SRC_DIR, "block")
TOY_GEN_DIR    = path.join(TOY_SRC_DIR, "gen")
TOY_SHELL_DIR  = path.join(TOY_SRC_DIR, "shell")
TOY_EDITOR_DIR = path.join(TOY_SRC_DIR, "editor")

TOY_3RDPARTY_DIR = path.join(TOY_DIR, "3rdparty")

dofile(path.join(MUD_DIR, "scripts/toolchain_mud.lua"))

configuration { "windows"}
    libdirs {
        "C:/Program Files (x86)/OpenAL/libs/Win32",
        "C:/Program Files/OpenAL/libs/Win32",
    }
    
    includedirs {
        "C:/Program Files (x86)/OpenAL/include",
        "C:/Program Files/OpenAL/include",
    }

configuration { "vs*-clang" }
    defines {
        "BT_NO_SIMD_OPERATOR_OVERLOADS"
    }
    
configuration {}