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
