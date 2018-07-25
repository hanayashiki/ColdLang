#include "stdafx.h"
#include "CLObject.h"

namespace IR
{
	CLObject::CLObject()
	{
	}


	void CLObject::set_id(size_t id)
	{
		id_ = id;
	}

	size_t CLObject::get_id()
	{
		return id_;
	}

	CLObject::~CLObject()
	{
	}
}