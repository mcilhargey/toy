//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.
#ifndef TOOLBOX_H
#define TOOLBOX_H

/* toy */
#include <edit/Generated/Forward.h>
#include <core/Store/Array.h>
#include <core/Store/StoreObserver.h>
#include <core/Entity/Entity.h>
#include <edit/Generated/Types.h>

#include <map>
#include <string>

using namespace mud; namespace toy
{
	using string = std::string;
	using Selection = std::vector<Ref>;

	class _refl_ Toolbox : public NonCopy
	{
	public:
		Toolbox(cstring name);
		virtual ~Toolbox();

		string m_name;
		std::vector<object_ptr<Tool>> m_tools;
	};

	class _refl_ DynamicToolbox : public Toolbox
	{
	public:
		DynamicToolbox(cstring name, const Selection& targets);

		void update_tools(const Selection& targets);

	protected:
		std::vector<Tool*> m_current_tools;

		const Selection& m_targets;
	};

	class _refl_ Toolbelt : public NonCopy
	{
	public:
		Toolbelt();

		std::map<string, object_ptr<Toolbox>> m_toolboxes;

		Tool* m_activeTool;
		Brush* m_activeBrush;

		Toolbox& toolbox(cstring name);

		void setActiveTool(Tool& tool);
	};
}

#endif // EDITORACTION_H
