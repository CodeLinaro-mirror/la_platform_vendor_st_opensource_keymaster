/******************************************************************************
 *
 *  Copyright 2020-2021 NXP
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
#include "EseWakeLockManager.h"

#include <android-base/logging.h>

#include <fcntl.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <string>

namespace {

constexpr const char* kWakeLockPath = "/sys/power/wake_lock";
constexpr const char* kWakeUnlockPath = "/sys/power/wake_unlock";
constexpr const char* kWakeLockName = "ese_weaver_wakelock";

bool writeStringToFile(const char* path, const char* value) {
    int fd = TEMP_FAILURE_RETRY(open(path, O_WRONLY | O_CLOEXEC));
    if (fd < 0) {
        PLOG(ERROR) << "Failed to open " << path;
        return false;
    }

    const size_t len = strlen(value);
    ssize_t written = TEMP_FAILURE_RETRY(write(fd, value, len));

    if (close(fd) != 0) {
        PLOG(ERROR) << "Failed to close " << path;
    }

    if (written != static_cast<ssize_t>(len)) {
        PLOG(ERROR) << "Failed to write wake lock value to " << path;
        return false;
    }

    return true;
}

}  // namespace

EseWakeLockManager::EseWakeLockManager() {
    mExpiryTime = std::chrono::steady_clock::now();
    mWorker = std::thread(&EseWakeLockManager::workerLoop, this);

    LOG(INFO) << "EseWakeLockManager started";
}

EseWakeLockManager::~EseWakeLockManager() {
    {
        std::lock_guard<std::mutex> lock(mMutex);
        mStop = true;
    }

    mCv.notify_all();

    if (mWorker.joinable()) {
        mWorker.join();
    }

    bool needRelease = false;
    {
        std::lock_guard<std::mutex> lock(mMutex);
        needRelease = mWakeLockHeld;
        mWakeLockHeld = false;
    }

    if (needRelease) {
        releaseWakeLock();
    }

    LOG(INFO) << "EseWakeLockManager stopped";
}

void EseWakeLockManager::maintainActive(int64_t durationMs) {
    if (durationMs <= 0) {
        LOG(INFO) << "No eSE wake lock needed, durationMs=" << durationMs;
        return;
    }

    const auto now = std::chrono::steady_clock::now();
    const auto newExpiryTime = now + std::chrono::milliseconds(durationMs);

    bool needAcquire = false;

    {
        std::lock_guard<std::mutex> lock(mMutex);

        if (!mWakeLockHeld) {
            mWakeLockHeld = true;
            needAcquire = true;
        }

        // Extend only. Do not shorten an already longer keep-alive window.
        mExpiryTime = std::max(mExpiryTime, newExpiryTime);
    }

    if (needAcquire) {
        if (!acquireWakeLock()) {
            std::lock_guard<std::mutex> lock(mMutex);
            mWakeLockHeld = false;
            return;
        }
    }

    mCv.notify_all();

    LOG(INFO) << "eSE wake lock active for another " << durationMs << " ms";
}

void EseWakeLockManager::workerLoop() {
    std::unique_lock<std::mutex> lock(mMutex);

    while (!mStop) {
        if (!mWakeLockHeld) {
            mCv.wait(lock, [this] {
                return mStop || mWakeLockHeld;
            });
            continue;
        }

        const auto expirySnapshot = mExpiryTime;

        const bool wokeBecauseChanged =
                mCv.wait_until(lock, expirySnapshot, [this, expirySnapshot] {
                    return mStop || !mWakeLockHeld || mExpiryTime != expirySnapshot;
                });

        if (wokeBecauseChanged) {
            continue;
        }

        if (mStop) {
            break;
        }

        if (mWakeLockHeld && std::chrono::steady_clock::now() >= mExpiryTime) {
            mWakeLockHeld = false;

            lock.unlock();
            releaseWakeLock();
            lock.lock();

            LOG(INFO) << "eSE wake lock expired and released";
        }
    }
}

bool EseWakeLockManager::acquireWakeLock() {
    LOG(INFO) << "Acquiring eSE wake lock: " << kWakeLockName;
    return writeStringToFile(kWakeLockPath, kWakeLockName);
}

bool EseWakeLockManager::releaseWakeLock() {
    LOG(INFO) << "Releasing eSE wake lock: " << kWakeLockName;
    return writeStringToFile(kWakeUnlockPath, kWakeLockName);
}
