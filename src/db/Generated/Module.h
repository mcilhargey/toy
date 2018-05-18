
#pragma once

#include <obj/System/System.h>

#include <obj/Generated/Module.h>
#include <math/Generated/Module.h>
#include <util/Generated/Module.h>
        
#include <db/Generated/Forward.h>
#include <db/Generated/Types.h>

#include <db/Database.h>
#include <db/SqliteDatabase.h>
#include <db/SqliteLoader.h>

namespace toy
{
	class TOY_DB_EXPORT toydb : public Module
	{
	private:
		toydb();

	public:
		static toydb& module() { static toydb instance; return instance; }
	};
}

#ifdef TOY_DB_MODULE
extern "C"
TOY_DB_EXPORT Module& getModule();
#endif
