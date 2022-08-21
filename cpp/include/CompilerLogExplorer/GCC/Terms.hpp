#include <CompilerLogExplorer/Utility.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	using json = nlohmann::json;

	struct SourceLocation {
		std::filesystem::path path;
		std::optional<size_t> line, column;
	};

	// <Parse Template Binding/Specialization Lists in GCC Logs> //

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

	constexpr static const auto semiColonTerm = ctpg::char_term(';');
	constexpr static const auto equalTerm = ctpg::char_term('=');

	constexpr static const auto leftSquareBracket = ctpg::char_term('[');
	constexpr static const auto rightSquareBracket = ctpg::char_term(']');
	
	constexpr static const auto cppAtom = ctpg::nterm<json>("C++Atom");
	constexpr static const auto binding = ctpg::nterm<json>("Binding");
	constexpr static const auto bindingData = ctpg::nterm<json>("BindingData");
	constexpr static const auto bindingArgument = ctpg::nterm<json>("BindingArgument");
	constexpr static const auto squareBracketScope = ctpg::nterm<std::string>("SquareBracketScope");
	constexpr static const auto bindingListScope = ctpg::nterm<json>("bindingListScope");
	constexpr static const auto bindingValue = ctpg::nterm<std::string>("BindingValue");

	// <Parse Template Binding/Specialization Lists in GCC Logs> //
	
	// <Parse Source Locations (e.g /my/file.cpp:42:42), some support for windows paths (e.g C:/MyFile) in GCC Logs> //
	constexpr static const auto filePathPattern = FixedString{"([a-zA-Z]:)?([/\\\\][^/\\0]+)+:"};

	constexpr static const ctpg::regex_term<filePathPattern.string> gccFilePath("GccLogFilePath");

	constexpr static const auto colonTerm = ctpg::char_term(':');

	constexpr static const auto lineOrColumnNumber = ctpg::nterm<size_t>("LineOrColumnNumber");
	constexpr static const auto filePath = ctpg::nterm<std::filesystem::path>("FilePath");
	constexpr static const auto sourceLocation = ctpg::nterm<SourceLocation>("SourceLocation");
	// </Parse Source Locations (e.g /my/file.cpp:42:42) in GCC Logs> //

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

#endif // COMPILER__LOG__EXPLORER__GCC__TERMS__HPP__INCLUDE__GUARD

