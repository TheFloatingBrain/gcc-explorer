#include <CompilerLogExplorer/Utility.hpp>
#include <CompilerLogExplorer/GCC/ParseTemplateBinding.hpp>
#include <CppUTest/TestHarness.h>

using namespace CompilerLogExplorer::GCC;
using namespace CompilerLogExplorer;

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

static bool debug = false;

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant
	>
void check_parse(std::initializer_list<std::string> keys, auto expected)
{
	if(debug == true)
		std::cout << "Running: " << ToPaseParameterConstant.string << "\n";
	const auto result = parse<ToPaseParameterConstant, ParserParameterType>(debug);
	if(result.has_value() == false)
	{
		std::cerr
				<< "Failed to parse: " 
				<< ToPaseParameterConstant.string 
				<< "\n";
	}
	else if(debug == true) {
		std::cout << "Result: \n"
				<< "\tFull Result: " << result.value() << "\n";
	}
	CHECK((result.has_value() == true));
	if(result.has_value() == true)
	{
		const auto testValue = probeValueFromKey(result.value(), toVector(keys));
		const bool correct = (testValue == expected);
		if(correct == false)
		{
			std::cout << "Failed to parse: " 
					<< ToPaseParameterConstant.string 
					<< "\n{with: \n" 
					<< "\tActual: " << testValue << "\n"
					<< "\tExpected: " << expected << "\n"
					<< "}\n";
		}
		CHECK((correct == true));
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
			ToPaseParameterConstant
		>(std::initializer_list{keys}, std::string_view{ExpectedParameterConstant});
}

template<
		typename ParserParameterType, 
		auto ToPaseParameterConstant, 
		typename ExpectedParameterType
	>
void check_parse(std::string keys, std::optional<ExpectedParameterType> expected)
{
	check_parse<
			ParserParameterType, 
			ToPaseParameterConstant
		>(std::initializer_list{keys}, expected.value());
}

TEST_GROUP(ParseTemplateBinding) {};

TEST(ParseTemplateBinding, ParseBindings)
{
	//debug = true;
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
	const auto nestedList = parse<
			FixedString{"[with herp = flerp; kerp = 3]"}, 
			TemplateBindingParser
		>();
	CHECK((nestedList.has_value() == true));
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; "
						"derp = [with herp = flerp; kerp = 3]; "
						"test = 12]"
					}
		>("derp", nestedList);
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; "
						"test = 12; "
						"derp = [with herp = flerp; kerp = 3]]"
					}
		>("derp", nestedList);
	const auto nestedNestedListTwoElement = parse<
			FixedString{"[with derp = [with herp = flerp; kerp = 3]; hundred = 100]"}, 
			TemplateBindingParser
		>();
	CHECK((nestedNestedListTwoElement.has_value() == true));
	check_parse<
			TemplateBindingParser, 
			FixedString{"[with xyz = abc; vv = \"ww\"; "
						"test = 12; "
						"nonsense = [with derp = [with herp = flerp; kerp = 3]; hundred = 100];"
						"beep = boop]"
					}
		>("nonsense", nestedNestedListTwoElement);
};

