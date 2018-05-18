
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
#include <geom/Generated/Module.h>
#include <lang/Generated/Module.h>
#include <util/Generated/Module.h>
#include <core/Generated/Module.h>
#include <ui/Generated/Module.h>
#include <uio/Generated/Module.h>
#include <gfx/Generated/Module.h>
#include <visu/Generated/Module.h>
        
#include <editor/Generated/Forward.h>
#include <editor/Generated/Types.h>

#include <editor/Editor.h>
#include <editor/Controller/CameraController.h>
#include <editor/Controller/FPSCameraController.h>
#include <editor/Controller/RTSCameraController.h>
#include <editor/Edit/EditActions.h>
#include <editor/Edit/Editor.h>
#include <editor/Edit/ReactiveEdit.h>
#include <editor/Edit/Viewport.h>
#include <editor/Editor/Brush.h>
#include <editor/Editor/Editor.h>
#include <editor/Editor/Toolbox.h>

namespace toy
{
	class TOY_EDITOR_EXPORT toyeditor : public Module
	{
	private:
		toyeditor();

	public:
		static toyeditor& module() { static toyeditor instance; return instance; }
	};
}

#ifdef TOY_EDITOR_MODULE
extern "C"
TOY_EDITOR_EXPORT Module& getModule();
#endif
