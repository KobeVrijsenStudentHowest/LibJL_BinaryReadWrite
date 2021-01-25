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