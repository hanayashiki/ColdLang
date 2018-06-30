#include "stdafx.h"

namespace tutils
{
	TreeUnitBuilder optional(TreeUnitBuilder tub)
	{
		tub.set_optional(true);
		return tub;
	}
	TreeUnitBuilder negative(TreeUnitBuilder tub)
	{
		tub.set_not(true);
		return tub;
	}
}