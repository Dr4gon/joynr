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

define("joynr/exceptions/MethodInvocationException", [
    "joynr/types/TypeRegistrySingleton",
    "joynr/util/UtilInternal",
    "joynr/exceptions/JoynrRuntimeException",
    "joynr/system/LoggerFactory"
], function(TypeRegistrySingleton, Util, JoynrRuntimeException, LoggerFactory) {
    var defaultSettings;

    /**
     * @classdesc
     *
     * @summary
     * Constructor of MethodInvocationException object used for reporting
     * error conditions when invoking a method (e.g. method does not
     * exist or no method with matching signature found etc.) that should
     * be transmitted back to consumer side.
     *
     * @constructor
     * @name MethodInvocationException
     *
     * @param {Object}
     *            [settings] the settings object for the constructor call
     * @param {String}
     *            [settings.detailMessage] message containing details
     *            about the error
     * @returns {MethodInvocationException}
     *            The newly created MethodInvocationException object
     */
    function MethodInvocationException(settings) {
        if (!(this instanceof MethodInvocationException)) {
            // in case someone calls constructor without new keyword (e.g. var c
            // = Constructor({..}))
            return new MethodInvocationException(settings);
        }

        var log = LoggerFactory.getLogger("joynr.exceptions.MethodInvocationException");
        var runtimeException = new JoynrRuntimeException(settings);

        /**
         * Used for serialization.
         * @name MethodInvocationException#_typeName
         * @type String
         */
        Util.objectDefineProperty(this, "_typeName", "joynr.exceptions.MethodInvocationException");

        /**
         * The provider version information
         * @name MethodInvocationException#providerVersion
         * @type String
         */
        if (settings) {
            Util.checkProperty(settings, "Object", "settings");
            Util.checkPropertyIfDefined(
                    settings.providerVersion,
                    "Version",
                    "settings.providerVersion");
        }

        Util.extend(this, defaultSettings, settings, runtimeException);
    }

    defaultSettings = {};

    TypeRegistrySingleton.getInstance().addType(
            "joynr.exceptions.MethodInvocationException",
            MethodInvocationException);

    MethodInvocationException.prototype = new Error();
    MethodInvocationException.prototype.constructor = MethodInvocationException;
    MethodInvocationException.prototype.name = "MethodInvocationException";

    return MethodInvocationException;

});
