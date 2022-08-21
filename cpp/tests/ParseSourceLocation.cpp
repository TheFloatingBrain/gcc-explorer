#include <CompilerLogExplorer/Utility.hpp>
#include <CompilerLogExplorer/GCC/ParseSourceLocation.hpp>
#include <CppUTest/TestHarness.h>

using namespace CompilerLogExplorer::GCC;
using namespace CompilerLogExplorer;

TEST_GROUP(ParseSourceLocation) {};

template<auto ToPaseParameterConstant>
void check_parse(SourceLocation expected, bool debug = false)
{
	const auto result = parse<ToPaseParameterConstant, SourceLocationParser>();
	CHECK((result.has_value() == true));
	CHECK((result.value() == expected));
}
template<auto ToPaseParameterConstant>
void check_parse(auto SourceLocation::* testMember, auto expectedValue, bool debug = false)
{
	const auto result = parse<ToPaseParameterConstant, SourceLocationParser>();
	CHECK((result.has_value() == true));
	CHECK((result.value().*testMember == expectedValue));
}

TEST(ParseSourceLocation, FilePath)
{
	check_parse<FixedString{"/home/Documents"}>(&SourceLocation::path, std::filesystem::path("/home/Documents"));
	check_parse<FixedString{"C:/Users/Alice/Documents"}>(&SourceLocation::path, std::filesystem::path("C:/Users/Alice/Documents"));
	check_parse<FixedString{"/home/Documents/Main.cpp"}>(&SourceLocation::path, std::filesystem::path("/home/Documents/Main.cpp"));
	check_parse<FixedString{"C:/Users/Alice/Documents/ToBob.asc"}>(&SourceLocation::path, std::filesystem::path("C:/Users/Alice/Documents/ToBob.asc"));
};

