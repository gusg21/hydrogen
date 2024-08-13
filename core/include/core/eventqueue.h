#pragma once

#include "tinystl/allocator.h"
#include "tinystl/vector.h"

#include "core/event.h"

namespace h_core
{
class EventQueue {
public:
    EventQueue();
    EventQueue(const EventQueue&) = delete;
    EventQueue& operator=(const EventQueue&) = delete;

    void clear();
    void postEvent(h_core::Event event);
    size_t getSize();
    h_core::Event* getHeadPointer();

private:
    tinystl::vector<h_core::Event> m_queue {};
};
} // namespace h_core
