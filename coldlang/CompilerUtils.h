#pragma once

#include <set>
#include <functional>

namespace Compile {
	class CompilerUtils {
	public:
		template<typename T,
			template<typename U> typename Container>
		static inline
			std::set<T> Select(const Container<T> & container, std::function<bool(T)> judge) {
			std::set<T> result;
			for (const T & t : container) {
				if (judge(t)) {
					result.emplace(static_cast<T>(t));
				}
			}
			return result;
		}
	};
}