#include <iostream>
#include <vector>
#include <string>
#include <string_view>
#include <algorithm>
#include <ranges>
#include <cctype>
#include <ctre.hpp>
#include <nlohmann/json.hpp>

#include <test_strings.hpp>

using json = nlohmann::json;

constexpr static const auto specialisation_pattern = ctll::fixed_string{
		"\\[with (.+?)]"
	};
constexpr static const auto specialisation_binding_pattern = ctll::fixed_string{
		"^(.+)\\s*=\\s*(.+)$"
	};

std::string erase_white_space(auto remove_from)
{
	remove_from.erase(
			std::remove_if(
					remove_from.begin(), 
					remove_from.end(), 
					::isspace
				), 
		remove_from.end()
	);
	return remove_from;
}

std::string iterators_to_string(auto from) {
	return std::string{from.begin(), from.end()};
}

template<
		auto SpecializationRegexParameterConstant = specialisation_pattern
	>
std::vector<std::string> extract_specialization_lists(std::string_view log_contents)
{
	std::vector<std::string> specialization_lists;
	//std::string tail = "";
    // find '... [with x=y; z=...] lists and create tables
	for(auto match : ctre::range<SpecializationRegexParameterConstant>(log_contents)) {
		auto [whole, list] = match;
		specialization_lists.push_back(std::string{std::string_view{list}});
	}
	return specialization_lists;
}

template< 
		auto DeliminatorParameterConstant = ';', 
		auto SpecializationBindingRegex = specialisation_binding_pattern
	>
auto extract_specialization_substitutions(std::string_view specialization_list)
{
	std::vector<json> specializations;
	for(const auto specialization 
			: std::views::split(
					specialization_list, 
					DeliminatorParameterConstant
				) 
		)
	{
		json new_specialiation_list;
		std::string slice{specialization.begin(), specialization.end()};
		auto current_specialzations = ctre::range<
				SpecializationBindingRegex
			>(std::string_view(slice)); 
		for(const auto specialization : current_specialzations)
		{
			auto [whole, parameter, argument] = specialization;
			new_specialiation_list[erase_white_space(
					iterators_to_string(parameter)
				)] = erase_white_space(iterators_to_string(argument));
		}
		specializations.push_back(new_specialiation_list);
	}
	return specializations;
}

int main(int argc, char** args)
{
	auto specialization_lists = extract_specialization_lists(test_log);
	for(const auto& specialization_list : specialization_lists)
	{
		std::cout << "List: " << specialization_list << "\n";
		auto substitutions = extract_specialization_substitutions(specialization_list);
		for(const auto& subs : substitutions)
			std::cout << subs << "\n";
	}
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

