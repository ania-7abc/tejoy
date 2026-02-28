// storage.hpp
#ifndef STORAGE_HPP
#define STORAGE_HPP

#include <nlohmann/json.hpp>
#include <string>

class Storage
{
private:
    std::string storage_path_;

    void save_impl(const std::string &path, const nlohmann::json &j);
    nlohmann::json load_impl(const std::string &path);

public:
    nlohmann::json data;

    Storage(std::string storage_path) : storage_path_(storage_path) {}

    void save();
    void load();
    void update();
    void remove();
};

#endif // STORAGE_HPP
