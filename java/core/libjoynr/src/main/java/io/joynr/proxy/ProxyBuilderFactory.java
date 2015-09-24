package io.joynr.proxy;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2015 BMW Car IT GmbH
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

import io.joynr.capabilities.LocalCapabilitiesDirectory;
import io.joynr.dispatcher.rpc.JoynrInterface;
import io.joynr.messaging.routing.MessageRouter;
import joynr.system.routingtypes.Address;

public class ProxyBuilderFactory {

    private final LocalCapabilitiesDirectory capabilitiesDirectory;
    private final ProxyInvocationHandlerFactory proxyInvocationHandlerFactory;
    private final MessageRouter messageRouter;
    private final Address libjoynrMessageAddress;

    public ProxyBuilderFactory(LocalCapabilitiesDirectory capabilitiesDirectory,
                               ProxyInvocationHandlerFactory proxyInvocationHandlerFactory,
                               MessageRouter messageRouter,
                               Address libjoynrMessageAddress) {
        this.capabilitiesDirectory = capabilitiesDirectory;
        this.proxyInvocationHandlerFactory = proxyInvocationHandlerFactory;
        this.messageRouter = messageRouter;
        this.libjoynrMessageAddress = libjoynrMessageAddress;
    }

    public <T extends JoynrInterface> ProxyBuilder<T> get(String domain, Class<T> interfaceClass) {
        return new ProxyBuilderDefaultImpl<T>(capabilitiesDirectory,
                                              domain,
                                              interfaceClass,
                                              proxyInvocationHandlerFactory,
                                              messageRouter,
                                              libjoynrMessageAddress);
    }
}