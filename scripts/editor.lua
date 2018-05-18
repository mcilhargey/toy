-- toy engine
-- toy editor application

project "editor"
	kind "ConsoleApp"

	files {
        path.join(TOY_EDITOR_DIR, "main.cpp"),
	}
    
    uses_toy()
    toy_binary("editor")
    

    