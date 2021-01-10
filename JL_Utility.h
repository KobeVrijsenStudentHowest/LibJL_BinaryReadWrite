// Utility - Utility items

/*
   Copyright (C) 2021 Kobe Vrijsen

   this file is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 3.0 of the License, or (at your option) any later version.

   This file is made available in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, see
   <https://www.gnu.org/licenses/>.

   Author :   KoneLinx
   Contact:   konelinx@posteo.net
*/

#pragma once

#include <utility>
#include <type_traits>
#include <array>

namespace JL::utility
{

	using std::begin;
	using std::end;

	using std::data;
	using std::size;

	using std::addressof;

#if (__cplusplus < 202002L)

	// C++20 to_address

	template <typename It>
	constexpr auto to_address(It* it) noexcept -> decltype(&*it)
	{
		return &*it;
	}

	template <typename It>
	constexpr auto to_address(It const& it) noexcept(noexcept(it.operator->())) -> decltype(to_address(it.operator->()))
	{
		return to_address(it.operator->());
	}

	using std::is_pod;
	using std::is_pod_v;

#else

	using std::to_address;

	//C++20 is_pod deprecation

	template <typename T>
	using is_pod = std::bool_constant<std::is_trivial<T>::value && std::is_standard_layout<T>::value>;

	template <typename T>
	constexpr bool is_pod_v = is_pod<T>::value;

#endif

	template <typename T>
	constexpr char* bytes(T* p) noexcept
	{
		return reinterpret_cast<char *>(p);
	}

	template <typename T>
	constexpr char const* bytes(T const* p) noexcept
	{
		return reinterpret_cast<char const*>(p);
	}

	template <typename T>
	constexpr char const* cbytes(T const* p) noexcept
	{
		return bytes(p);
	}



	template<typename ...>
	using void_t = void;

	template<typename T, typename ...>
	using requires_t = T;


	namespace detail
	{
		template <template<typename...> typename Temp, typename ... T>
		constexpr std::true_type  is_template_helper(Temp<T...>) noexcept { return {}; };

		template <typename T>
		constexpr std::false_type is_template_helper(T) noexcept { return {}; };
	}

	template <typename T>
	struct is_template : decltype(detail::is_template_helper(std::declval<T>())) {};

	template <typename T>
	constexpr bool is_template_v = is_template<T>::value;



	template <typename T, typename = void>
	struct is_container : std::false_type {};

	template <typename T>
	struct is_container<T, void_t<decltype(begin(std::declval<T>())), decltype(end(std::declval<T>()))>> : std::true_type {};

	template <typename T>
	constexpr bool is_container_v = is_container<T>::value;


	template <typename T, typename = void>
	struct is_contiguous_container : std::false_type {};

	template <typename T>
	struct is_contiguous_container<T, void_t<decltype(data(std::declval<T>()))>> : std::bool_constant<is_container_v<T>> {};

	template <typename T>
	constexpr bool is_contiguous_container_v = is_contiguous_container<T>::value;

}