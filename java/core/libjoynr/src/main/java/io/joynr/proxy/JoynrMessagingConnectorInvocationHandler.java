package io.joynr.proxy;

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

import java.lang.reflect.Method;
import java.util.Set;

import javax.annotation.CheckForNull;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.joynr.common.ExpiryDate;
import io.joynr.dispatching.DispatcherUtils;
import io.joynr.dispatching.RequestReplyManager;
import io.joynr.dispatching.rpc.ReplyCallerDirectory;
import io.joynr.dispatching.rpc.RpcAsyncRequestReplyCaller;
import io.joynr.dispatching.rpc.RpcUtils;
import io.joynr.dispatching.rpc.SynchronizedReplyCaller;
import io.joynr.dispatching.subscription.SubscriptionManager;
import io.joynr.exceptions.JoynrIllegalStateException;
import io.joynr.exceptions.JoynrRuntimeException;
import io.joynr.messaging.MessagingQos;
import io.joynr.proxy.invocation.AttributeSubscribeInvocation;
import io.joynr.proxy.invocation.BroadcastSubscribeInvocation;
import io.joynr.proxy.invocation.UnsubscribeInvocation;
import joynr.MethodMetaInformation;
import joynr.OneWayRequest;
import joynr.Reply;
import joynr.Request;
import joynr.exceptions.ApplicationException;

//TODO maybe Connector should not be a dynamic proxy. ProxyInvocationHandler could call execute...Method() directly.
final class JoynrMessagingConnectorInvocationHandler implements ConnectorInvocationHandler {
    @SuppressWarnings("unused")
    private static final Logger logger = LoggerFactory.getLogger(JoynrMessagingConnectorInvocationHandler.class);

    private final Set<String> toParticipantIds;
    private final String fromParticipantId;

    private final MessagingQos qosSettings;

    private final RequestReplyManager requestReplyManager;
    private final ReplyCallerDirectory replyCallerDirectory;

    private final SubscriptionManager subscriptionManager;

    JoynrMessagingConnectorInvocationHandler(Set<String> toParticipantIds,
                                             String fromParticipantId,
                                             MessagingQos qosSettings,
                                             RequestReplyManager requestReplyManager,
                                             ReplyCallerDirectory replyCallerDirectory,
                                             SubscriptionManager subscriptionManager) {
        this.toParticipantIds = toParticipantIds;
        this.fromParticipantId = fromParticipantId;

        this.qosSettings = qosSettings;

        this.requestReplyManager = requestReplyManager;
        this.replyCallerDirectory = replyCallerDirectory;
        this.subscriptionManager = subscriptionManager;

    }

    @SuppressWarnings("unchecked")
    @Override
    public Future<?> executeAsyncMethod(Method method, Object[] params, Future<?> future) {

        if (method == null) {
            throw new IllegalArgumentException("Method cannot be null");
        }
        if (toParticipantIds.size() > 1) {
            throw new JoynrIllegalStateException("You can't execute async methods for multiple participants.");
        }
        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have exactly one participant to be able to execute an async method.");
        }

        MethodMetaInformation methodMetaInformation = JoynrMessagingConnectorFactory.ensureMethodMetaInformationPresent(method);
        if (methodMetaInformation.getCallbackAnnotation() == null) {
            throw new JoynrIllegalStateException("All async methods need to have a annotated callback parameter.");
        }

        int callbackIndex = methodMetaInformation.getCallbackIndex();
        ICallback callback = (ICallback) params[callbackIndex];

        Object[] paramsWithoutCallback = new Object[params.length - 1];
        copyArrayWithoutElement(params, paramsWithoutCallback, callbackIndex);
        Class<?>[] paramDatatypes = method.getParameterTypes();
        Class<?>[] paramDatatypesWithoutCallback = new Class<?>[paramDatatypes.length - 1];
        copyArrayWithoutElement(paramDatatypes, paramDatatypesWithoutCallback, callbackIndex);

        Request request = new Request(method.getName(), paramsWithoutCallback, paramDatatypesWithoutCallback);
        String requestReplyId = request.getRequestReplyId();

        RpcAsyncRequestReplyCaller<?> callbackWrappingReplyCaller = new RpcAsyncRequestReplyCaller(requestReplyId,
                                                                                                   callback,
                                                                                                   future,
                                                                                                   method,
                                                                                                   methodMetaInformation);

        ExpiryDate expiryDate = DispatcherUtils.convertTtlToExpirationDate(qosSettings.getRoundTripTtl_ms());

        replyCallerDirectory.addReplyCaller(requestReplyId, callbackWrappingReplyCaller, expiryDate);
        requestReplyManager.sendRequest(fromParticipantId, toParticipantIds.iterator().next(), request, qosSettings);
        return future;
    }

    private void copyArrayWithoutElement(Object[] fromArray, Object[] toArray, int removeIndex) {
        System.arraycopy(fromArray, 0, toArray, 0, removeIndex);
        System.arraycopy(fromArray, removeIndex + 1, toArray, removeIndex, toArray.length - removeIndex);
    }

    @CheckForNull
    @Override
    public Object executeSyncMethod(Method method, Object[] args) throws ApplicationException {

        // TODO does a method with 0 args pass in an empty args array, or null for args?
        if (method == null) {
            throw new IllegalArgumentException("Method cannot be null");
        }
        if (toParticipantIds.size() > 1) {
            throw new JoynrIllegalStateException("You can't execute sync methods for multiple participants.");
        }
        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have exactly one participant to be able to execute a sync method.");
        }

        MethodMetaInformation methodMetaInformation = JoynrMessagingConnectorFactory.ensureMethodMetaInformationPresent(method);

        Request request = new Request(method.getName(), args, method.getParameterTypes());
        Reply reply;
        String requestReplyId = request.getRequestReplyId();
        SynchronizedReplyCaller synchronizedReplyCaller = new SynchronizedReplyCaller(fromParticipantId,
                                                                                      toParticipantIds,
                                                                                      requestReplyId,
                                                                                      request);
        ExpiryDate expiryDate = DispatcherUtils.convertTtlToExpirationDate(qosSettings.getRoundTripTtl_ms());
        replyCallerDirectory.addReplyCaller(requestReplyId, synchronizedReplyCaller, expiryDate);
        reply = (Reply) requestReplyManager.sendSyncRequest(fromParticipantId,
                                                            toParticipantIds.iterator().next(),
                                                            request,
                                                            synchronizedReplyCaller,
                                                            qosSettings);
        if (reply.getError() == null) {
            if (method.getReturnType().equals(void.class)) {
                return null;
            }
            return RpcUtils.reconstructReturnedObject(method, methodMetaInformation, reply.getResponse());
        } else if (reply.getError() instanceof ApplicationException) {
            throw (ApplicationException) reply.getError();
        } else {
            throw (JoynrRuntimeException) reply.getError();
        }

    }

    @Override
    public void executeOneWayMethod(Method method, Object[] args) {
        // TODO does a method with 0 args pass in an empty args array, or null for args?
        if (method == null) {
            throw new IllegalArgumentException("Method cannot be null");
        }
        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have at least one participant to be able to execute an oneWayMethod.");
        }

        OneWayRequest request = new OneWayRequest(method.getName(), args, method.getParameterTypes());
        requestReplyManager.sendOneWayRequest(fromParticipantId, toParticipantIds, request, qosSettings);
    }

    @Override
    public void executeSubscriptionMethod(UnsubscribeInvocation unsubscribeInvocation) {

        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have at least one participant to be able to execute a subscription method.");
        }

        subscriptionManager.unregisterSubscription(fromParticipantId,
                                                   toParticipantIds,
                                                   unsubscribeInvocation.getSubscriptionId(),
                                                   qosSettings);
    }

    @Override
    public void executeSubscriptionMethod(AttributeSubscribeInvocation attributeSubscription) {
        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have at least one participant to be able to execute a subscription method.");
        }

        subscriptionManager.registerAttributeSubscription(fromParticipantId, toParticipantIds, attributeSubscription);
    }

    @Override
    public void executeSubscriptionMethod(BroadcastSubscribeInvocation broadcastSubscription) {

        if (toParticipantIds.isEmpty()) {
            throw new JoynrIllegalStateException("You must have at least one participant to be able to execute a subscription method.");
        }

        subscriptionManager.registerBroadcastSubscription(fromParticipantId, toParticipantIds, broadcastSubscription);
    }

}
