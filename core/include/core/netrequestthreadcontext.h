//
// Created by Angus Goucher on 9/6/2024.
//

#pragma once

#include <atomic>
#include <mutex>
#include <deque>

#include "core/netrequestjob.h"

namespace h_core {
class NetRequestThreadContext {
  public:
    NetRequestThreadContext() = default;

    std::atomic_bool shouldNetRequestThreadDie = false;
    std::atomic_bool netAssetsFlushDone = false;
    std::mutex assetListLock {};
    std::mutex jobQueueLock {};
    std::deque<h_core::NetRequestJob> jobs {};
    h_core::Assets* assets = nullptr;
};
}