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

// module_manager.hpp
#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "module.hpp"

namespace tejoy::detail::modules
{

    class ModuleManager
    {
    public:
        explicit ModuleManager(event_system::EventBus &bus);

        template <typename T, typename... Args>
        T &create_module(Args &&...args)
        {
            static_assert(std::is_base_of_v<Module, T>, "T must be derived from Module");
            auto module = std::make_shared<T>(bus_, std::forward<Args>(args)...);
            auto it = modules_.find(std::type_index(typeid(T)));
            if (it != modules_.end())
                throw std::runtime_error("Module of this type already exists");
            modules_[std::type_index(typeid(T))] = module;
            return *module;
        }

        template <typename T>
        T &get_module()
        {
            auto it = modules_.find(std::type_index(typeid(T)));
            if (it == modules_.end())
                throw std::runtime_error("Module not found");
            return *std::dynamic_pointer_cast<T>(it->second);
        }

        template <typename T>
        void restart_module()
        {
            auto &mod = get_module<T>();
            mod.on_stop();
            mod.on_start();
        }

        template <typename T, typename... Args>
        void recreate_module(Args &&...args)
        {
            auto it = modules_.find(std::type_index(typeid(T)));
            if (it != modules_.end())
            {
                it->second->on_stop();
                modules_.erase(it);
            }
            create_module<T>(std::forward<Args>(args)...);
        }

        void start_all();
        void stop_all();

    private:
        event_system::EventBus &bus_;
        std::unordered_map<std::type_index, std::shared_ptr<Module>> modules_;
    };

} // namespace tejoy
