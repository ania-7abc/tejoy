#include <tejoy/node.hpp>
#include <utility>

namespace tejoy
{
    Node::Node(boost::asio::any_io_executor executor, nlohmann::json& config)
        : executor_(std::move(executor)), event_bus_(executor_), config_(config)
    {
    }

    auto Node::get_bus() -> event_bus::EventBus&
    {
        return event_bus_;
    }
}
