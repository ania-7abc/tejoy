// storage.cpp

#include <simple_io/simple_io.hpp>
#include <storage/storage.hpp>

#include <vector>

void Storage::save_impl(const std::string &path, const nlohmann::json &json) const
{
    if (json.is_object() || json.is_array())
    {
        SimpleIO::create_path(path);
        for (const auto &[key, val] : json.items())
        {
            save_impl(std::string(path).append("/").append(key), val);
        }
        if (json.is_array())
        {
            SimpleIO::write("{}/.array", "", path);
        }
    }
    else
    {
        SimpleIO::write(path, json.dump());
    }
}

auto Storage::load_impl(const std::string &path) -> nlohmann::json
{
    if (SimpleIO::is_file(path))
    {
        return nlohmann::json::parse(SimpleIO::read(path));
    }

    std::vector<std::string> entries = SimpleIO::list_dir(path);
    if (SimpleIO::exists("{}/.array", path))
    {
        nlohmann::json arr = nlohmann::json::array();
        for (const std::string &name : entries)
        {
            if (name == ".array")
            {
                continue;
            }
            arr[std::stoul(name)] = load_impl(std::string(path).append("/").append(name));
        }
        return arr;
    }

    nlohmann::json obj = nlohmann::json::object();
    for (const std::string &name : entries)
    {
        obj[name] = load_impl(std::string(path).append("/").append(name));
    }
    return obj;
}

void Storage::save() const
{
    if (SimpleIO::exists(storage_path_))
    {
        SimpleIO::remove(storage_path_);
    }
    save_impl(storage_path_, data_);
}

void Storage::load()
{
    if (!SimpleIO::exists(storage_path_))
    {
        data_ = nlohmann::json::object();
        return;
    }
    data_ = load_impl(storage_path_);
}

void Storage::update()
{
    save();
    load();
}

void Storage::remove()
{
    data_ = nlohmann::json::object();
    SimpleIO::remove(storage_path_);
}
