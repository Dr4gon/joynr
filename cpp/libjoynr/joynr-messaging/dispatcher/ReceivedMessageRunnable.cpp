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
#include "libjoynr/joynr-messaging/dispatcher/ReceivedMessageRunnable.h"

#include <cassert>

#include "joynr/Dispatcher.h"

namespace joynr
{

INIT_LOGGER(ReceivedMessageRunnable);

ReceivedMessageRunnable::ReceivedMessageRunnable(const JoynrMessage& message,
                                                 Dispatcher& dispatcher)
        : Runnable(true),
          ObjectWithDecayTime(message.getHeaderExpiryDate()),
          message(message),
          dispatcher(dispatcher)
{
    JOYNR_LOG_DEBUG(
            logger, "Creating ReceivedMessageRunnable for message type: {}", message.getType());
}

void ReceivedMessageRunnable::shutdown()
{
}

void ReceivedMessageRunnable::run()
{
    JOYNR_LOG_DEBUG(logger,
                    "Running ReceivedMessageRunnable for message type: {}, msg ID: {} and "
                    "payload: {}",
                    message.getType(),
                    message.getHeaderMessageId(),
                    message.getPayload());
    if (isExpired()) {
        JOYNR_LOG_DEBUG(
                logger, "Dropping ReceivedMessageRunnable message, because it is expired: ");
        return;
    }

    if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_REQUEST) {
        dispatcher.handleRequestReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_REPLY) {
        dispatcher.handleReplyReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_ONE_WAY) {
        dispatcher.handleOneWayRequestReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_SUBSCRIPTION_REQUEST) {
        dispatcher.handleSubscriptionRequestReceived(message);
    } else if (message.getType() ==
               JoynrMessage::VALUE_MESSAGE_TYPE_BROADCAST_SUBSCRIPTION_REQUEST) {
        dispatcher.handleBroadcastSubscriptionRequestReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_SUBSCRIPTION_REPLY) {
        dispatcher.handleSubscriptionReplyReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_PUBLICATION) {
        dispatcher.handlePublicationReceived(message);
    } else if (message.getType() == JoynrMessage::VALUE_MESSAGE_TYPE_SUBSCRIPTION_STOP) {
        dispatcher.handleSubscriptionStopReceived(message);
    } else {
        JOYNR_LOG_FATAL(logger, "unknown message type: {}", message.getType());
        assert(false);
    }
}

} // namespace joynr
