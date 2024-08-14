#include "core/eventqueue.h"

h_core::EventQueue::EventQueue() {}

void h_core::EventQueue::clear() {
    m_queue.clear();
}

void h_core::EventQueue::postEvent(Event event) {
    m_queue.push_back(event);
}

size_t h_core::EventQueue::getSize() {
    return m_queue.size();
}

h_core::Event* h_core::EventQueue::getHeadPointer() {
    return m_queue.data();
}