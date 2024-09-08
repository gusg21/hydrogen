//
// Created by Angus Goucher on 9/6/2024.
//

#pragma once

#include <atomic>
#include <mutex>
#include <deque>

#include "core/netrequestjob.h"
#include "core/netrequestresult.h"

namespace h_core {
class NetRequestThreadContext {
  public:
    NetRequestThreadContext() = default;

    std::atomic_bool netRequestThreadAlive = false;

    std::mutex resultQueueLock {};
    std::mutex jobQueueLock {};
    std::deque<h_core::NetRequestJob> jobs {};
    std::deque<h_core::NetRequestResult> results {};
};
}