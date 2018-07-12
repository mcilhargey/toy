//  Copyright (c) 2018 Hugo Amiard hugo.amiard@laposte.net
//  This software is provided 'as-is' under the zlib License, see the LICENSE.txt file.
//  This notice and the license may not be removed or altered from any source distribution.


#include <edit/Edit/EditActions.h>

using namespace mud; namespace toy
{
	void Edit::begin()
	{
		//mUser->selector().selection()->select(m_target); // @crash the selection is being iterated
	}

	void Clone::begin()
	{
		//Object& clone = m_object_type.m_meta->creator().cloneObject(m_object);
		//mUser->selector().selection()->select(clone);
	}
}
