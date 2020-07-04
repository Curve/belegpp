
# Belegpp

[![Last Commit](https://img.shields.io/github/last-commit/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Cardsity/game-server/commits)
[![License](https://img.shields.io/github/license/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Cardsity/game-server/blob/master/LICENSE)

## What is this?
Belegpp is a header only c++17 library that aims to simplify working with stl containers (and strings).
This library aims to bring JS-Like functions for arrays and strings (or even other things) to c++ in form of "extensions".
## TODO
- [ ] splice
- [ ] slice
- [ ] split
- [ ] reverse
- [ ] some
- [ ] every
- [ ] sort (maybe)
- [ ] remove
- [ ] removeIf
- [ ] mutiply operator for strings ("n" * 5 -> "nnnnn")

## Extensions

### String Extensions
Found in `namespace beleg::extensions::strings`
* toLower
	* Example:
		```cpp
		std::string test("THIS IS UPPER CASE");
		auto upperCase = test | toLower(); //-> "this is upper case"
		//or
		char test[] = "THIS IS UPPER CASE";
		std::string lowerCase = test | toLower(); //-> "this is upper case" works on cstrs too!
		```
* toUpper
	* For an example see toLower

### STL Container Extensions
Found in `namespace beleg::extensions::containers`
* contains
	* Example
		```cpp
		std::vector<int> list = {1,2,3,4,5};
		list | contains(5) //-> true;
		
		std::string str = "Test string";
		str | contains("string") //-> true;
		```
* containsKey, containsItem
	* Example:
		```cpp
		std::map<std::string, int> test = { {"key", 5} };
		test | containsKey("key") //-> true;
		test | containsItem(5) //-> true;
		```
* map
	* Example
		```cpp
		std::vector<std::string> list = {"One", "Two"};
		list | map([](auto& item) { return "Number " + item; }) //-> {"Number One", "Number Two"}
		```
* filter
	* Example
		```cpp
		std::vector<int> list = {1, 2, 3, 4, 5, 6};
		list | filter([](auto& item) { return item % 2 == 0; }) //-> {2, 4, 6}
		```
* forEach
	* Basically just std::for_each as an extension
	* Example
		```cpp
		std::vector<int> list = {1,2,3};
		list | forEach([](auto& item) { std::cout << item << std::endl; }) //-> prints 1, 2, 3
		```
* find
	* Returns an optional iterator
	* Example
		```cpp
		std::vector<int> list = { 1, 2, 3 };
		auto two = list | find(2);
		if (two)
		{
			std::cout << **two << std::endl;
		}
		//-> Prints 2
		```
* findKey
	* see example for find, this is used on maps, also returns an iterator to the whole entry.
* findItem
	*	see findKey, same but for the value instead of the key.
