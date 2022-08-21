#include <CompilerLogExplorer/Utility.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	using json = nlohmann::json;

	struct ParentList
	{
		json parent;
		std::string parameterName;
		json data;
	};
	
	constexpr static const auto bindingListBegin = FixedString{"[with"};
	constexpr static const auto bindingListEnd = FixedString{"]"};
	
	constexpr static const auto beginSpecializationListPattern = FixedString{"\\[with"};
	constexpr static const auto identifierPattern = FixedString{"[a-zA-Z_$][a-zA-Z_$0-9]+"};
	constexpr static const auto operatorPattern = FixedString{"[\\.<>,+-/*[,\\\\'\\\":~!#%^&()|]+"};
	constexpr static const auto numberPattern = FixedString{"[0-9]+"};
	
	constexpr static const ctpg::regex_term<identifierPattern.string> cppIdentifier("C++Identifier");
	constexpr static const ctpg::regex_term<operatorPattern.string> cppOperator("C++Operator");
	constexpr static const ctpg::regex_term<numberPattern.string> cppNumber("C++Number");
	constexpr static const ctpg::regex_term<beginSpecializationListPattern.string> beginSpecializationList("BeginSpecializationList");

	constexpr static const auto semiColonTerm = ctpg::char_term(';', 1, ctpg::associativity::ltor);
	constexpr static const auto equalTerm = ctpg::char_term('=');
	constexpr static const auto endSquareBracketTerm = ctpg::char_term(']');
	
	constexpr static const auto cppAtom = ctpg::nterm<json>("C++Atom");
	constexpr static const auto binding = ctpg::nterm<json>("Binding");
	constexpr static const auto bindingList = ctpg::nterm<json>("SpecializationBindingList");
	constexpr static const auto parentBindingList = ctpg::nterm<ParentList>("ParentSpecializationBindingList");
	constexpr static const auto bindingValue = ctpg::nterm<std::string>("BindingValue");
}

#endif // COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

