#pragma once
#include <string>
#include <optional>
#include <algorithm>
#include <functional>

namespace beleg
{
	namespace sfinae
	{
		template <typename T>
		struct has_const_iterator
		{
		private:
			template <typename C> static std::true_type & test(typename C::const_iterator*);
			template <typename C> static std::false_type & test(...);
		public:
			static const bool value = sizeof(test<T>(nullptr)) == sizeof(std::true_type);
		};

		template <typename T>
		struct is_map_like
		{
		private:
			template <typename C> static std::true_type & test(typename C::const_iterator::value_type::second_type*);
			template <typename C> static std::false_type & test(...);
		public:
			static const bool value = sizeof(test<T>(nullptr)) == sizeof(std::true_type);
		};

		template<typename T, typename D, typename = void>
		struct is_equality_comparable : std::false_type { };
		template<typename T, typename D>
		struct is_equality_comparable<T, D,
			typename std::enable_if<
			true,
			decltype(std::declval<T&>() == std::declval<D&>(), (void)0)
			>::type
		> : std::true_type {};
	}
	namespace helpers
	{
		namespace strings
		{
			inline std::string toLower(std::string str)
			{
				std::transform(str.begin(), str.end(), str.begin(), ::tolower);
				return str;
			}
			inline std::string toUpper(std::string str)
			{
				std::transform(str.begin(), str.end(), str.begin(), ::toupper);
				return str;
			}
		}
		namespace containers
		{
			template <typename T, typename W,
				typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type, W>::value &&
				!std::is_same<T, std::string>::value
				>* = nullptr
			>
				bool contains(T& container, W what)
			{
				auto found = std::find_if(container.begin(), container.end(), [&](auto item)
				{
					return item == what;
				});
				return found != container.end();
			}
			template <typename T, typename W, std::enable_if_t < std::is_same<T, std::string>::value >* = nullptr>
			bool contains(T str, W what)
			{
				return str.find(what) != std::string::npos;
			}

			template <typename T, typename W,
				typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type::second_type, W>::value &&
				!std::is_same<T, std::string>::value
				>* = nullptr
			>
				bool containsItem(T& container, W what)
			{
				auto found = std::find_if(container.begin(), container.end(), [&](auto item)
				{
					return item.second == what;
				});
				return found != container.end();
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type::first_type, W>::value &&
				!std::is_same<T, std::string>::value
			>* = nullptr>
				bool containsKey(T& container, W what)
			{
				return container.find(what) != container.end();
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				T map(T container, std::function<typename T::const_iterator::value_type(typename T::const_iterator::value_type&)> func)
			{
				std::for_each(container.begin(), container.end(), [&](auto& item)
				{
					item = func(item);
				});
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				T filter(T container, std::function<bool(typename T::const_iterator::value_type&)> func)
			{
				T newContainer;
				std::for_each(container.begin(), container.end(), [&](auto& item)
				{
					if (func(item))
					{
						newContainer.insert(newContainer.end(), item);
					}
				});
				return newContainer;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				void forEach(T container, std::function<void(typename T::const_iterator::value_type&)> func)
			{
				std::for_each(container.begin(), container.end(), [&](auto& item)
				{
					func(item);
				});
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> find(T& container, std::function<bool(typename T::const_iterator::value_type&)> func)
			{
				auto item = std::find_if(container.begin(), container.end(), [&](auto it)
				{
					return func(it);
				});
				if (item != container.end())
					return item;
				return std::nullopt;
			}
			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> findItem(T& container, std::function<bool(typename T::const_iterator::value_type::second_type&)> func)
			{
				auto item = std::find_if(container.begin(), container.end(), [&](auto it)
				{
					return func(it.second);
				});
				if (item != container.end())
					return item;
				return std::nullopt;
			}
			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> findKey(T& container, std::function<bool(typename T::const_iterator::value_type::first_type&)> func)
			{
				auto item = std::find_if(container.begin(), container.end(), [&](auto it)
				{
					return func(it.first);
				});
				if (item != container.end())
					return item;
				return std::nullopt;
			}
		}
	}
	namespace extensions
	{
		namespace strings
		{
			struct toLower {};
			inline std::string operator|(std::string str, toLower)
			{
				return helpers::strings::toLower(str);
			}
			inline std::string operator|(const char* str, toLower)
			{
				return helpers::strings::toLower(std::string(str));
			}
			struct toUpper {};
			inline std::string operator|(std::string str, toUpper)
			{
				return helpers::strings::toUpper(str);
			}
			inline std::string operator|(const char* str, toUpper)
			{
				return helpers::strings::toUpper(str);
			}
		}
		namespace containers
		{
			template <typename T> struct contains { T what; contains(T what) : what(what) {} };
			template <typename T, typename W, std::enable_if_t<std::is_same<T, std::string>::value && (std::is_same<W, std::string>::value || std::is_same<const char*, W>::value)>* = nullptr>
			bool operator|(T str, contains<W> what)
			{
				return helpers::containers::contains(str, std::is_same<W, std::string>::value ? what.what : std::string(what.what));
			}
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type, W>::value &&
				!std::is_same<T, std::string>::value
			>* = nullptr>
				bool operator|(T& container, contains<W> what)
			{
				return helpers::containers::contains(container, what.what);
			}

			template <typename T> struct containsItem { T what; containsItem(T what) : what(what) {} };
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type::second_type, W>::value &&
				!std::is_same<T, std::string>::value
			>* = nullptr>
				bool operator|(T& container, containsItem<W> what)
			{
				return helpers::containers::containsItem(container, what.what);
			}

			template <typename T> struct containsKey { T what; containsKey(T what) : what(what) {} };
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_equality_comparable<typename T::const_iterator::value_type::first_type, W>::value &&
				!std::is_same<T, std::string>::value
			>* = nullptr>
				bool operator|(T& container, containsKey<W> what)
			{
				return helpers::containers::containsKey(container, what.what);
			}

			template <typename T> struct map { T func; map(T func) : func(func) {} };
			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				T operator|(T container, map<F> transfrm)
			{
				return helpers::containers::map(container, transfrm.func);
			}

			template <typename T> struct filter { T func; filter(T func) : func(func) {} };
			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				T operator|(T container, filter<F> transfrm)
			{
				return helpers::containers::filter(container, transfrm.func);
			}

			template <typename T> struct forEach { T func; forEach(T func) : func(func) {} };
			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				void operator|(T container, forEach<F> transfrm)
			{
				return helpers::containers::forEach(container, transfrm.func);
			}

			template <typename T> struct find { T func; find(T func) : func(func) {} };
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> operator|(T& container, find<W> what)
			{
				return helpers::containers::find(container, what.func);
			}

			template <typename T> struct findItem { T func; findItem(T func) : func(func) {} };
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> operator|(T& container, findItem<W> what)
			{
				return helpers::containers::findItem(container, what.func);
			}

			template <typename T> struct findKey { T func; findKey(T func) : func(func) {} };
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<T>()))>,
				typename = std::decay_t<decltype(*end(std::declval<T>()))>,
				std::enable_if_t<
				sfinae::has_const_iterator<T>::value &&
				sfinae::is_map_like<T>::value
			>* = nullptr>
				std::optional<typename T::const_iterator> operator|(T& container, findKey<W> what)
			{
				return helpers::containers::findKey(container, what.func);
			}
		}
	}
}