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

import com.google.inject.Inject;

import io.joynr.messaging.JoynrMessageSerializer;
import io.joynr.messaging.serialize.AbstractMiddlewareMessageSerializerFactory;
import io.joynr.messaging.serialize.JsonSerializer;
import joynr.system.RoutingTypes.WebSocketAddress;

public class WebSocketMessageSerializerFactory extends AbstractMiddlewareMessageSerializerFactory<WebSocketAddress> {

    private JsonSerializer jsonSerializer;

    @Inject
    public WebSocketMessageSerializerFactory(JsonSerializer jsonSerializer) {
        this.jsonSerializer = jsonSerializer;
    }

    @Override
    protected JoynrMessageSerializer createInternal(WebSocketAddress address) {
        return jsonSerializer;
    }

}
