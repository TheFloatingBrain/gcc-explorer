#include <CompilerLogExplorer/Common.hpp>
#include <CompilerLogExplorer/GCC/Terms.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	struct TemplateBindingParser
	{
		constexpr const static auto parser = ctpg::parser(
				bindingList, 
				ctpg::terms(
						cppIdentifier, 
						cppOperator, 
						beginSpecializationList, 
						equalTerm, 
						semiColonTerm, 
						endSquareBracketTerm, 
						cppNumber
					), 
				ctpg::nterms(
						cppAtom, 
						binding, 
						bindingValue, 
						bindingList, 
						parentBindingList
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
						bindingValue(cppAtom) >= [](auto atom) {
							return atom;
						}, 
						bindingValue(bindingValue, cppAtom) >= [](auto value, auto atom) {
							return value + std::string{atom};
						}, 
						bindingValue(bindingValue, endSquareBracketTerm) >= [](auto value, auto bracket) {
							return value + std::string{bracket};
						}, 
						bindingList(beginSpecializationList) >= [](auto) {
							//std::cout << "New list\n";
							return json{};
						}, 
						bindingList(bindingList, bindingValue, equalTerm, bindingValue, semiColonTerm) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list[parameter] = argument;
							return list;
						}, 
						bindingList(bindingList, bindingValue, equalTerm, bindingValue, endSquareBracketTerm) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							std::cout << "End0\n";
							list[parameter] = argument;
							return list;
						}, 
						parentBindingList(
								bindingList, 
								bindingValue, 
								equalTerm, 
								beginSpecializationList
							)
						>= [](auto list, auto parameter, auto, auto) {
							std::cout << "NESTED LIST!!\n";
							return ParentList{list, parameter};
						}, 
						parentBindingList(
								parentBindingList, 
								bindingValue, 
								equalTerm, 
								bindingValue, 
								semiColonTerm
							) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							list.data[parameter] = argument;
							return list;
						}, 
						bindingList(
								parentBindingList, 
								bindingValue, 
								equalTerm, 
								bindingValue, 
								endSquareBracketTerm, 
								semiColonTerm
							)
						>= [](auto parent, auto parameter, auto, auto argument, auto, auto)
						{
							ParentList list = parent;
							list.data[parameter] = argument;
							list.parent[list.parameterName] = list.data;
							return list.parent;
						}
					)
			);
	};

	template<auto ToPaseParameterConstant, typename ParserParameterType>
	static const auto parse()
	{
		const auto result = ParserParameterType::parser.parse(
				ctpg::buffers::cstring_buffer{ToPaseParameterConstant.string}, 
				std::cerr
			);
		return result;
	}
}

#endif // COMPILER__LOG__EXPLORER__GCC__PARSE__TEMPLATE__BINDING__HPP__INCLUDE__GUARD

