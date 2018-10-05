#pragma once
#include <utility>

namespace CldRuntime {
	class CldMem {
	public:
		template<typename T>
		inline static T* Malloc(size_t size) {
			return reinterpret_cast<T*>(new uint8_t[size]);
		}

		template<typename T>
		inline static T* Calloc(size_t size) {
			auto ptr = Malloc<T>(size);
			memset(ptr, 0, size);
			return ptr;
		}

		template<typename T, typename... Args>
		inline static T* New(Args&&... args) {
			return new (Malloc<T>(sizeof(T))) T(std::forward<Args>(args)...);
		}

		template<typename T>
		inline static void Delete(T * t) {
			delete t;
		}
	};
}