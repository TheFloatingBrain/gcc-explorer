#include <CompilerLogExplorer/Utility.hpp>
#include <CompilerLogExplorer/GCC/ParseTemplateBinding.hpp>
#include <CppUTest/TestHarness.h>

using namespace CompilerLogExplorer::GCC;
using namespace CompilerLogExplorer;


//template<auto LengthParameterCostant>
//constexpr static const auto operator ""}(const char (&string)[LengthParameterCostant]) {
//	return FixedString{string};
//}

const auto probeValueFromKey(auto data, std::vector<std::string> keys)
{
	const auto currentValue = data[*keys.begin()];
	if(keys.size() == 1)
		return currentValue;
	else
	{
		return probeValueFromKey(currentValue, std::vector<std::string>(
				keys.begin() + 1, keys.end()
			));
	}
}

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant, 
		auto ExpectedParameterConstant
	>
void check_parse(std::initializer_list<std::string> keys)
{
	const auto result = parse<ToPaseParameterConstant, ParserParameterType>();
	if(result.has_value() == false)
	{
		std::cerr
				<< "Failed to parse: " 
				<< ToPaseParameterConstant.string 
				<< "\n";
	}
	CHECK((result.has_value() == true));
	if(result.has_value() == true)
	{
		
		CHECK((probeValueFromKey(result.value(), toVector(keys))
				== std::string_view{ExpectedParameterConstant.string}
			));
	}
}

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant, 
		auto ExpectedParameterConstant
	>
void check_parse(std::string keys)
{
	check_parse<
			ParserParameterType, 
			ToPaseParameterConstant, 
			ExpectedParameterConstant
		>(std::initializer_list{keys});
}

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant
	>
void check_parse(std::initializer_list<std::string> keys, auto expected)
{
	const auto result = parse<ToPaseParameterConstant, ParserParameterType>();
	if(result.has_value() == false)
	{
		std::cerr
				<< "Failed to parse: " 
				<< ToPaseParameterConstant.string 
				<< "\n";
	}
	CHECK((result.has_value() == true));
	if(result.has_value() == true)
		CHECK((probeValueFromKey(result.value(), toVector(keys)) == expected));
}

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant
	>
void check_parse(std::string key, auto expected) {
	check_parse<ParserParameterType, ToPaseParameterConstant>(key, expected);
}

TEST_GROUP(ParseTemplateBinding) {};

TEST(ParseTemplateBinding, ParseBindings)
{
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc]"}, 
			FixedString{"abc"}
		>(std::string{"xyz"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = ww]"}, 
			FixedString{"abc"}
		>(std::string{"xyz"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = ww]"}, 
			FixedString{"ww"}
		>(std::string{"vv"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; test = 12]"}, 
			FixedString{"abc"}
		>(std::string{"xyz"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; test = 12]"}, 
			FixedString{"\"ww\""}
		>(std::string{"vv"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; test = 12]"}, 
			FixedString{"12"}
		>(std::string{"test"});
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc[1]; vv = \"ww\"; test = 12]"}, 
			FixedString{"abc[1]"}
		>(std::string{"xyz"});
};

TEST(ParseTemplateBinding, ParseNestedBindings)
{
	const auto parseSubList = parse<
			FixedString{"[with herp = flerp; kerp = 3]"}, 
			TemplateBindingParser
		>();
	std::cout << "Sub!\n";
	CHECK((parseSubList.has_value() == true));
	std::cout << "Blub!\n";
	std::cout << "SUBLIST: \n" << parseSubList.value() << "\n";
	std::cout << parse<
			FixedString{"[with xyz = abc; vv = \"ww\"; "
						"derp = [with herp = flerp; kerp = 3]; test = 12]"}, 
			TemplateBindingParser
		>().value() << "\n";
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; "
						"derp = [with herp = flerp; kerp = 3]; test = 12]"}
		>(std::initializer_list{std::string{"derp"}}, parseSubList.value());
	std::cout << "Club\n";
};

