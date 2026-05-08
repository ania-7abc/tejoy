// json_variant.hpp
#pragma once
#include <nlohmann/json.hpp>
#include <variant>

namespace nlohmann
{

template <typename... Ts> void from_json(const json &json, std::variant<Ts...> &variant)
{
	auto try_set = [&json, &variant]<typename T>() -> bool {
		if (!variant.valueless_by_exception() && std::holds_alternative<T>(variant))
			return false;
		try
		{
			variant = json.get<T>();
			return true;
		}
		catch (const json::exception &)
		{
			return false;
		}
	};

	if (!(try_set.template operator()<Ts>() || ...))
		throw json::out_of_range::create(501, "cannot parse variant", nullptr); // NOLINT
}

template <typename... Ts> void to_json(json &json, const std::variant<Ts...> &variant)
{
	std::visit([&json](const auto &val) -> void { json = val; }, variant);
}

} // namespace nlohmann
