// BinaryReadWrite_Impl - read/write implementations

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

#include "JL_ReadWrite_Base.h"

namespace JL::rw
{

	namespace impl
	{
		using namespace utility;

		template <typename T>
		constexpr bool can_be_parsed_v = true
			|| is_pod_v          <T>
			|| is_container_v    <T>
			|| std::is_pointer_v <T>
		;
	}

	// Read  (declaration)
	template <typename Object, typename Stream>
	auto read(Stream& stream, Object& object) -> std::enable_if_t<impl::can_be_parsed_v<Object>, Result>;

	// Write (declaration)
	template <typename Object, typename Stream>
	auto write(Stream& stream, Object const& object)->std::enable_if_t<impl::can_be_parsed_v<Object>, Result>;

	namespace impl
	{

		// Pod (pointer)
		template <bool W, typename Pod, typename Stream>
		Result parse_pod_ptr(Stream& stream, Pod* const data)
		{
			if constexpr (W)
			{
				stream.write(bytes(data), sizeof(Pod));
			}
			else // R
			{
				stream.read(bytes(data), sizeof(Pod));
			}
			return failed(stream)
				? result_fail
				: result_success;
		}

		// Pod
		template <bool W, typename Pod, typename Stream>
		Result parse_pod(Stream& stream, Pod& data)
		{
			return parse_pod_ptr<W>(stream, addressof(data));
		}

		template <typename Pod, typename Stream>
		Result write_pod(Stream& stream, Pod const& data)
		{
			return parse_pod_ptr<WRITE>(stream, addressof(data));
		}

		// Dynamic array
		template <bool W, typename Pod, typename Stream>
		Result parse_array(Stream& stream, Pod* const data, ptrdiff_t const count)
		{
			if (count < 0)
				return result_fail;
			else
			if (count == 0)
				return result_success;
			else
			if constexpr (W)
			{
				if (failed(write_pod(stream, std::make_unsigned_t<ptrdiff_t>(count))))
					return result_fail;
				stream.write(bytes(data), count * sizeof(Pod));
			}
			else // R
			{
				stream.read(bytes(data), count * sizeof(Pod));
			}
			return failed(stream)
				? result_fail
				: result_success;
		}

		// Containers
		template <bool W, typename _Cont, typename Stream>
		Result parse_conatiner(Stream& stream, _Cont& cont)
		{
			using Cont = decltype(cont);
			using T = std::decay_t<decltype(*begin(cont))>;
			if constexpr (is_pod_v<T> && is_contiguous_container_v<Cont>)
			{
				if constexpr (W)
				{
					return parse_array<WRITE>(stream, data(cont), size(cont));
				}
				else // R
				{
					decltype(size(cont)) count{};
					if (failed(parse_pod<READ>(stream, count)))
						return result_fail;

					cont.resize(count);
					return parse_array<READ>(stream, data(cont), count);
				}
			}
			else
			{
				if constexpr (W)
				{
					write_pod(stream, size(cont));
					for (auto const& el : cont)
						if (failed(write(stream, el)))
							return result_fail;
					return result_success;
				}
				else // R
				{
					decltype(size(cont)) count{};
					if (failed(parse_pod<READ>(stream, count)))
						return result_fail;
					if (count == 0)
						return result_success;

					auto inserter = std::inserter(cont, end(cont));
					do
					{
						T object{};
						if (failed(read(stream, object)))
							return result_fail;
						inserter = std::move(object);
					} while (--count);

					return result_success;
				}
			}
		}

		// Any
		template <bool W, typename Object, typename Stream>
		Result parse_any(Stream& stream, Object& object)
		{

			if constexpr (is_pod_v<Object>)
			{
				return parse_pod<W>(stream, object);
			}
			else
			if constexpr (is_container_v<Object>)
			{
				return parse_conatiner<W>(stream, object);
			}
			else 
			if constexpr (std::is_pointer_v<Object>)
			{
				if constexpr (W)
					return write(stream, *object);
				else
					return read (stream, *object);
			}
			else return result_fail;

		}

	}

	// Read
	template <typename Object, typename Stream>
	auto read(Stream& stream, Object& object) -> std::enable_if_t<impl::can_be_parsed_v<Object>, Result>
	{
		return impl::parse_any<impl::READ>(stream, object);
	}

	// Write
	template <typename Object, typename Stream>
	auto write(Stream& stream, Object const& object) -> std::enable_if_t<impl::can_be_parsed_v<Object>, Result>
	{
		return impl::parse_any<impl::WRITE>(stream, object);
	}

}