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
#ifndef ABSTRACTJOYNRPROVIDER_H
#define ABSTRACTJOYNRPROVIDER_H

#include <string>
#include <memory>
#include <map>
#include <vector>

#include "joynr/IJoynrProvider.h"
#include "joynr/types/ProviderQos.h"
#include "joynr/ReadWriteLock.h"
#include "joynr/SubscriptionAttributeListener.h"
#include "joynr/SubscriptionBroadcastListener.h"
#include "joynr/PrivateCopyAssign.h"
#include "joynr/JoynrExport.h"

namespace joynr
{

/**
 * @brief Abstract class that specifies the interface providers need to implement
 * and contains functionality to support listening to onChange events
 */
class JOYNR_EXPORT AbstractJoynrProvider : public virtual IJoynrProvider
{
public:
    /** @brief Default constructor */
    AbstractJoynrProvider();

    /** @brief Destructor */
    ~AbstractJoynrProvider() override;

    // --- Interface to be implemented by Providers ---

    /**
     * @deprecated
     * @see JoynrRuntime#registerProvider
     *
     * @brief Get the provider quality of service settings
     * @return the provider quality of service settings
     */
    [[deprecated("Will be removed by end of the year 2016. Use JoynrRuntime::registerProvider "
                 "instead.")]] types::ProviderQos
    getProviderQos() const override;

    // --- Support for listening to onChange events ---

    /**
     * @brief Register an object that will be informed when the value of an attribute changes
     * @param attributeName The name of the attribute for which publications shall be done
     * @param attributeListener The listener object containing the callbacks for publications and
     * failures
     */
    void registerAttributeListener(const std::string& attributeName,
                                   SubscriptionAttributeListener* attributeListener) override;

    /**
     * @brief Unregister and delete an attribute listener
     * @param attributeName The name of the attribute for which publications shall be stopped
     * @param attributeListener The listener object to be unregisterd
     */
    void unregisterAttributeListener(const std::string& attributeName,
                                     SubscriptionAttributeListener* attributeListener) override;

    /**
     * @brief Register an object that will be informed when an event occurs
     * @param broadcastName The name of the broadcast for which publications shall be done
     * @param broadcastListener The listener object containing the callbacks for publications and
     * failures
     */
    void registerBroadcastListener(const std::string& broadcastName,
                                   SubscriptionBroadcastListener* broadcastListener) override;

    /**
     * @brief Unregister and delete a broadcast listener
     * @param broadcastName The name of the broadcast for which publications shall be done
     * @param broadcastListener The listener object containing the callbacks for publications and
     * failures
     */
    void unregisterBroadcastListener(const std::string& broadcastName,
                                     SubscriptionBroadcastListener* broadcastListener) override;

protected:
    /**
     * @brief Called by subclasses when the value of an attribute changes
     * @param attributeName The name of the attribute whose value changes
     * @param value The new value of the attribute
     */
    template <typename T>
    void onAttributeValueChanged(const std::string& attributeName, const T& value)
    {
        ReadLocker locker(lock);

        if (attributeListeners.find(attributeName) != attributeListeners.cend()) {
            std::vector<SubscriptionAttributeListener*>& listeners =
                    attributeListeners[attributeName];

            // Inform all the attribute listeners for this attribute
            for (SubscriptionAttributeListener* listener : listeners) {
                listener->attributeValueChanged(value);
            }
        }
    }

    /**
     * @brief Called by subclasses when a selective broadcast occurs
     * @param broadcastName The name of the broadcast that occurred
     * @param filters vector containing the broadcast filters
     * @param values The output values of the broadcast
     */
    template <typename BroadcastFilter, typename... Ts>
    void fireSelectiveBroadcast(const std::string& broadcastName,
                                const std::vector<std::shared_ptr<BroadcastFilter>>& filters,
                                const Ts&... values)
    {

        ReadLocker locker(lock);
        const std::vector<SubscriptionBroadcastListener*>& listeners =
                broadcastListeners[broadcastName];
        // Inform all the broadcast listeners for this broadcast
        for (SubscriptionBroadcastListener* listener : listeners) {
            listener->selectiveBroadcastOccurred(filters, values...);
        }
    }

    /**
     * @brief Called by subclasses when a selective broadcast occurs
     * @param broadcastName The name of the broadcast that occurred
     * @param values The output values of the broadcast
     */
    template <typename... Ts>
    void fireBroadcast(const std::string& broadcastName, const Ts&... values)
    {
        ReadLocker locker(lock);
        const std::vector<SubscriptionBroadcastListener*>& listeners =
                broadcastListeners[broadcastName];
        // Inform all the broadcast listeners for this broadcast
        for (SubscriptionBroadcastListener* listener : listeners) {
            listener->broadcastOccurred(values...);
        }
    }

    /**
     * @deprecated
     * @see JoynrRuntime#registerProvider
     *
     * @brief The provider quality settings
     */
    [[deprecated("Will be removed by end of the year 2016. Use JoynrRuntime::registerProvider "
                 "instead.")]] types::ProviderQos providerQos;

private:
    DISALLOW_COPY_AND_ASSIGN(AbstractJoynrProvider);

    ReadWriteLock lock;
    std::map<std::string, std::vector<SubscriptionAttributeListener*>> attributeListeners;
    std::map<std::string, std::vector<SubscriptionBroadcastListener*>> broadcastListeners;

    friend class End2EndBroadcastTest;
    friend class End2EndSubscriptionTest;
};

} // namespace joynr
#endif // ABSTRACTJOYNRPROVIDER_H
