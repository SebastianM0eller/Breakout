#pragma once

#include <algorithm>
#include <functional>
#include <list>
#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Engine {

class EventManager {
   public:
    template <typename EventType>
    void AddListner(std::function<void(const EventType&)> callback);

    template <typename EventType>
    void SendEvent(const EventType& event);

   private:
    std::unordered_map<std::type_index, std::list<std::shared_ptr<void>>>
        m_EventListners;
};

template <typename EventType>
void EventManager::AddListner(std::function<void(const EventType&)> callback) {
    std::type_index typeName(typeid(EventType));

    auto wrapper =
        std::make_shared<std::function<void(const EventType&)>>(callback);

    m_EventListners[typeName].push_back(std::move(wrapper));
}

template <typename EventType>
void EventManager::SendEvent(const EventType& event) {
    std::type_index typeName(typeid(EventType));

    // Check if we have any callbacks for this event.
    if (m_EventListners.find(typeName) == m_EventListners.end())
        return;

    for (auto& genericCallback : m_EventListners[typeName]) {
        // Exstract the method
        auto callback =
            std::static_pointer_cast<std::function<void(const EventType&)>>(
                genericCallback);

        // Call the method
        (*callback)(event);
    }
}
}  // namespace Engine
