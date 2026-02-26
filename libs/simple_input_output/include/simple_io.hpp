#ifndef SIMPLE_IO_HPP
#define SIMPLE_IO_HPP

#include <string>

class SimpleIO
{
public:
    static std::string read(const char *filename_fmt, ...);
    static void write(const char *filename_fmt, const std::string &content, ...);

    static void create(const char *filename_fmt, ...);
    static void remove(const char *filename_fmt, ...);

    static bool exists(const char *filename_fmt, ...);

private:
    static std::string format_filename(const char *fmt, va_list args);
};

#endif // SIMPLE_IO_HPP
