#pragma once
#include "RuntimeObject.h"

namespace CldRuntime {
	struct RuntimeStackSlot {
		union Slot {
			RuntimeValue base_value;
			IntegerValue integer_value;
			FloatValue float_value;
			BooleanValue boolean_value;
			NoneValue none_value;
			PointerValue pointer_value;
			Slot() {
				this->none_value = NoneValue();
			}
		} value;
		RuntimeStackSlot() 
			: value()
		{

		}
	};

	struct RuntimeStack {

	};
}