#include <CompilerLogExplorer/Common.hpp>
#include <CompilerLogExplorer/GCC/Terms.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	struct TemplateBindingParser
	{
		template<typename ParentParameterType = json>
		constexpr static const auto fromParent(auto parent, auto parameter, auto argument)
		{
			ParentList list = parent;
			std::cout << "LIST DATA: " << list.data << "\n";
			list.data[parameter] = argument;
			if(list.parameterName.has_value() == true)
			{
				auto& parentData = list.parent
						[ParentList::parentParameter]
						[ParentList::dataParameter];
			std::cout << "LIST PARENT: " << parentData << "\n";
				parentData[list.parameterName.value()] = list.data;
				return parentData;

			}
			return list.data;
		}
		constexpr const static auto parser = ctpg::parser(
				bindingData, 
				ctpg::terms(
						cppIdentifier, 
						cppOperator, 
						beginSpecializationList, 
						equalTerm, 
						semiColonTerm, 
						leftSquareBracket, 
						rightSquareBracket, 
						cppNumber, 
						cppCharacter, 
						cppString
					), 
				ctpg::nterms(
						cppAtom, 
						binding, 
						bindingValue, 
						bindingList, 
						bindingData, 
						bindingArgument, 
						listArgumentTerminator, 
						squareBracketScope
					), 
				ctpg::rules(
						cppAtom(cppIdentifier) >= [](auto identifier) {
							return std::string{identifier};
						}, 
						cppAtom(cppOperator) >= [](auto op) {
							return std::string{op};
						}, 
						cppAtom(cppNumber) >= [](auto number) {
							return std::string{number};
						}, 
						cppAtom(cppString) >= [](auto string) {
							return std::string{string};
						}, 
						cppAtom(cppCharacter) >= [](auto character) {
							return std::string{character};
						}, 
						squareBracketScope(leftSquareBracket) >= [](auto) {
							return std::string{"["};
						}, 
						squareBracketScope(squareBracketScope, cppAtom) >= [](auto scope, auto atom) {
							return scope + std::string{atom};
						}, 
						bindingValue(squareBracketScope, rightSquareBracket) >= [](auto scope, auto) {
							return scope + std::string{"]"};
						}, 
						bindingValue(bindingValue, squareBracketScope, rightSquareBracket) >= [](auto value, auto scope, auto) {
							return value + scope + std::string{"]"};
						}, 
						bindingValue(squareBracketScope) >= [](auto scope) {
							return scope;
						}, 
						bindingValue(cppAtom) >= [](auto atom) {
							return atom;
						}, 
						bindingValue(bindingValue, cppAtom) >= [](auto value, auto atom) {
							return value + std::string{atom};
						}, 
						bindingList(beginSpecializationList) >= [](auto) {
							return ParentList{};
						}, 
						bindingList(
								bindingList, 
								bindingValue, 
								equalTerm, 
								beginSpecializationList
							) 
						>= [](auto list, auto parameter, auto, auto) {
							return ParentList(list, parameter);
						}, 
						bindingList(
								bindingList, 
								bindingValue, 
								equalTerm, 
								bindingValue, 
								semiColonTerm
							) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list.data[parameter] = argument;
							return list;
						}, 
						bindingArgument(
								bindingList, 
								bindingValue, 
								equalTerm, 
								bindingValue, 
								rightSquareBracket
							) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							return fromParent(list, parameter, argument);
						}, 
						bindingData(
								bindingData, 
								bindingValue, 
								equalTerm, 
								bindingArgument, 
								rightSquareBracket, 
								semiColonTerm
							)
						>= [](auto list, auto parameter, auto, auto argument, auto, auto) {
							list[parameter] = argument;
							return list;
							//return fromParent(list, parameter, argument);
						}, 
						bindingData(bindingArgument) >= [](auto list) {
							return list;
						}
					)
			);
	};

	template<auto ToPaseParameterConstant, typename ParserParameterType>
	static const auto parse(bool verbose = false)
	{
		const auto result = ParserParameterType::parser.parse(
				verbose == true ? ctpg::parse_options{}.set_verbose() : ctpg::parse_options{}, 
				ctpg::buffers::cstring_buffer{ToPaseParameterConstant.string}, 
				std::cerr
			);
		return result;
	}
}

#endif // COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

