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
						endSquareBracketTerm, 
						cppNumber
					), 
				ctpg::nterms(
						cppAtom, 
						binding, 
						bindingValue, 
						bindingList, 
						bindingData
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
						bindingValue(bindingValue, endSquareBracketTerm) 
						>= [](auto value, auto bracket) {
							return value + std::string{bracket};
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
						bindingData(
								bindingList, 
								bindingValue, 
								equalTerm, 
								bindingValue, 
								endSquareBracketTerm
							) 
						>= [](auto list, auto parameter, auto, auto argument, auto) {
							return fromParent(list, parameter, argument);
						}
				)
				//Good?
						//bindingValue(bindingList, bindingValue, equalTerm, bindingValue, endSquareBracketTerm, semiColonTerm) 
						//>= [](auto list, auto parameter, auto, auto argument, auto, auto) {
						//	std::cout << "l.p: " << list << "\n"
						//			//<< "l.d: " << list.data << "\n"
						//			//<< "l.pn: " << list.parameterName << "\n"
						//			<< "p: " << parameter << "\n"
						//			<< "a: " << argument << "\n";
						//	list[parameter] = argument;
						//	return list;
						//}//, 
	//			////////////////////
	//					parentBindingList(
	//							bindingList, 
	//							bindingValue, 
	//							equalTerm, 
	//							beginSpecializationList
	//						)
	//					>= [](auto list, auto parameter, auto, auto) {
	//							return ParentList(list, parameter);
	//					}, 
	//					parentBindingList(
	//							parentBindingList, 
	//							bindingValue, 
	//							equalTerm, 
	//							beginSpecializationList
	//						)
	//					>= [](auto list, auto parameter, auto, auto) {
	//						std::cout << "Creating nested nested list\n";
	//						return ParentList(list, parameter);
	//					}, 
	//					bindingList(
	//							parentBindingList, 
	//							bindingValue, 
	//							equalTerm, 
	//							bindingValue, 
	//							endSquareBracketTerm, 
	//							semiColonTerm
	//						)
	//					>= [](auto parent, auto parameter, auto, auto argument, auto, auto) {
	//						return fromParent(parent, parameter, argument);
	//					}, 
	//			//
	//					parentBindingList(
	//							parentBindingList, 
	//							bindingValue, 
	//							equalTerm, 
	//							bindingValue, 
	//							semiColonTerm
	//						) 
	//					>= [](auto list, auto parameter, auto, auto argument, auto) {
	//						std::cout << "l.p: " << list.parent << "\n"
	//								<< "l.d: " << list.data << "\n"
	//								<< "l.pn: " << list.parameterName << "\n"
	//								<< "p: " << parameter << "\n"
	//								<< "a: " << argument << "\n";
	//						return fromParent(list, parameter, argument);
	//						//list.data[parameter] = argument;
	//						//return list;
	//					}//, 
				//
						//bindingList(
						//		parentBindingList, 
						//		bindingValue, 
						//		equalTerm, 
						//		bindingValue, 
						//		endSquareBracketTerm, 
						//		endSquareBracketTerm
						//	)
						//>= [](auto parent, auto parameter, auto, auto argument, auto, auto) {
						//	std::cout << "end0\n";
						//	return fromParent(parent, parameter, argument);
						//}//, 
				//
						//parentBindingList(
						//		parentBindingList, 
						//		bindingValue, 
						//		equalTerm, 
						//		bindingValue, 
						//		endSquareBracketTerm, 
						//		endSquareBracketTerm
						//	)
						//>= [](auto parent, auto parameter, auto, auto argument, auto, auto) {
						//	std::cout << "end1\n";
						//	return fromParent(parent, parameter, argument);
						//}, 
						//bindingList(parentBindingList) >= [](auto list) {
						//	std::cout << "The end\n";
						//	list.parent[list.parameterName] = list.data;
						//	return list.parent;
						//}
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

