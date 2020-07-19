#include "belegpp.h"
#include "belegpp_sf.h"
#include <cassert>
#include <map>
#include <iostream>

using namespace beleg::extensions::strings;
using namespace beleg::extensions::containers;

int main()
{
	{
		std::string test("TEST");
		assert((test | toLower()) == "test");
		assert(("TEST" | toLower()) == "test");
	}
	{
		std::string test("test");
		assert((test | toUpper()) == "TEST");
		assert(("test" | toUpper()) == "TEST");
	}
	{
		std::string test("n");
		assert((test * 3) == "nnn");
		assert(("n" | mul(3)) == "nnn");
	}
	{
		std::string test("");
		assert(!test == true);
		assert(!!test == false);
	}
	{
		std::string test("test");
		assert((test | replace("t", "s")) == "sess");
		assert(("test" | replace("t", "s")) == "sess");
	}
	{
		std::string test("test,test");
		assert((test | split(",")).size() == 2);
		assert(("test,test" | split(",")).size() == 2);
	}
	{
		std::string test("sometest");
		assert((test | startsWith("some")) == true);
		assert(("sometest" | startsWith("some")) == true);
	}
	{
		std::string test("sometest");
		assert((test | endsWith("test")) == true);
		assert(("sometest" | endsWith("test")) == true);
	}
	{
		std::string test("TEST");
		assert((test | equalsIgnoreCase("tEst")) == true);
		assert(("TEST" | equalsIgnoreCase("tEst")) == true);
	}
	{
		std::string test("    test  ");
		assert((test | trim()) == "test");
		assert(("    test  " | trim()) == "test");
	}
	{
		std::string test("test");
		assert((test | contains("es")) == true);
		assert(("test" | contains("es")) == true);
	}
	{
		std::vector<int> test = { 1, 2, 3 };
		assert((test | contains(2)) == true);
	}
	{
		std::map<int, int> test = { {1,2} };
		assert((test | containsKey(1)) == true);
		assert((test | containsItem(2)) == true);
	}
	{
		std::vector<std::string> test = { "One", "Two" };
		auto mapped = test | map([](auto& item) { return "Number " + item; });
		assert(mapped.at(0) == "Number One");
	}
	{
		std::vector<int> test = { 1, 2, 3, 4, 5, 6 };
		auto filtered = test | filter([](auto& item) { return item % 2 == 0; });
		assert((filtered.at(1) == 4));
	}
	{
		std::vector<int> test = { 1 };
		test | forEach([](auto& item) { assert(item == 1); });
	}
	{
		std::vector<int> test = { 1,2,3 };
		auto it = test | find(2);
		assert(it);
		assert(**it == 2);
	}
	{
		std::vector<int> test = { 1,2,3 };
		auto it = test | findIf([](auto& item) { return item % 2 == 0; });
		assert(it);
		assert(**it == 2);
	}
	{
		std::vector<int> test = { 1,2,3 };
		auto reversed = test | reverse();
		assert(reversed.at(0) == 3);
	}
	{
		std::vector<int> test = { 1, 2, 3 };
		test | beleg::extensions::containers::remove(2);
		assert(test.at(1) == 3);
	}
	{
		std::vector<int> test = { 1,2,3,4 };
		test | removeIf([](auto& item) { return item % 2 == 0; });
		assert(test.at(1) == 3);
	}
	{
		std::vector<int> test = { 1, 2, 3 };
		test | removeAt(1);
		assert(test.at(1) == 3);
	}
	{
		std::vector<int> test = { 1, 2, 3 };
		auto sorted = test | sort([](auto& first, auto& second) { return first > second; });
		assert(sorted.at(0) == 3);
	}
	{
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		assert((test | some([](auto item) {return item % 2 == 0; })) == true);
	}
	{
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		assert((test | every([](auto item) {return item % 2 == 0; })) == false);
	}
	{
		std::vector<int> test = { 1, 2, 3, 4, 5, 6 };
		auto sliced = test | slice(1, -1);
		assert(sliced.at(0) == 2);
		assert(sliced.at(3) == 5);
	}
	{
		//Well, this may fail sometimes due to randomness but is very unlikely
		std::vector<int> test = { 1, 2, 3 };
		auto shuffled = test | shuffle();
		bool success = false;
		for (int i = 0; 10 > i; i++)
		{
			if (shuffled != test)
				success = true;
		}
		assert(success == true);
	}
	{
		std::vector<int> test = { 1, 2, 3, 4, 5, 6 };
		std::stringstream sstr;
		sstr << test;
		assert(sstr.str().size() > 0);
	}
	{
		std::vector<int> test = { 1,2,3 };
		auto res = test | mapTo<std::vector<std::string>>([](auto& item) { return std::to_string(item); });
		assert(res.at(0) == "1");
	}

	std::cout << "Tests finished!" << std::endl;
	return 0;
}