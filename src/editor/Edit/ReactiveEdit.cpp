//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <editor/Generated/Types.h>
#include <editor/Edit/ReactiveEdit.h>

#include <uio/Unode.h>
#include <uio/Unode.h>

#include <core/Reactive/Reactive.h>

using namespace mud; namespace toy
{
	void edit_reactive(Widget& parent, Reactive& reactive)
	{
		for(auto& behavior : reactive.m_behaviors)
			object_hook(parent, behavior.get());
	}
}
