package io.joynr.systemintegrationtest.jee;

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

import javax.ejb.Stateless;

import joynr.test.SystemIntegrationTestSync;

import io.joynr.jeeintegration.api.ServiceProvider;

@Stateless
@ServiceProvider(serviceInterface = SystemIntegrationTestSync.class)
public class SystemIntegrationTestBean implements SystemIntegrationTestSync {

    @Override
    public Integer add(Integer addendA, Integer addendB) {
        return addendA + addendB;
    }

}