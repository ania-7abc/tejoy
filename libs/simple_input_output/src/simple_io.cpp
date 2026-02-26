#include <simple_io.hpp>

#include <cstdio>
#include <cstdarg>
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

std::string SimpleIO::read(const char *filename_fmt, ...)
{
    va_list args;
    va_start(args, filename_fmt);
    std::string filename = format_filename(filename_fmt, args);
    va_end(args);

    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for reading: " + filename);
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size))
        throw std::runtime_error("Read failed: " + filename);
    return std::string(buffer.data(), size);
}

void SimpleIO::write(const char *filename_fmt, const std::string &content, ...)
{
    va_list args;
    va_start(args, content);
    std::string filename = format_filename(filename_fmt, args);
    va_end(args);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot open file for writing: " + filename);
    file.write(content.data(), content.size());
    if (!file.good())
        throw std::runtime_error("Write failed: " + filename);
}

void SimpleIO::create(const char *filename_fmt, ...)
{
    va_list args;
    va_start(args, filename_fmt);
    std::string filename = format_filename(filename_fmt, args);
    va_end(args);

    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open())
        throw std::runtime_error("Cannot create file: " + filename);
}

void SimpleIO::remove(const char *filename_fmt, ...)
{
    va_list args;
    va_start(args, filename_fmt);
    std::string filename = format_filename(filename_fmt, args);
    va_end(args);

    if (std::remove(filename.c_str()) != 0)
        throw std::runtime_error("Cannot delete file: " + filename);
}

bool SimpleIO::exists(const char *filename_fmt, ...)
{
    va_list args;
    va_start(args, filename_fmt);
    std::string filename = format_filename(filename_fmt, args);
    va_end(args);

    std::ifstream file(filename);
    return file.good();
}

std::string SimpleIO::format_filename(const char *fmt, va_list args)
{
    va_list args_copy;
    va_copy(args_copy, args);
    int len = vsnprintf(nullptr, 0, fmt, args_copy);
    va_end(args_copy);
    if (len < 0)
        throw std::runtime_error("Filename formatting error");

    std::string result(len, '\0');
    vsnprintf(&result[0], len + 1, fmt, args);
    return result;
}
