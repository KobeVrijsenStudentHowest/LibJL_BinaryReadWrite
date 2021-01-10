// ReadWrite example      simple stream read/write implementation example

// This is freeand unencumbered software released into the public domain.
// 
// Anyone is free to copy, modify, publish, use, compile, sell, or
// distribute this software, either in source code form or as a compiled
// binary, for any purpose, commercial or non - commercial, and by any
// means.
// 
// In jurisdictions that recognize copyright laws, the author or authors
// of this software dedicate any and all copyright interest in the
// software to the public domain.We make this dedication for the benefit
// of the public at largeand to the detriment of our heirsand
// successors.We intend this dedication to be an overt act of
// relinquishment in perpetuity of all presentand future rights to this
// software under copyright law.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
// 
// For more information, please refer to < https://unlicense.org>


// Include layout
#include "JL_ReadWrite_Layout.h"

// use layout without namespace calls all the time
using JL::rw::Layout;

#include <set>

// An example of how to define a layout
using MyLayout = Layout<
	std::vector<std::string>,
	std::array<float, 12>,
	long
>;

using SetLayout = Layout<
	long,
	std::set<std::string>
>;

// An example of a class we like to use
class WeirdObject
{

public:


	/*other code*/


	// reading/writing

	// Object write implementation
	friend bool write(std::ofstream& os, WeirdObject const& object)
	{
		return WeirdObject::Layout::Write(os, object.m_Names, object.m_Heights, object.m_Measurements);
	}

	// Object read implementaion
	friend bool read(std::ifstream& is, WeirdObject& object)
	{
		return WeirdObject::Layout::Read(is, object.m_Names, object.m_Heights, object.m_Measurements);
	}

	// Make the function freind so it's global and can be called by  Layout::write()  internally

private:

	// example data memebers
	std::vector<std::string> m_Names;
	std::array<float, 12>    m_Heights;
	long					 m_Measurements;

	// Object read/write layout
	using Layout = Layout<
		decltype(m_Names),
		decltype(m_Heights),
		decltype(m_Measurements)
	>;

};




int main()
{

	// loose objects
	std::vector<std::string> names{ "ann", "joseph", "catherine" };
	std::array<float, 12> heights{ 2, 3, 5, 7, 11, 13, 17, 23, 29, 31, 37, 43 };
	long measurements{ 1234 };

	// some of our weird class objects
	WeirdObject weird1{}, weird2{}, weird3{};
	
	{
		// File stream
		std::ofstream file{ "file.bin", std::ios::out | std::ios::binary };

		// Write a previously defined layout
		auto result = MyLayout::Write(file, names, heights, measurements);
		if (failed(result))
			return -1;

		// Or define one on the spot
		auto result = Layout<WeirdObject, WeirdObject, WeirdObject, std::string, int>::Write(file, weird1, weird2, weird3, "Some string, idk", 1009);
		if (failed(result))
			return -1;
	}

	{
		// Same for reading.

		// Just copy the layout

		std::ifstream file{ "file.bin", std::ios::in | std::ios::binary };

		auto result = MyLayout::Read(file, names, heights, measurements);
		if (failed(result))
			return -1;

		std::string str{};
		int i{};

		auto result = Layout<WeirdObject, WeirdObject, WeirdObject, std::string, int> ::Read(file, weird1, weird2, weird3, str, i);
		if (failed(result))
			return -1;

	}


	// Example that uses use std::set with std::string

	{
		std::set<std::string> names{ "Ann", "Joseph", "Catherine" };

		std::ofstream file{ "set.bin", std::ios::out | std::ios::binary };
		SetLayout::Write(file, 0xDEADFACE, names);
	}

	{
		std::set<std::string> names{};
		long num{};

		std::ifstream file{ "set.bin", std::ios::in | std::ios::binary };
		SetLayout::Read(file, num, names);
	}

	return 0;

}
