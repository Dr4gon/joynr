package io.joynr.messaging.websocket;

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

import java.io.IOException;

import org.eclipse.jetty.websocket.api.WebSocketAdapter;

import com.fasterxml.jackson.databind.ObjectMapper;
import com.google.inject.Inject;
import com.google.inject.name.Named;

import io.joynr.messaging.FailureAction;
import io.joynr.messaging.IMessagingSkeleton;
import io.joynr.messaging.routing.MessageRouter;
import joynr.JoynrMessage;
import joynr.system.RoutingTypes.WebSocketAddress;

/**
 *
 */
public class WebSocketMessagingSkeleton extends WebSocketAdapter implements IMessagingSkeleton {
    MessageRouter messageRouter;
    ObjectMapper objectMapper;
    JoynrWebSocketEndpoint webSocketEndpoint;
    private WebSocketEndpointFactory webSocketEndpointFactory;
    protected WebSocketAddress serverAddress;

    @Inject
    public WebSocketMessagingSkeleton(@Named(WebsocketModule.WEBSOCKET_SERVER_ADDRESS) WebSocketAddress serverAddress,
                                      WebSocketEndpointFactory webSocketEndpointFactory,
                                      MessageRouter messageRouter,
                                      ObjectMapper objectMapper) {
        this.serverAddress = serverAddress;
        this.webSocketEndpointFactory = webSocketEndpointFactory;
        this.messageRouter = messageRouter;
        this.objectMapper = objectMapper;
    }

    @Override
    public void init() {
        webSocketEndpoint = webSocketEndpointFactory.create(serverAddress);
        webSocketEndpoint.setMessageListener(this);
        webSocketEndpoint.start();
    }

    @Override
    public void transmit(JoynrMessage message, FailureAction failureAction) {
        try {
            messageRouter.route(message);
        } catch (Exception exception) {
            failureAction.execute(exception);
        }
    }

    @Override
    public void transmit(String serializedMessage, FailureAction failureAction) {
        try {
            JoynrMessage message = objectMapper.readValue(serializedMessage, JoynrMessage.class);
            transmit(message, failureAction);
        } catch (IOException error) {
            failureAction.execute(error);
        }
    }

    @Override
    public void shutdown() {
        if (webSocketEndpoint != null) {
            webSocketEndpoint.shutdown();
        }
    }
}
