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
#ifndef IMESSAGESENDER_H
#define IMESSAGESENDER_H

#include "joynr/MessagingSettings.h"
#include "joynr/JoynrMessage.h"
#include "joynr/system/RoutingTypes/Address.h"
#include "joynr/exceptions/JoynrException.h"

#include <string>
#include <memory>

namespace joynr
{

class IMessageSender
{
public:
    virtual ~IMessageSender() = default;
    /**
    * @brief Sends the message to the given channel.
    */
    virtual void sendMessage(
            const joynr::system::RoutingTypes::Address& destinationAddress,
            const JoynrMessage& message,
            const std::function<void(const exceptions::JoynrRuntimeException&)>& onFailure) = 0;

    virtual void registerReceiveQueueStartedCallback(
            std::function<void(void)> waitForReceiveQueueStarted) = 0;
};
} // namespace joynr

#endif // IMESSAGESENDER_H
