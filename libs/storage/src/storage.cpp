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

// storage.cpp
#include "storage.hpp"
#include <simple_io.hpp>
#include <vector>
#include <functional>

void Storage::save_impl(const std::string &path, const nlohmann::json &j)
{
    if (j.is_object() || j.is_array())
    {
        SimpleIO::create_path(path);
        for (auto &[key, val] : j.items())
            save_impl(path + "/" + key, val);
        if (j.is_array())
            SimpleIO::write("{}/.array", "", path);
    }
    else
        SimpleIO::write(path, j.dump());
}

nlohmann::json Storage::load_impl(const std::string &path)
{
    if (SimpleIO::is_file(path))
        return nlohmann::json::parse(SimpleIO::read(path));

    std::vector<std::string> entries = SimpleIO::list_dir(path);
    if (SimpleIO::exists("{}/.array", path))
    {
        nlohmann::json arr = nlohmann::json::array();
        for (const std::string &name : entries)
        {
            if (name == ".array")
                continue;
            arr[std::stoul(name)] = load_impl(path + "/" + name);
        }
        return arr;
    }
    else
    {
        nlohmann::json obj = nlohmann::json::object();
        for (const std::string &name : entries)
            obj[name] = load_impl(path + "/" + name);
        return obj;
    }
}

void Storage::save()
{
    if (SimpleIO::exists(storage_path_))
        SimpleIO::remove(storage_path_);
    save_impl(storage_path_, data);
}

void Storage::load()
{
    if (!SimpleIO::exists(storage_path_))
    {
        data = nlohmann::json::object();
        return;
    }
    data = load_impl(storage_path_);
}

void Storage::update()
{
    save();
    load();
}

void Storage::remove()
{
    data = nlohmann::json::object();
    SimpleIO::remove(storage_path_);
}
