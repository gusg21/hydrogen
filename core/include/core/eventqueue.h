#pragma once

#include <vector>

#include "core/event.h"

namespace h_core {
class EventQueue {
  public:
    EventQueue() = default;
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    /// @brief clear the queue
    void clear();

    /// @brief put an event onto the queue
    /// @param event the event
    void postEvent(h_core::Event event);

    /// @brief get the current size of the queue
    /// @return the size
    [[nodiscard]] size_t getSize() const;

    /// @brief get a pointer to the beginning of the queue
    /// @return head pointer
    [[nodiscard]] const h_core::Event* getHeadPointer() const;

  private:
    std::vector<h_core::Event> m_queue {};
};
}  // namespace h_core
