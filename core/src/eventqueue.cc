#include "core/eventqueue.h"

void h_core::EventQueue::clear() {
    m_queue.clear();
}

void h_core::EventQueue::postEvent(Event event) {
    m_queue.push_back(event);
}

size_t h_core::EventQueue::getSize() const {
    return m_queue.size();
}

const h_core::Event* h_core::EventQueue::getHeadPointer() const {
    return m_queue.data();
}