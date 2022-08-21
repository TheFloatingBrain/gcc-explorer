#include <CompilerLogExplorer/Utility.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	using json = nlohmann::json;

	enum class ParentListTag {
		Json, ParentList
	};

	struct ParentList;

	void to_json(json& data, const ParentList& list);
	void from_json(const json& data, ParentList& list);

	struct ParentList
	{
		constexpr static const char parentParameter[] = "parent";
		constexpr static const char parameterNameParameter[] ="parameterName";
		constexpr static const char dataParameter[] = "data";
		constexpr static const char tagParameter[] = "tag";

		json parent;
		std::optional<std::string> parameterName;
		json data;
		ParentListTag tag;
		ParentList() {}
		explicit ParentList(json parent, std::string parameterName) noexcept 
				: parent(json{{parentParameter, parent}}), 
				parameterName(parameterName), 
				data({}), 
				tag(ParentListTag::Json) {
			std::cout << "Json Parent List\n";
		}
		explicit ParentList(ParentList parent, std::string parameterName) noexcept 
				: parent(json{{parentParameter, parent}}), 
				parameterName(parameterName), 
				data({}), 
				tag(ParentListTag::ParentList) {
			std::cout << "Parent List Parent List\n";
		}
	};

	void to_json(json& data, const ParentList& list)
	{
		json newData = {};
		if(list.parent.contains(ParentList::parentParameter) == true)
			newData[ParentList::parentParameter] = list.parent[ParentList::parentParameter];
		if(list.parameterName.has_value() == true)
			newData[ParentList::parameterNameParameter] = list.parameterName.value();
		newData[ParentList::dataParameter] = list.data;
		newData[ParentList::tagParameter] = list.tag;
		data = newData;
	}

	void from_json(const json& data, ParentList& list)
	{
		if(data.contains(ParentList::parentParameter) == true)
			list.parent = data[ParentList::parentParameter];
		if(data.contains(ParentList::parameterNameParameter) == true)
			list.parameterName = std::string{data[ParentList::parameterNameParameter]};
		if(data.contains(ParentList::dataParameter) == true)
			list.data = data[ParentList::dataParameter];
		if(data.contains(ParentList::tagParameter) == true)
			list.tag = data[ParentList::tagParameter];
	}
	
	constexpr static const auto bindingListBegin = FixedString{"[with"};
	constexpr static const auto bindingListEnd = FixedString{"]"};
	
	constexpr static const auto beginSpecializationListPattern = FixedString{"\\[with"};
	constexpr static const auto identifierPattern = FixedString{"[a-zA-Z_$][a-zA-Z_$0-9]+"};
	constexpr static const auto operatorPattern = FixedString{"[\\.<>,+-/*,\\\\:~!#%^&()|]+"};
	constexpr static const auto stringPattern = FixedString{"\"([^\"]|\\\")+\""};
	constexpr static const auto characterPattern = FixedString{"\'([^\"\']|\\\"|\\\')\'"};

	constexpr static const auto numberPattern = FixedString{"[0-9]+"};
	
	constexpr static const ctpg::regex_term<identifierPattern.string> cppIdentifier("C++Identifier");
	constexpr static const ctpg::regex_term<operatorPattern.string> cppOperator("C++Operator");
	constexpr static const ctpg::regex_term<numberPattern.string> cppNumber("C++Number");
	constexpr static const ctpg::regex_term<characterPattern.string> cppCharacter("C++Character");
	constexpr static const ctpg::regex_term<stringPattern.string> cppString("C++String");
	constexpr static const ctpg::regex_term<beginSpecializationListPattern.string> beginSpecializationList("BeginSpecializationList", 10);

	constexpr static const auto semiColonTerm = ctpg::char_term(';');//, 1, ctpg::associativity::ltor);
	constexpr static const auto equalTerm = ctpg::char_term('=');

	constexpr static const auto leftSquareBracket = ctpg::char_term('[');
	constexpr static const auto rightSquareBracket = ctpg::char_term(']');
	//constexpr static const auto rightCurleyBracket = ctpg::char_term('}');
	//constexpr static const auto rightAngleBracket = ctpg::char_term('>');
	//constexpr static const auto rightParenthesisBracket = ctpg::char_term(')');
	
	constexpr static const auto cppAtom = ctpg::nterm<json>("C++Atom");
	constexpr static const auto binding = ctpg::nterm<json>("Binding");
	constexpr static const auto bindingList = ctpg::nterm<ParentList>("SpecializationBindingList");
	constexpr static const auto bindingData = ctpg::nterm<json>("BindingData");
	constexpr static const auto bindingArgument = ctpg::nterm<json>("BindingArgument");
	constexpr static const auto squareBracketScope = ctpg::nterm<std::string>("SquareBracketScope");
	//constexpr static const auto bindingList = ctpg::nterm<ParentList>("SpecializationBindingList");
	//constexpr static const auto parentBindingList = ctpg::nterm<ParentList>("ParentSpecializationBindingList");
	constexpr static const auto bindingValue = ctpg::nterm<std::string>("BindingValue");
	struct ListArgumentTerminator {};
	constexpr static const auto listArgumentTerminator = ctpg::nterm<ListArgumentTerminator>("ListArgumentTerminator");

}

#endif // COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

