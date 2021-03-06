package io.joynr.pubsub.subscription;

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

public interface AttributeSubscriptionListener<T> {
    /**
     * Gets called when the subscription is successfully registered at the provider
     *
     * Since the onSubscribed callback is called by a communication middleware thread, it should
     * not be blocked, wait for user interaction, or do larger computation.
     *
     * @param subscriptionId the subscription id of the subscription as string
     */
    void onSubscribed(String subscriptionId);

    /**
     * Gets called on every received publication
     *
     * Since the onReceive callback is called by a communication middleware thread, it should not
     * be blocked, wait for user interaction, or do larger computation.
     *
     * @param value associated with the subscription this listener is listening to
     */
    void onReceive(T value);

    /**
     * Gets called on every error that is detected on the subscription
     *
     * Since the onError callback is called by a communication middleware thread, it should not
     * be blocked, wait for user interaction, or do larger computation.
     *
     * @param error JoynrRuntimeException describing the error
     */
    void onError(JoynrRuntimeException error);
}
