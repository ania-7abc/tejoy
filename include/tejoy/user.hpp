// user.hpp
#pragma once
#include <boost/functional/hash.hpp>
#include <cstdint>
#include <nlohmann/json.hpp>
#include <secret_box/secret_box.hpp>
#include <string>

namespace tejoy
{

struct User
{
	SecretBox box = SecretBox(false);
	std::string ip;
	uint16_t port{};
};

auto operator==(const User &lhs, const User &rhs) -> bool;

void to_json(nlohmann::json &json, const User &user);
void from_json(const nlohmann::json &json, User &user);

} // namespace tejoy

template <> struct std::hash<tejoy::User>
{
	auto operator()(const tejoy::User &user) const noexcept -> std::size_t;
}; // namespace std
