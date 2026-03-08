// storage.hpp
#pragma once

#include <string>

#include <nlohmann/json.hpp>
#include <utility>

class Storage
{
  private:
    std::string storage_path_;

    void save_impl(const std::string &path, const nlohmann::json &json) const;
    auto load_impl(const std::string &path) -> nlohmann::json;

    nlohmann::json data_;

  public:
    auto data() -> nlohmann::json &
    {
        return data_;
    }

    explicit Storage(std::string storage_path) : storage_path_(std::move(std::move(storage_path)))
    {
    }

    void save() const;
    void load();
    void update();
    void remove();
};
