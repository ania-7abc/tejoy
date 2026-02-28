// MIT License
// 
// Copyright (c) 2026 Anya Baykina Dmitrievna
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

// simple_io.hpp
#pragma once

#include <string>
#include <vector>
#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <utility>
#include <iterator>

class SimpleIO
{
public:
    template <typename... Args>
    static void write(const std::string_view &fmt, const std::string &data, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        std::ofstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(path);
        file << data;
        file.close();
    }

    template <typename... Args>
    static std::string read(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        std::ifstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(path);
        std::string temp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return temp;
    }

    template <typename... Args>
    static bool is_file(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        return std::filesystem::is_regular_file(path);
    }

    template <typename... Args>
    static void remove(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        std::filesystem::remove_all(path);
    }

    template <typename... Args>
    static bool exists(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        return std::filesystem::exists(path);
    }

    template <typename... Args>
    static std::vector<std::string> list_dir(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(args...));
        std::vector<std::string> entries;
        for (const auto &entry : std::filesystem::directory_iterator(path))
            entries.push_back(entry.path().filename().string());
        return entries;
    }

    template <typename... Args>
    static void create_path(const std::string_view &fmt, Args &&...args)
    {
        std::string goose = std::vformat(fmt, std::make_format_args(args...));
        std::filesystem::create_directories(goose);
    }
};
