#include <CompilerLogExplorer/Common.hpp>

#ifndef COMPILER__LOG__EXPLORER__UTILITY__HPP__INCLUDE__GUARD
#define COMPILER__LOG__EXPLORER__UTILITY__HPP__INCLUDE__GUARD

namespace CompilerLogExplorer
{
	template<size_t LengthParameterConstant>
	constexpr const auto& copy_array(const auto (&from)[LengthParameterConstant], auto (&to)[LengthParameterConstant])
	{
		for(size_t ii = 0; ii < LengthParameterConstant; ++ii)
			to[ii] = from[ii];
		return to;
	}
	
	template<size_t LengthParameterConstant>
	struct FixedString
	{
		char string[LengthParameterConstant];
		using StringType = decltype(string);
		constexpr static const size_t size = LengthParameterConstant;
		constexpr FixedString(const auto (&string_)[LengthParameterConstant]) noexcept {
			copy_array(string_, string);
		}
		constexpr operator std::string_view() const {
			return std::string_view{string};
		}
		constexpr operator const char*() const {
			return string;
		}
		constexpr operator ctpg::buffers::cstring_buffer<LengthParameterConstant>() const {
			return ctpg::buffers::cstring_buffer{string};
		}
	};

	template<typename ParameterType>
	std::string optionalToString(std::optional<ParameterType> optional) {
		return optional.has_value() == true ? std::string{optional.value()} : std::string{"<nullopt>"};
	}
	template<std::integral ParameterType>
	std::string optionalToString(std::optional<ParameterType> optional) {
		return optional.has_value() == true ? std::to_string(optional.value()) : std::string{"<nullopt>"};
	}

	//template<size_t CapacityParameterConstant = 1024>
	//struct ConstantLog
	//{
	//	constinit static const size_t capacity = CapacityParameterConstant;
	//	const lefticus::tools::simple_stack_vector<std::string, capacity> log;

	//	constexpr ConstantLog() noexcept : log({}) {}
	//	constexpr ConstantLog(const ConstantLog& other, std::string next) noexcept : log(other.log.push_back(next)) {}

	//	constexpr ConstantLog operator<<(std::string_view data) const {
	//		return ConstantLog<capacity>{*this, std::string{data}};
	//	}

	//	constexpr auto operator<<(const std::integral auto& data) const {
	//		return ConstantLog<capacity>{*this, std::to_string(data)};
	//	}

	//	constexpr auto operator<<(const auto& data) {
	//		return ConstantLog<capacity>{*this, data)};
	//	}

	//	constexpr auto operator<<(const std::initializer_list<char> data) const {
	//		return ConstantLog<capacity>{*this, std::string{data.begin(), data.end()}};
	//	}

	//	constexpr auto operator<<(const char& data) const {
	//		const char toString[2] = { data, '\0' };
	//		return ConstantLog<capacity>{*this, std::string{toString}};
	//	}
	//};

	template<typename ParameterType>
	static const auto toVector(std::initializer_list<ParameterType> list) {
		std::vector<ParameterType> vector{list.begin(), list.end()};
		return vector;
	}

	template<typename FromParameterType>
	using IteratorType = decltype(std::declval<FromParameterType>().begin());

	template<typename ParameterType>
	static const auto toVector(auto begin, auto end) {
		std::vector<ParameterType> vector{begin, end};
		return vector;
	}

	static const std::string toString(const auto& from) {
		return std::string{from};
	}

	template<typename ParameterType>
	requires(ParameterType::operator std::string)
	static const std::string toString(const ParameterType& from) {
		return std::string{from};
	}

	std::string_view removeLastCharacter(std::convertible_to<std::string_view> auto from) {
		auto view = std::string_view{from};
		return std::string_view{view.begin(), view.end() - 1};
	}
}

#endif // COMPILER__LOG__EXPLORER__UTILITY__HPP__INCLUDE__GUARD

