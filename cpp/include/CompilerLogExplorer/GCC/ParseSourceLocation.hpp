#include <CompilerLogExplorer/Common.hpp>
#include <CompilerLogExplorer/GCC/Terms.hpp>

#ifndef COMPILER__LOG__EXPLORER__GCC__PARSE__SOURCE__LOCATION__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__GCC__PARSE__SOURCE__LOCATION__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer::GCC
{
	struct SourceLocationParser
	{
		constexpr const static auto parser = ctpg::parser(
				sourceLocation, 
				ctpg::terms(
						cppNumber, 
						gccFilePath, 
						colonTerm
					), 
				ctpg::nterms(
						filePath, 
						sourceLocation
					), 
				ctpg::rules(
						filePath(gccFilePath, colonTerm) >= [](auto path, auto) {
							return std::filesystem::path{std::string_view{path}};
						}, 
						sourceLocation(filePath, cppNumber, colonTerm) >= [](auto path, auto lineNumber, auto)
						{
							size_t lineNumberUnsinged;
							const auto lineNumberStringView = std::string_view{lineNumber};
							std::from_chars(lineNumberStringView.begin(), lineNumberStringView.end(), lineNumberUnsinged);
							return SourceLocation{path, lineNumberUnsinged};
						}, 
						sourceLocation(filePath) >= [](auto path) {
							return SourceLocation{std::filesystem::path{path}};
						}
					)
			);
	};
}

#endif // COMPILER__LOG__EXPLORER__GCC__PARSE__SOURCE__LOCATION__HPP__INCLUDE__GUARD

