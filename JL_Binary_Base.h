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

#include "../Utility/JL_Utility.h"

#define JL_BINARY JL::binary

namespace JL_BINARY
{

	namespace detail
	{

		using namespace JL_UTILITY;

		constexpr bool   READ           = false;
		constexpr bool   WRITE          = true;

		template <typename ... T>
		bool failed(std::basic_ios<T...> const& stream)
		{
			constexpr auto state = std::ios_base::badbit | std::ios_base::failbit;
			return bool(stream.rdstate() & state);
		}

		enum class eKind
		{
			invalid = 0,
			pod,
			container,
			pointer
		};

		template <typename Object>
		constexpr auto parse_kind() noexcept
		{
			if constexpr (is_pod_v<Object>)
			{
				return eKind::pod;
			}
			else if constexpr (is_container_v<Object>)
			{
				return eKind::container;
			}
			else if constexpr (std::is_pointer_v<Object>)
			{
				return eKind::pointer;
			}
			else return eKind::invalid;
		}

		template <typename Object, typename Return>
		using enable_if_parsable_t = std::enable_if_t<parse_kind<Object>() != eKind::invalid, Return>;

	}


}