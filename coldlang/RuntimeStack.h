#pragma once
#include "RuntimeObject.h"

namespace CldRuntime {
	struct RuntimeStackSlot {
		union {
			IntegerValue integer_value;
			FloatValue float_value;
			BooleanValue boolean_value;
			NoneValue none_value;
			PointerValue pointer_value;
		} value;
	};

	struct RuntimeStack {

	};
}