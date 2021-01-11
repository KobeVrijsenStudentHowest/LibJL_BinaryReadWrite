
#include <iostream>
#include <sstream>
#include <set>

#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include "JL_BinaryReadWrite.h"

TEST_CASE("BinaryReadWrite")
{
	using namespace JL::rw;

	// Test layout
	using TestLayout = Layout<char[4], int, float, std::string, std::set<char>>;

	// Test stream
	std::stringstream sstr{};

	// Write
	
	char  chars[4]     { "123" };
	int   i            { 0 | int(0x80'00'00'00) };
	float f            { 1234.5678f };
	std::string name   { "Marry" };
	std::set<char> cset{ 'h', 'p', 'c', 'd', 'z', 't' };

	TestLayout::Write(sstr, chars, i, f, name, cset);

	// Read

	char  n_chars[4];
	int   n_i;
	float n_f;
	std::string n_name;
	std::set<char> n_cset;

	TestLayout::Read(sstr, n_chars, n_i, n_f, n_name, n_cset);

	// Test
	REQUIRE(memcmp(chars, n_chars, 4) == 0);
	REQUIRE(i == n_i);
	REQUIRE(f == n_f);
	REQUIRE(name == n_name);
	REQUIRE(std::equal(begin(cset), end(cset), begin(n_cset)));

}
