package io.joynr.dispatching.rpc;

import java.util.Collections;
import java.util.HashSet;

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

import java.util.List;
import java.util.Set;

import joynr.Reply;
import joynr.Request;

public class SynchronizedReplyCaller implements ReplyCaller {
    private List<Object> responsePayloadContainer;
    final private String fromParticipantId;
    final private Set<String> toParticipantIds;
    final private String requestReplyId;
    final private Request request;

    public SynchronizedReplyCaller(String fromParticipantId,
                                   String toParticipantId,
                                   String requestReplyId,
                                   Request request) {
        this.fromParticipantId = fromParticipantId;
        toParticipantIds = new HashSet<>();
        toParticipantIds.add(toParticipantId);
        this.requestReplyId = requestReplyId;
        this.request = request;
    }

    public SynchronizedReplyCaller(String fromParticipantId,
                                   Set<String> toParticipantIds,
                                   String requestReplyId,
                                   Request request) {
        this.fromParticipantId = fromParticipantId;
        this.toParticipantIds = Collections.unmodifiableSet(toParticipantIds);
        this.requestReplyId = requestReplyId;
        this.request = request;
    }

    @Override
    public void messageCallBack(Reply payload) {
        if (responsePayloadContainer == null) {
            throw new IllegalStateException("SynchronizedReplyCaller: ResponseContainer not set!");
        }
        synchronized (responsePayloadContainer) {
            responsePayloadContainer.add(payload);
            responsePayloadContainer.notify();
        }

    }

    @Override
    public void error(Throwable error) {
        synchronized (responsePayloadContainer) {
            responsePayloadContainer.add(error);
            responsePayloadContainer.notify();
        }

    }

    public void setResponseContainer(List<Object> responsePayloadContainer) {
        this.responsePayloadContainer = responsePayloadContainer;

    }

    @Override
    public String toString() {
        StringBuilder stringBuilder = new StringBuilder();
        stringBuilder.append("ReplyCaller: ");
        stringBuilder.append("\r\n");
        stringBuilder.append("sender: ");
        stringBuilder.append(fromParticipantId);
        stringBuilder.append("\r\n");
        stringBuilder.append("receiver: ");
        stringBuilder.append(toParticipantIds);
        stringBuilder.append("\r\n");
        stringBuilder.append("requestReplyId: ");
        stringBuilder.append(requestReplyId);
        stringBuilder.append("\r\n");
        stringBuilder.append("request: ");
        stringBuilder.append(request);
        stringBuilder.append("\r\n");
        return stringBuilder.toString();
    }

    @Override
    public String getRequestReplyId() {
        return requestReplyId;
    }

}
