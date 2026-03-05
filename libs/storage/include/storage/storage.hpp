// storage.hpp
#pragma once

#include <string>

#include <nlohmann/json.hpp>

class Storage
{
private:
  std::string storage_path_;

  void save_impl(const std::string &path, const nlohmann::json &j) const;
  nlohmann::json load_impl(const std::string &path);

public:
  nlohmann::json data;

  Storage(std::string storage_path) : storage_path_(storage_path) {}

  void save() const;
  void load();
  void update();
  void remove();
};
