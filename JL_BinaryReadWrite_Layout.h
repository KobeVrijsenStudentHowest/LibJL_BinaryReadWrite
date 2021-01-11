// BinaryReadWrite_Layout.h - Layout structure

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

#include "JL_BinaryReadWrite_Impl.h"

namespace JL::rw
{

	template<typename ... Objects>
	struct Layout
	{
		using Result = rw::Result;

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