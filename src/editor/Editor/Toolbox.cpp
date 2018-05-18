//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Generated/Types.h>
#include <editor/Editor/Toolbox.h>

#include <edit/Tool.h>
#include <edit/Brush.h>

#include <core/Selector/Selection.h>

using namespace mud; namespace toy
{
	Toolbox::Toolbox(cstring name)
		: m_name(name)
		, m_tools()
	{}

	Toolbox::~Toolbox()
	{}

	DynamicToolbox::DynamicToolbox(cstring name, const Selection& targets)
		: Toolbox(name)
		, m_current_tools()
		, m_targets(targets)
	{
		//m_targets.observe(*this);
	}

	void DynamicToolbox::update_tools(const Selection& targets)
	{
		UNUSED(targets);
		m_current_tools.clear();

		for(auto& tool : m_tools)
			if(tool->enabled())
			//if(tool->test_target())
				vector_add(m_current_tools, tool.get());
	}

	Toolbelt::Toolbelt()
		: m_toolboxes()
		, m_activeTool(nullptr)
		, m_activeBrush(nullptr)
	{}

	Toolbox& Toolbelt::toolbox(cstring name)
	{
		if(!m_toolboxes[name])
			m_toolboxes[name] = make_object<Toolbox>(name);
		return *m_toolboxes[name];
	}

	void Toolbelt::setActiveTool(Tool& tool)
	{
		if(m_activeTool)
			m_activeTool->deactivate();

		m_activeTool = &tool;

		if(is<Brush>(tool))
			m_activeBrush = &as<Brush>(tool);
	}
}
