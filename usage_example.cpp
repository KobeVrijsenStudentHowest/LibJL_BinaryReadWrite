
#include "JL_ReadWrite_Layout.h"
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
		if (MyLayout::Write(file, names, heights, measurements))
			return -1;

		// Or define one on the spot
		if (Layout<WeirdObject, WeirdObject, WeirdObject, std::string, int>::Write(file, weird1, weird2, weird3, "Some string, idk", 1009))
			return -1;
	}

	{
		// Same for reading.

		// Just copy the layout

		std::ifstream file{ "file.bin", std::ios::in | std::ios::binary };

		if (MyLayout::Read(file, names, heights, measurements))
			return -1;

		std::string str{};
		int i{};

		if (Layout<WeirdObject, WeirdObject, WeirdObject, std::string, int> ::Read(file, weird1, weird2, weird3, str, i))
			return -1;

	}


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
