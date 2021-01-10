// BinaryReadWrite_Base.h - Base declarations

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

#include <fstream>
#include <string>
#include <vector>
#include <array>

#include "JL_Utility.h"

namespace JL::rw
{

	using Result = bool;

	constexpr bool failed(Result const& r) noexcept
	{
		return r == true;
	}

	namespace impl
	{

		constexpr bool   READ           = false;
		constexpr bool   WRITE          = true;
		
		constexpr Result result_fail    = true;
		constexpr Result result_success = false;

		using rw::failed;

		template <typename ... T>
		bool failed(std::basic_ios<T...> const& stream)
		{
			constexpr auto state = std::ios_base::badbit | std::ios_base::failbit;
			return bool(stream.rdstate() & state);
		}

	}


}