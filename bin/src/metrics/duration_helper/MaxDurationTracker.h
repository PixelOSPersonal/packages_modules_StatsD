/*
 * Copyright (C) 2017 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef MAX_DURATION_TRACKER_H
#define MAX_DURATION_TRACKER_H

#include "DurationTracker.h"

#include "frameworks/base/cmds/statsd/src/stats_log.pb.h"

namespace android {
namespace os {
namespace statsd {

// Tracks a pool of atom durations, and output the max duration for each bucket.
// To get max duration, we need to keep track of each individual durations, and compare them when
// they stop or bucket expires.
class MaxDurationTracker : public DurationTracker {
public:
    MaxDurationTracker(sp<ConditionWizard> wizard, int conditionIndex,
                       uint64_t currentBucketStartNs, uint64_t bucketSizeNs,
                       std::vector<DurationBucketInfo>& bucket);
    void noteStart(const HashableDimensionKey& key, bool condition, const uint64_t eventTime,
                   const ConditionKey& conditionKey) override;
    void noteStop(const HashableDimensionKey& key, const uint64_t eventTime) override;
    void noteStopAll(const uint64_t eventTime) override;
    bool flushIfNeeded(uint64_t timestampNs) override;
    void onSlicedConditionMayChange(const uint64_t timestamp) override;
    void onConditionChanged(bool condition, const uint64_t timestamp) override;

private:
    std::map<HashableDimensionKey, DurationInfo> mInfos;

    void noteConditionChanged(const HashableDimensionKey& key, bool conditionMet,
                              const uint64_t timestamp);
};

}  // namespace statsd
}  // namespace os
}  // namespace android

#endif  // MAX_DURATION_TRACKER_H