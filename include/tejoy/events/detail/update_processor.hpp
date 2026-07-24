/**
 * @file update_processor.hpp
 * @brief Defines the events needed to control the update processing module
 * @copyright Copyright (c) 2026 ania-7abc SPDX-License-Identifier: MIT
 */

#pragma once

#include <tejoy/user.hpp>

namespace tejoy::events::detail
{
    namespace requests
    {
        struct SendUpdate
        {
        };
    }

    struct UpdateReceived
    {
    };
}
