package io.joynr.dispatching.subscription;

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

import io.joynr.exceptions.JoynrRuntimeException;
import io.joynr.messaging.MessagingQos;
import io.joynr.proxy.invocation.AttributeSubscribeInvocation;
import io.joynr.proxy.invocation.BroadcastSubscribeInvocation;
import io.joynr.pubsub.subscription.AttributeSubscriptionListener;
import io.joynr.pubsub.subscription.BroadcastSubscriptionListener;
import joynr.SubscriptionReply;

import java.util.Set;

import javax.annotation.CheckForNull;

public interface SubscriptionManager {

    void registerAttributeSubscription(String fromParticipantId,
                                       Set<String> toParticipantIds,
                                       AttributeSubscribeInvocation subscriptionRequest);

    void registerBroadcastSubscription(String fromParticipantId,
                                       Set<String> toParticipantIds,
                                       BroadcastSubscribeInvocation subscriptionRequest);

    void unregisterSubscription(String fromParticipantId,
                                Set<String> toParticipantIds,
                                String subscriptionId,
                                MessagingQos qosSettings);

    void handleSubscriptionReply(SubscriptionReply subscriptionReply);

    void touchSubscriptionState(final String subscriptionId);

    Class<?> getAttributeType(String subscriptionId);

    Class<?>[] getBroadcastOutParameterTypes(String subscriptionId);

    boolean isBroadcast(String subscriptionId);

    BroadcastSubscriptionListener getBroadcastSubscriptionListener(String subscriptionId);

    @CheckForNull
    <T> AttributeSubscriptionListener<T> getSubscriptionListener(String subscriptionId);

    void handleBroadcastPublication(String subscriptionId, Object[] broadcastValues);

    <T> void handleAttributePublication(String subscriptionId, T attributeValue);

    <T> void handleAttributePublicationError(String subscriptionId, JoynrRuntimeException error);
}
