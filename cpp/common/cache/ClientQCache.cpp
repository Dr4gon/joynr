/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2016 BMW Car IT GmbH
 * %%
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
 * #L%
 */
#include "joynr/ClientQCache.h"

#include <chrono>

#include "joynr/CachedValue.h"

namespace joynr
{

static const int MAX_CUMMULATIVE_CACHE_COST = 1000;

ClientQCache::ClientQCache() : cache(), mutex()
{
    cache.setCacheCapacity(MAX_CUMMULATIVE_CACHE_COST);
}

boost::any ClientQCache::lookUp(const std::string& attributeId)
{
    std::lock_guard<std::mutex> lock(mutex);
    if (!cache.contains(attributeId)) {
        return boost::any();
    }
    CachedValue<boost::any>* entry = cache.object(attributeId);
    return entry->getValue();
}

void ClientQCache::insert(std::string attributeId, boost::any value)
{
    std::lock_guard<std::mutex> lock(mutex);
    auto now = std::chrono::system_clock::now();
    CachedValue<boost::any>* cachedValue = new CachedValue<boost::any>(value, now);
    cache.insert(attributeId, cachedValue);
}
} // namespace joynr
