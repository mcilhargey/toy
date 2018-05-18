#pragma once

#include <mud/mud.h>

#include <snd/Generated/Module.h>
#include <util/Generated/Module.h>
#include <db/Generated/Module.h>
#include <core/Generated/Module.h>
#include <visu/Generated/Module.h>
#include <block/Generated/Module.h>
#include <shell/Generated/Module.h>

#if MUD_PLATFORM_EMSCRIPTEN
#define TOY_RESOURCE_PATH "/data/"
#endif
