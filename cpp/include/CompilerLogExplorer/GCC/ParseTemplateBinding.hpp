#include <CompilerLogExplorer/Common.hpp>
#include <CompilerLogExplorer/GCC/Terms.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	struct TemplateBindingParser
	{
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
						bindingData, 
						bindingArgument, 
						squareBracketScope, 
						bindingListScope
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
						bindingListScope(beginSpecializationList) >= [](auto) {
							return json{};
						}, 
						bindingListScope(bindingListScope, bindingValue, equalTerm, bindingValue, semiColonTerm)
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list[parameter] = argument;
							return list;
						}, 
						bindingListScope(bindingListScope, bindingValue, equalTerm, bindingArgument, semiColonTerm)
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list[parameter] = argument;
							return list;
						}, 
						bindingArgument(bindingListScope, bindingValue, equalTerm, bindingValue, rightSquareBracket)
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list[parameter] = argument;
							return list;
						}, 
						bindingArgument(bindingListScope, bindingValue, equalTerm, bindingArgument, rightSquareBracket)
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list[parameter] = argument;
							return list;
						}, 
						bindingData(bindingArgument) >= [](auto data) {
							return data;
						}
					)
			);
	};

}

#endif // COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

