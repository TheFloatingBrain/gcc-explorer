#include <iostream>
#include <string>
#include <algorithm>
#include <string_view>
#include <ctre.hpp>

#include <test_strings.hpp>

constexpr static const auto specialisation_pattern = ctll::fixed_string{"\\[with (.+?)]"};

template<
		auto SpecializationRegexParameterConstant = specialisation_pattern
	>
std::string tokenise(std::string_view log_contents)
{
	std::string log{log_contents};
	//std::string tail = "";
    // find '... [with x=y; z=...] lists and create tables
	for(auto match : ctre::range<SpecializationRegexParameterConstant>(log_contents))
		std::cout << match << "\n";
	return log;
}

int main(int argc, char** args)
{
	tokenise(std::string_view{test_log});
	std::cout << "Done\n";
	return 0;
}

/*
    x = x.replace( function (_, list) {
        let result = "";
        $.each(list.split('; '), function (_, row) {
            result += row.replace(/^(.+) = (.+)$/, function (_, label, value) {
                return specialisation_row_start +
                       specialisation_cell_start + tokenise(label) + specialisation_cell_end +
                       specialisation_cell_start + tokenise(value) + specialisation_cell_end + 
                       specialisation_row_end;
            });
        });
        tail = specialisation_table_start + result + specialisation_table_end;
        return "";
    });
    // a comma creates a new list element
    x = x.replaceAll(',', ',' + list_element_end + list_element_start);
    x = x.replaceAll(';', ';' + list_element_end + list_element_start);
    // an open brace creates a sublist - designate the character
    x = x.replaceAll('{', clean_html('{') + sublist_start_braces + list_element_start)
    x = x.replaceAll('(', clean_html('(') + sublist_start_parens + list_element_start)
    x = x.replaceAll('[', clean_html('[') + sublist_start_square + list_element_start)
    x = x.replaceAll('<', clean_html('<') + sublist_start_angle + list_element_start)
    // a close brace closes a sublist
    x = x.replaceAll(/[\])}>]/g, x => list_element_end + sublist_end + clean_html(x));
    // replace injected tokens with HTML tags
    // note: sublists can be collapsed, so we toggle them with an fold_toggle

    return code_start + x + code_end + tail;
}*/

