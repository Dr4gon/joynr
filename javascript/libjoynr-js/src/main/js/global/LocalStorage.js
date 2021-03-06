/*global localStorage: true*/

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

/**
 * LocalStorage
 *
 * @returns constructor for a localStorage object
 */
define([ "joynr/util/Util"
], function(Util) {
    /**
     * constructor for a localStorage object
     * @param {Object}
     *            settings the settings object
     * @param {Boolean}
     *            settings.clearPersistency localStorage is cleared if set to true
     *
     * @constructor LocalStorage
     */
    var LocalStorage =
            function(settings) {
                settings = settings || {};
                Util.checkPropertyIfDefined(
                        settings.clearPersistency,
                        "Boolean",
                        "settings.clearPersistency");
                if (settings.clearPersistency) {
                    localStorage.clear();
                }
                return localStorage;
            };

    return LocalStorage;
});