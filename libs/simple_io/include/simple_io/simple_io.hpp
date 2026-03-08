// simple_io.hpp
#pragma once

#include <filesystem>
#include <format>
#include <fstream>
#include <ios>
#include <iterator>
#include <string>
#include <vector>

class SimpleIO
{
  public:
    template <typename... Args> static void write(const std::string_view &fmt, const std::string &data, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        std::ofstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(path);
        file << data;
        file.close();
    }

    template <typename... Args> static auto read(const std::string_view &fmt, Args &&...args) -> std::string
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        std::ifstream file;
        file.exceptions(std::ios::failbit | std::ios::badbit);
        file.open(path);
        std::string temp((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        file.close();
        return temp;
    }

    template <typename... Args> static auto is_file(const std::string_view &fmt, Args &&...args) -> bool
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        return std::filesystem::is_regular_file(path);
    }

    template <typename... Args> static void remove(const std::string_view &fmt, Args &&...args)
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        std::filesystem::remove_all(path);
    }

    template <typename... Args> static auto exists(const std::string_view &fmt, Args &&...args) -> bool
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        return std::filesystem::exists(path);
    }

    template <typename... Args>
    static auto list_dir(const std::string_view &fmt, Args &&...args) -> std::vector<std::string>
    {
        std::string path = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        std::vector<std::string> entries;
        for (const auto &entry : std::filesystem::directory_iterator(path))
        {
            entries.push_back(entry.path().filename().string());
        }
        return entries;
    }

    template <typename... Args> static void create_path(const std::string_view &fmt, Args &&...args)
    {
        std::string goose = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        std::filesystem::create_directories(goose);
    }
};
