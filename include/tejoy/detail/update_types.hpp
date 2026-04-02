// update_types.hpp
#pragma once
#include <string_view>

namespace tejoy::detail::UpdateTypes
{

inline constexpr std::string_view MESSAGE = "message";
inline constexpr std::string_view ACK = "ack";
inline constexpr std::string_view DIS_FIND = "dis_find";
inline constexpr std::string_view DIS_OK = "dis_ok";
inline constexpr std::string_view PING = "ping";
inline constexpr std::string_view PONG = "pong";

} // namespace tejoy::detail::UpdateTypes
