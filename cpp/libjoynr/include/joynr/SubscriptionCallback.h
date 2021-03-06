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
#ifndef SUBSCRIPTIONCALLBACK_H
#define SUBSCRIPTIONCALLBACK_H

#include <memory>

#include "joynr/ISubscriptionCallback.h"
#include "joynr/ISubscriptionListener.h"
#include "ISubscriptionManager.h"
#include "joynr/PublicationInterpreter.h"
#include "joynr/PrivateCopyAssign.h"
#include "joynr/Future.h"
#include "joynr/SubscriptionReply.h"

namespace joynr
{

/**
  * @class SubscriptionCallback
  * @brief
  */
template <typename T, typename... Ts>
class SubscriptionCallback : public ISubscriptionCallback
{
public:
    explicit SubscriptionCallback(std::shared_ptr<ISubscriptionListener<T, Ts...>> listener,
                                  std::shared_ptr<Future<std::string>> future,
                                  ISubscriptionManager* subscriptionManager)
            : listener(std::move(listener)),
              future(std::move(future)),
              subscriptionManager(subscriptionManager)
    {
    }

    void onError(const exceptions::JoynrRuntimeException& error) override
    {
        listener->onError(error);
    }

    template <typename Holder = T>
    std::enable_if_t<std::is_void<Holder>::value, void> onSuccess()
    {
        listener->onReceive();
    }

    template <typename Holder = T>
    std::enable_if_t<!std::is_void<Holder>::value, void> onSuccess(const Holder& value,
                                                                   const Ts&... values)
    {
        listener->onReceive(value, values...);
    }

    void execute(SubscriptionPublication&& subscriptionPublication) override
    {
        PublicationInterpreter<T, Ts...>::execute(*this, std::move(subscriptionPublication));
    }

    void execute(const SubscriptionReply& subscriptionReply) override
    {
        std::shared_ptr<exceptions::JoynrRuntimeException> error = subscriptionReply.getError();
        if (error) {
            subscriptionManager->unregisterSubscription(subscriptionReply.getSubscriptionId());
            future->onError(error);
            listener->onError(*error);
        } else {
            future->onSuccess(subscriptionReply.getSubscriptionId());
            listener->onSubscribed(subscriptionReply.getSubscriptionId());
        }
    }

protected:
    std::shared_ptr<ISubscriptionListener<T, Ts...>> listener;
    std::shared_ptr<Future<std::string>> future;
    ISubscriptionManager* subscriptionManager;

private:
    DISALLOW_COPY_AND_ASSIGN(SubscriptionCallback);
};

} // namespace joynr
#endif // SUBSCRIPTIONCALLBACK_H
