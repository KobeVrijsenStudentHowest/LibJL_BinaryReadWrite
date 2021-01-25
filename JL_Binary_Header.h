// Binary - JL template library, binary reading and writing

/*
   Copyright (C) 2021 KoneLinx

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

#include "JL_Binary_Base.h"

namespace JL_BINARY
{

	// Result

	using Result = bool;

	constexpr bool failed(Result const& r) noexcept
	{
		return r == true;
	}


	// read

	template <typename Object, typename Stream>
	auto read(Stream& stream, Object& object) -> detail::enable_if_parsable_t<Object, Result>;


	// write

	template <typename Object, typename Stream>
	auto write(Stream& stream, Object const& object) -> detail::enable_if_parsable_t<Object, Result>;


	// Layout

	template<typename ... Objects>
	struct Layout
	{
		using Result = JL_BINARY::Result;

		template <typename Stream>
		static Result Read(Stream& is, Objects & ... objects)
		{
			return (failed(read(is, objects)) || ...);
		}

		template <typename Stream>
		static Result Write(Stream& os, Objects const& ... objects)
		{
			return (failed(write(os, objects)) || ...);
		}
	};

}