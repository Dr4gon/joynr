package io.joynr.discovery;

/*
 * #%L
 * %%
 * Copyright (C) 2011 - 2013 BMW Car IT GmbH
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

import io.joynr.runtime.AbstractJoynrApplication;
import joynr.infrastructure.ChannelUrlDirectoryAbstractProvider;
import joynr.infrastructure.GlobalCapabilitiesDirectoryAbstractProvider;

import com.google.inject.Inject;
import com.google.inject.persist.PersistService;

public class DiscoveryDirectoriesLauncher extends AbstractJoynrApplication {

    private static final String AUTH_TOKEN = "DiscoveryDirectoryLauncher";

    @Inject
    private ChannelUrlDirectoryAbstractProvider channelUrlDirectoryProvider;

    @Inject
    private GlobalCapabilitiesDirectoryAbstractProvider capabilitiesDirectoryProvider;

    private PersistService persistService;

    @Inject
    public DiscoveryDirectoriesLauncher(PersistService persistService) {
        this.persistService = persistService;
        persistService.start();
    }

    @Override
    public void run() {
        runtime.registerCapability(localDomain, channelUrlDirectoryProvider, AUTH_TOKEN);
        runtime.registerCapability(localDomain, capabilitiesDirectoryProvider, AUTH_TOKEN);
    }

    @Override
    public void shutdown() {
        super.shutdown();
        persistService.stop();
    }
}