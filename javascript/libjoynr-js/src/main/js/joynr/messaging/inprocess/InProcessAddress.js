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

define("joynr/messaging/inprocess/InProcessAddress", [], function() {

    /**
     * @constructor
     * @name InProcessAddress
     *
     * @param {InProcessMessagingSkeleton} inProcessMessagingSkeleton the skeleton that should be addressed in process
     */
    function InProcessAddress(inProcessMessagingSkeleton) {

        /**
         * The receive function from the corresponding local messaging receiver
         * @name InProcessAddress#getSkeleton
         * @function
         *
         * @returns {InProcessMessagingSkeleton} the skeleton that should be addressed
         */
        this.getSkeleton = function getSkeleton() {
            return inProcessMessagingSkeleton;
        };
    }

    return InProcessAddress;

});