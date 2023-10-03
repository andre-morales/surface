#pragma once
/*#include <typeinfo>
#include <typeindex>

namespace Glass {
	namespace detail {
		// SFINAE helper. Value evalutes to true if T has a member named parent_t
		template <class T>
		class has_parent_type {
			struct One { char a[1]; };
			struct Two { char a[2]; };

			template <class U>
			static One foo(U::parent_t*);

			template <class U>
			static Two foo(...);

		public:
			static const bool value = sizeof(foo<T>(nullptr)) == sizeof(One);
		};

		template <class T,
			typename = std::enable_if<!has_parent_type<T>::value>::type>
		consteval int parentCount() {
			return 0;
		}

		template < class T, class P = T::parent_t,
			typename = std::enable_if<has_parent_type<T>::value>::type>
		consteval int parentCount() {
			return parentCount<P>() + 1;
		}

	}

	template <class T>
	consteval int parentCount() {
		return Glass::detail::parentCount<T>();
	}

	template <class T>
	consteval std::vector<std::type_index> parents() {
		std::vector<std::type_index> vec;
		Glass::detail::parents<T>(vec);
		return vec;
	}
}*/