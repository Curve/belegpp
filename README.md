



# Belegpp

[![Last Commit](https://img.shields.io/github/last-commit/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Git-Curve/belegpp/commits)
[![License](https://img.shields.io/github/license/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Git-Curve/belegpp/blob/master/LICENSE)

## What is this?
Belegpp is a header only c++17 library that aims to simplify working with stl containers (and strings).
This library aims to bring JS-Like functions for arrays and strings (or even other things) to c++ in form of "extensions".
## TODO
- [x] ~~slice~~
- [x] ~~split~~
- [x] ~~replace~~
- [x] ~~reverse~~
- [x] ~~some~~
- [x] ~~every~~
- [x] ~~sort (maybe)~~
- [x] ~~remove~~
- [x] ~~removeIf~~
- [x] ~~mutiply operator for strings ("n" * 5 -> "nnnnn")~~

## Extensions

### String Extensions
Found in `namespace beleg::extensions::strings`
* toLower
	* Returns a copy
	* Example:
		```cpp
		std::string test("THIS IS UPPER CASE");
		auto upperCase = test | toLower(); //-> "this is upper case"
		//or
		char test[] = "THIS IS UPPER CASE";
		std::string lowerCase = test | toLower(); //-> "this is upper case" works on cstrs too!
		```
* toUpper
	* Returns a copy
	* For an example see toLower
* operator*
	* Repeats string n times
	* Example
		```cpp
		 std::string test("a");
		 std::cout << test * 3 << std::endl; //-> Prints "aaa"
		//Doesn't work on cstrs that good though
		 std::string test2("a" | mul(3)); //-> test2 = "aaa"
		```
* replace
	* Returns a copy
	* Example
		 ```cpp
		 std::string test("Replace this with something");
		 auto replaced = test | replace("this", "something"); //-> "Replace something with something"
		 ```
* split
	* Example
		```cpp
		std::string test("Give,me,this,as,an,array");
		auto list = test | split(",");
		//->list {"Give","me","this","as","an","array"}
		```

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
	* Returns a copy
	* Example
		```cpp
		std::vector<std::string> list = {"One", "Two"};
		list | map([](auto& item) { return "Number " + item; }) //-> {"Number One", "Number Two"}
		```
* filter
	* Returns a copy
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
* findIf
	* Returns an optional iterator
	* Basically just std::find_if as an extension
	* Example
		```cpp
		std::vector<int> list = { 1, 2, 3 };
		auto two = list | findIf([](auto& item) { return item == 2; });
		if (two)
		{
			std::cout << **two << std::endl;
		}
		//-> Prints 2
		```
* reverse
	* Returns a copy
	* just like std::reverse but as an extension
	* Example
		```cpp
		std::vector<int> list = { 1, 2, 3, 4, 5, 6 };
		list | reverse() | forEach([](auto item) { std::cout << item << std::endl; });
		//-> Prints 6, 5, 4, 3, 2, 1
		```
* remove
	* Works on the actual list
	* just like std::remove but as an extension
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3 };
		test | remove(2);
		//-> test is now { 1, 3 };
		```
* removeIf
	* Works on the actual list
	* just like std::remove_if but as an extension
	* Example
		```cpp
		std::vector<int> list = { 1, 2, 3, 4, 5, 6 };
		list | removeIf([](auto& item) { return item % 2 == 0; });
		//-> List is now: 1, 3, 5
		```
* sort
	* Returns a copy
	* Just like std::sort but as an extension
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3 };
		auto sorted = test | sort([](auto& first, auto& second) { return first > second; });
		//-> sorted { 3, 2, 1 };
		```
* some
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		test | some([](auto item) {return item % 2 == 0; }) //-> true
		```
* every
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		test | every([](auto item) {return item % 2 == 0; }) //-> false
		```
* slice
	* Returns a copy
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5, 6 };
		auto sliced = test | slice(1, -1)
		//-> sliced {2, 3, 4, 5}
		```

### Note
In all the examples only std::vector and std::map are used but the library is not limited to those.
As long as the container has the functions `begin`, `end` as well as an `const_iterator` the extensions ***should*** mostly work, but some extensions like the slice function requires the container to have an `assign` function.
Basically every extension except <u>slice</u> should work on every iterable object.