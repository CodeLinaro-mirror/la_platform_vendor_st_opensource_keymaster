/******************************************************************************
 *
 *  Copyright 2020 NXP
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *  http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 ******************************************************************************/
  /******************************************************************************
 **
 ** The original Work has been changed by THALES.
 **
 ** Licensed under the Apache License, Version 2.0 (the "License");
 ** you may not use this file except in compliance with the License.
 ** You may obtain a copy of the License at
 **
 ** http://www.apache.org/licenses/LICENSE-2.0
 **
 ** Unless required by applicable law or agreed to in writing, software
 ** distributed under the License is distributed on an "AS IS" BASIS,
 ** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 ** See the License for the specific language governing permissions and
 ** limitations under the License.
 **
 ** Copyright ©2023-2024 THALES. All rights Reserved.
 **
 *********************************************************************************/
#pragma once

#include <chrono>
#include <condition_variable>
#include <cstdint>
#include <mutex>
#include <thread>

class EseWakeLockManager {
public:
    EseWakeLockManager();
    ~EseWakeLockManager();

    void maintainActive(int64_t durationMs);

private:
    void workerLoop();

    bool acquireWakeLock();
    bool releaseWakeLock();

    std::mutex mMutex;
    std::condition_variable mCv;
    std::thread mWorker;

    bool mStop = false;
    bool mWakeLockHeld = false;

    std::chrono::steady_clock::time_point mExpiryTime;
};
