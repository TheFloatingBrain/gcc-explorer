#include <CompilerLogExplorer/Utility.hpp>
#include <CompilerLogExplorer/GCC/ParseSourceLocation.hpp>
#include <CppUTest/TestHarness.h>

using namespace CompilerLogExplorer::GCC;
using namespace CompilerLogExplorer;

TEST_GROUP(ParseSourceLocation) {};

template<auto ToPaseParameterConstant>
void check_parse(SourceLocation expected, bool debug = false)
{
	const auto result = parse<ToPaseParameterConstant, SourceLocationParser>(debug);
	if(result.has_value() == false) {
		std::cerr << "Failed to parse with: \n\t"
				<< ToPaseParameterConstant.string << "\n";
	}
	CHECK((result.has_value() == true));
	if(result.value() != expected)
	{
		std::cerr << "Test failed with: \n"
				<< "\tActual: " << toString(result.value()) << "\n"
				<< "\tExpected: " << toString(expected) << "\n";
	}
	CHECK((result.value() == expected));
}

template<auto ToPaseParameterConstant>
void check_parse(auto SourceLocation::* testMember, auto expectedValue, bool debug = false)
{
	const auto result = parse<ToPaseParameterConstant, SourceLocationParser>();
	if(result.has_value() == false) {
		std::cerr << "Failed to parse with: \n\t"
				<< ToPaseParameterConstant.string << "\n";
	}
	CHECK((result.has_value() == true));
	const auto actual = result.value().*testMember;
	if(actual != expectedValue)
	{
		std::cerr << "Test failed with: \n"
				<< "\tActual: " << toString(actual) << "\n"
				<< "\tExpected: " << toString(expectedValue) << "\n";
	}
	CHECK((actual == expectedValue));
}

TEST(ParseSourceLocation, FilePath)
{
	check_parse<FixedString{"/home/Documents:"}>(&SourceLocation::path, std::filesystem::path("/home/Documents"));
	check_parse<FixedString{"C:/Users/Alice/Documents:"}>(&SourceLocation::path, std::filesystem::path("C:/Users/Alice/Documents"));
	check_parse<FixedString{"/home/Documents/Main.cpp:"}>(&SourceLocation::path, std::filesystem::path("/home/Documents/Main.cpp"));
	check_parse<FixedString{"C:/Users/Alice/Documents/ToBob.asc:"}>(&SourceLocation::path, std::filesystem::path("C:/Users/Alice/Documents/ToBob.asc"));
};

TEST(ParseSourceLocation, LineNumber)
{
	bool debug = false;
	check_parse<FixedString{"/home/user/test.cpp:42:"}>(SourceLocation{std::filesystem::path("/home/user/test.cpp"), 42}, debug);
};

