
# Belegpp

[![Last Commit](https://img.shields.io/github/last-commit/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Git-Curve/belegpp/commits)
[![License](https://img.shields.io/github/license/Git-Curve/belegpp?style=for-the-badge)](https://github.com/Git-Curve/belegpp/blob/master/LICENSE)

## What is this?
Belegpp is a header only c++17 library that aims to simplify working with stl containers (and strings).
This library aims to bring JS-Like functions for arrays and strings (or even other things) to c++ in form of "extensions".

## Installing
### Manual
Drop the header file in to your project.
### Conan
Add the bintray remote:
```
conan remote add belegpp https://api.bintray.com/conan/curve/belegpp
```
and install belegpp:
```
conan install belegpp/0.1@curve/belegpp -r belegpp
```

Finally add belegpp to your conanfile.txt:
```
[requires]
belegpp/0.1@curve/belegpp
```
## Compatibility
If you're using [object_threadsafe](https://github.com/AlexeyAB/object_threadsafe) make sure to include `belegpp_sf.h` to enable container extensions on `safe_ptr`-container objects.

## Portability
Tested and compiled with:
-   Microsoft Visual Studio 2017
-   G++(GCC) 9.3.0
-   Clang(++) 10.0.0

## Exceptions
This library doesn't throw exceptions, I may add support for that later but for now I don't see any use case for exceptions in this library.
 
Code like this:
```cpp
std::vector<int> test = { 1 };
test | removeAt(5);
```
Will simply not do anything, it won't remove anything from the vector and will not throw an exception - same goes for every other extension provided by this library.
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
* operator!
	* Checks if a string is empty
	* Since overloading the "bool operator" from outside of the string class is not possible just use a double not to check if the string is **not** empty
	* Example
		```cpp
		std::string test;
		if (!test)
		{
			std::cout << "Empty!" << std::endl;
		}
		//-> Will print "Empty"
		test = "test";
		if (!!test)
		{
			std::cout << "Not empty!" << std::endl;
		}
		//-> Will print "Not empty!"
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
* startsWith
	* Example
		```cpp
		std::string test("sometest");
		test | startsWith("some") //-> true;
		```
* endsWith
	* Example
		```cpp
		std::string test("sometest");
		test | endsWith("test") //-> true;
		```
* equalsIgnoreCase
	* Example
		```cpp
		std::string test("TEST");
		test | equalsIgnoreCase("test") //-> true;
		```
* trim
	* Returns a copy
	* Example
		```cpp
		std::string test("  test       ");
		auto trimmed = test | trim(); //-> "test"
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
		/*or*/
		auto x = QuickPlaceholder<0>{};
		list | map("Number " + x) //-> {"Number One", "Number Two"}
		```
* mapTo
	* Returns a copy
	* Example
		```cpp
		std::vector<int> test = {1,2,3};
		test | mapTo<std::vector<std::string>>([](auto& item) { return std::to_string(item); }) //-> {"1","2","3"}
		```
* filter
	* Returns a copy
	* Example
		```cpp
		std::vector<int> list = {1, 2, 3, 4, 5, 6};
		list | filter([](auto& item) { return item % 2 == 0; }) //-> {2, 4, 6}
		/*or*/
		auto x = QuickPlaceholder<0>{};
		list | filter((x % 2) == 0) //-> {2, 4, 6}
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
		/*
		or:
		auto x = QuickPlaceholder<0>{};
		auto two = list | findIf(x == 2);
		*/
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
		/*or*/
		auto x = QuickPlaceholder<0>{};
		list | removeIf((x % 2) == 0);
		//-> List is now: { 1, 3, 5 }
		```
* removeAt
	* Works on the actual list
	* Example
		```cpp
		std::vector<int> list = { 1, 2, 3 };
		list | removeAt(1);
		//-> List is now: { 1, 3 }
		```
* sort
	* Returns a copy
	* Just like std::sort but as an extension
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3 };
		auto sorted = test | sort([](auto& first, auto& second) { return first > second; });
		/*or*/
		auto x = QuickPlaceholder<0>{};
		auto x2 = QuickPlaceholder<1>{};
		auto sorted = test | sort(x > x2);
		//-> sorted { 3, 2, 1 };
		```
* some
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		test | some([](auto item) {return item % 2 == 0; }) //-> true
		/*or*/
		auto x = QuickPlaceholder<0>{};
		test | some((x % 2) == 0) //-> true
		```
* every
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5 };
		test | every([](auto item) {return item % 2 == 0; }) //-> false
		/*or*/
		auto x = QuickPlaceholder<0>{};
		test | every((x % 2) == 0) //-> false
		```
* slice
	* Returns a copy
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3, 4, 5, 6 };
		auto sliced = test | slice(1, -1)
		//-> sliced { 2, 3, 4, 5 }
		```
* shuffle
	* Returns a copy
	* Uses std::mt19937 by default
	* Example
		```cpp
		std::vector<int> test = { 1, 2, 3 };
		auto shuffled = test | shuffle();
		//-> shuffled is now in a random order, for example { 3, 2, 1 }
		
		//Using another random
		auto customRandom = std::default_random_engine(beleg::helpers::containers::rd());
		auto cShuffled = test | shuffle(customRandom);
		```
* operator<<
	* Makes your container streamable
	* Will only work on containers that are initially not streamable
	* Example
		```cpp
		std::vector<std::string> test;
		test.push_back("Test");
		test.push_back("Test2");
		std::cout << test << std::endl; //-> { Test, Test2 }
	
		std::map<std::string, int> test2;
		test2.insert({ "Test", 1 });
		test2.insert({ "Test2", 2 }); 
		std::cout << test2 << std::endl; //-> { [Test, 1], [Test2, 2] }
		```
* join
	* Joins an array
	* Takes separator as an optional argument
	* Returns string
	* Example
		```cpp
		std::vector<int> test = { 1,2,3 };
		auto result = test | join(); //-> "123"
		```
	* Works with maps too!
		* Example
			```cpp
			std::map<std::string, int> test = { {"one", 1},{"two", 2} };
			auto resutl = test | join("..."); //-> "[one,1]...[two,2]"
			```
### print
Found in `namespace beleg::helpers::print`
* println
	* Prints everything thats passed to it (Most basic print, saves you from writing std::cout and std::endl)
	* While this is very basic it may be useful, I may add proper formatting support later.
	* Example
		```cpp
		println(10, " is a number!");
		```
* print
	* like println without newline
* printfln
	* Like printf
	* Takes a steam as optional paramter
	* Example
		```cpp
		printfln("%i", 10); //-> Will print 10
		
		std::stringstream someStream;
		printfln(someStream, "%i", 10);
		someStream.str() //-> "10\n"
		```
* printfs
	* Like printf but the result is returned as a string
	* Example
		```cpp
		std::string result = printfs("Test: %i", 1337); //-> "Test: 1337"
		```
* printfsln
	* Like printfs but appends a new line
## Beleg Lambdas
Found in `namespace beleg::lambdas`  
Beleg lambdas provide a simple way to write primitive lambdas.  
<b><i>Note:</b></i> Function calls aswell as some other "complex" stuff is not supported! 
### QuickPlaceholder
Is a placeholder for a function parameter.
#### How to use
```cpp
auto x = QuickPlaceholder<0/*the index of the function parameter*/>{};

// If we have a function like this:
void test(int firstParam, std::string secondParam);
// The placeholders would look like following =>
auto firstParam = QuickPlaceholder<0>{};
auto secondParam = QuickPlaceholder<1>{};
```
### Example
```cpp
auto x = QuickPlaceholder<0>{};
auto testFunc = (x = 10);

int testVal = 0;
testFunc(testVal);
//testVal is now 10

int testVal2 = 0;
auto isZeroFunc = (x == 0);
bool isZero = isZeroFunc(testVal2); 
//-> true

std::string testStr("Some");
auto multFunc = (x = (x + " Test"), x == "Some Test");
bool isSomeTest = multFunc(testStr);
//-> true
//-> testStr is now "Some Test"
```
For more examples look at `tests.cpp`


### Note
In all the examples only std::vector and std::map are used but the library is not limited to those.
As long as the container has the functions `begin`, `end` as well as an `const_iterator` the extensions ***should*** mostly work, but some extensions like the slice function requires the container to have an `assign` function.
Basically every extension except <u>slice</u> should work on every iterable object. 
