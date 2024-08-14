#pragma once

#include "tinystl/allocator.h"
#include "tinystl/vector.h"

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
    size_t getSize();

    /// @brief get a pointer to the beginning of the queue
    /// @return head pointer
    h_core::Event* getHeadPointer();

  private:
    tinystl::vector<h_core::Event> m_queue {};
};
}  // namespace h_core
