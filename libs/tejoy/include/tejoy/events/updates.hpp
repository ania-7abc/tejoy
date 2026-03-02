// MIT License
//
// Copyright (c) 2026 ania_7 (Anya Baykina Dmitrievna)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

// updates.hpp
#pragma once
#include <string>
#include <tejoy/user.hpp>
#include <event_system/event.hpp>

namespace tejoy::events
{

    struct MessageUpdateReceived : event_system::Event
    {
        MessageUpdateReceived(std::string text, uint32_t pkg_id, User from)
            : text(std::move(text)), pkg_id(pkg_id), from(std::move(from)) {}
        std::string text;
        uint32_t pkg_id;
        User from;
    };

    struct AckUpdateReceived : event_system::Event
    {
        AckUpdateReceived(uint32_t pkg_id, User from)
            : pkg_id(pkg_id), from(std::move(from)) {}
        uint32_t pkg_id;
        User from;
    };

} // namespace tejoy::events
