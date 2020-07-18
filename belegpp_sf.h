#pragma once
#include "belegpp.h"

namespace beleg
{
	namespace sfinae
	{
		//sf::obj_t
		template <typename T>
		struct is_safe_object
		{
		private:
			template <typename C> static std::true_type & test(typename C::obj_t*);
			template <typename C> static std::false_type & test(...);
		public:
			static const bool value = sizeof(test<typename T::obj_t>(nullptr)) == sizeof(std::true_type);
		};
	}
	namespace helpers
	{
		namespace containers
		{
			template <typename T, typename W,
				typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
				>* = nullptr
			>
				bool contains(T& container, W& what)
			{
				auto found = std::find_if(container->begin(), container->end(), [&](auto item)
				{
					return item == what;
				});
				return found != container->end();
			}

			template <typename T, typename W,
				typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type::second_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
				>* = nullptr
			>
				bool containsItem(T& container, W& what)
			{
				auto found = std::find_if(container->begin(), container->end(), [&](auto item)
				{
					return item.second == what;
				});
				return found != container->end();
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type::first_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
			>* = nullptr>
				bool containsKey(T& container, W& what)
			{
				return container->find(what) != container->end();
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T map(T container, std::function<typename T::obj_t::const_iterator::value_type(typename T::obj_t::const_iterator::value_type&)> func)
			{
				std::for_each(container->begin(), container->end(), [&](auto& item)
				{
					item = func(item);
				});
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T filter(T& container, std::function<bool(typename T::obj_t::const_iterator::value_type&)> func)
			{
				T newContainer;
				std::for_each(container->begin(), container->end(), [&](auto& item)
				{
					if (func(item))
					{
						newContainer->insert(newContainer->end(), item);
					}
				});
				return newContainer;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				void forEach(T& container, std::function<void(typename T::obj_t::const_iterator::value_type&)> func)
			{
				std::for_each(container->begin(), container->end(), [&](auto& item)
				{
					func(item);
				});
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				std::optional<typename T::obj_t::const_iterator> findIf(T& container, std::function<bool(typename T::obj_t::const_iterator::value_type&)> func)
			{
				auto item = std::find_if(container->begin(), container->end(), [&](auto it)
				{
					return func(it);
				});
				if (item != container->end())
					return item;
				return std::nullopt;
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value
			>* = nullptr>
				std::optional<typename T::obj_t::const_iterator> find(T& container, W& what)
			{
				auto item = std::find_if(container->begin(), container->end(), [&](auto it)
				{
					return it == what;
				});
				if (item != container->end())
					return item;
				return std::nullopt;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T& removeIf(T& container, std::function<bool(typename T::obj_t::const_iterator::value_type&)> func)
			{
				container->erase(std::remove_if(container->begin(), container->end(), [&](auto it)
				{
					return func(it);
				}), container->end());
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T reverse(T container)
			{
				std::reverse(container->begin(), container->end());
				return container;
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value
			>* = nullptr>
				T& remove(T& container, W& what)
			{
				container->erase(std::remove(container->begin(), container->end(), what), container->end());
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T& removeAt(T& container, std::size_t index)
			{
				if (container->size() > index && index >= 0)
					container->erase(container->begin() + index);
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T sort(T container, std::function<bool(typename T::obj_t::const_iterator::value_type& first, typename T::obj_t::const_iterator::value_type& second)> func)
			{
				std::sort(container->begin(), container->end(), func);
				return container;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				bool some(T& container, std::function<bool(typename T::obj_t::const_iterator::value_type& first)> func)
			{
				bool some = false;
				std::all_of(container->begin(), container->end(), [&](auto& item)
				{
					if (func(item))
					{
						some = true;
						return false;
					}
					return true;
				});
				return some;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				bool every(T& container, std::function<bool(typename T::obj_t::const_iterator::value_type& first)> func)
			{
				bool every = true;
				std::all_of(container->begin(), container->end(), [&](auto& item)
				{
					if (!func(item))
					{
						every = false;
						return false;
					}
					return true;
				});
				return every;
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T slice(T input, int start, int end)
			{
				if (end == 0) end = input->size();
				if (end < 0)
					end = input->size() + end;
				if (end < 0) end = 0;
				if (end > input->size()) end = input->size();

				if (start < 0) start = input->size() + start;
				if (start < 0) start = 0;

				if (start > end || start == end)
					return {};

				T sliced;
				sliced->assign(input->begin() + start, input->begin() + end);
				return sliced;
			}

			template <typename T, class random = std::mt19937, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T shuffle(T input, random rand = mt)
			{
				std::shuffle(input->begin(), input->end(), rand);
				return input;
			}

			template<typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				!sfinae::is_streamable<typename T::obj_t>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type>::value
			>* = nullptr>
				std::ostream& containerToStream(std::ostream& stream, const T& what)
			{
				for (auto it = what->begin(); it != what->end(); ++it)
				{
					if (it == what->begin())
					{
						stream << "{ ";
					}

					if (std::distance(it, what->end()) == 1)
					{
						stream << *it << " }";
					}
					else
					{
						stream << *it << ", ";
					}
				}
				return stream;
			}

			template<typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				!sfinae::is_streamable<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type::first_type>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type::second_type>::value
			>* = nullptr>
				std::ostream& mapLikeToStream(std::ostream& stream, const T& what)
			{
				for (auto it = what->begin(); it != what->end(); ++it)
				{
					if (it == what->begin())
					{
						stream << "{ ";
					}

					if (std::distance(it, what->end()) == 1)
					{
						stream << "[" << it->first << ", " << it->second << "]" << " }";
					}
					else
					{
						stream << "[" << it->first << ", " << it->second << "]" << ", ";
					}
				}
				return stream;
			}
		}
	}
	namespace extensions
	{
		namespace containers
		{
			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
			>* = nullptr>
				bool operator|(T& container, contains<W> what)
			{
				return helpers::containers::contains(container, what.what);
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type::second_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
			>* = nullptr>
				bool operator|(T& container, containsItem<W> what)
			{
				return helpers::containers::containsItem(container, what.what);
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type::first_type, W>::value &&
				!std::is_same<typename T::obj_t, std::string>::value
			>* = nullptr>
				bool operator|(T& container, containsKey<W> what)
			{
				return helpers::containers::containsKey(container, what.what);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, map<F> transfrm)
			{
				return helpers::containers::map(container, transfrm.func);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, filter<F> transfrm)
			{
				return helpers::containers::filter(container, transfrm.func);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				void operator|(T& container, forEach<F> transfrm)
			{
				return helpers::containers::forEach(container, transfrm.func);
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value
			>* = nullptr>
				std::optional<typename T::obj_t::const_iterator> operator|(T& container, find<W> what)
			{
				return helpers::containers::find(container, what.what);
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				std::optional<typename T::obj_t::const_iterator> operator|(T& container, findIf<W> what)
			{
				return helpers::containers::findIf(container, what.func);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T& operator|(T& container, removeIf<F> what)
			{
				return helpers::containers::removeIf(container, what.func);
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, reverse)
			{
				return helpers::containers::reverse(container);
			}

			template <typename T, typename W, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_equality_comparable<typename T::obj_t::const_iterator::value_type, W>::value
			>* = nullptr>
				T& operator|(T& container, remove<W> what)
			{
				return helpers::containers::remove(container, what.what);
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T& operator|(T& container, removeAt what)
			{
				return helpers::containers::removeAt(container, what.what);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, sort<F> what)
			{
				return helpers::containers::sort(container, what.func);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				bool operator|(T& container, some<F> what)
			{
				return helpers::containers::some(container, what.func);
			}

			template <typename T, typename F, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				bool operator|(T& container, every<F> what)
			{
				return helpers::containers::every(container, what.func);
			}

			template <typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, slice what)
			{
				return helpers::containers::slice(container, what.start, what.end);
			}

			template <typename T, class random = std::mt19937, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value
			>* = nullptr>
				T operator|(T& container, shuffle<random> what)
			{
				return helpers::containers::shuffle(container);
			}

			template<typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				!sfinae::is_streamable<typename T::obj_t>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type>::value
			>* = nullptr>
				std::ostream& operator<<(std::ostream& stream, const T& what)
			{
				return helpers::containers::containerToStream(stream, what);
			}

			template<typename T, typename = std::decay_t<decltype(*begin(std::declval<typename T::obj_t>()))>,
				typename = std::decay_t<decltype(*end(std::declval<typename T::obj_t>()))>,
				std::enable_if_t<sfinae::is_safe_object<T>::value &&
				sfinae::has_const_iterator<typename T::obj_t>::value &&
				sfinae::is_map_like<typename T::obj_t>::value &&
				!sfinae::is_streamable<typename T::obj_t>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type::first_type>::value &&
				sfinae::is_streamable<typename T::obj_t::const_iterator::value_type::second_type>::value
			>* = nullptr>
				std::ostream& operator<<(std::ostream& stream, const T& what)
			{
				return helpers::containers::mapLikeToStream(stream, what);
			}
		}
	}
}