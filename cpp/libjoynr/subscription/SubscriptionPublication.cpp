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

#include "joynr/SubscriptionPublication.h"

namespace joynr
{

const SubscriptionPublication SubscriptionPublication::NULL_RESPONSE = SubscriptionPublication();

SubscriptionPublication::SubscriptionPublication() : subscriptionId(), error()
{
}

SubscriptionPublication::SubscriptionPublication(BaseReply&& baseReply)
        : BaseReply::BaseReply(std::move(baseReply)), subscriptionId(), error()
{
}

std::string SubscriptionPublication::getSubscriptionId() const
{
    return subscriptionId;
}

void SubscriptionPublication::setSubscriptionId(const std::string& subscriptionId)
{
    this->subscriptionId = subscriptionId;
}

bool SubscriptionPublication::operator==(const SubscriptionPublication& other) const
{
    // if error ptr do not point to the same object
    if (error != other.getError()) {
        // if exactly one of error and other.getError() is a nullptr
        if (error == nullptr || other.getError() == nullptr) {
            return false;
        }
        // compare actual objects
        if (!(*error.get() == *other.getError().get())) {
            return false;
        }
    }

    return subscriptionId == other.getSubscriptionId() && BaseReply::operator==(other);
}

bool SubscriptionPublication::operator!=(const SubscriptionPublication& other) const
{
    return !(*this == other);
}

std::shared_ptr<exceptions::JoynrRuntimeException> SubscriptionPublication::getError() const
{
    return error;
}

void SubscriptionPublication::setError(std::shared_ptr<exceptions::JoynrRuntimeException> error)
{
    this->error = std::move(error);
}

// printing SubscriptionPublication with google-test and google-mock
void PrintTo(const SubscriptionPublication& subscriptionPublication, ::std::ostream* os)
{
    *os << "SubscriptionPublication{";
    *os << "subscriptionId:" << subscriptionPublication.subscriptionId;
    *os << ", ";
    *os << "error:" << subscriptionPublication.error ? "null"
                                                     : subscriptionPublication.error->getMessage();
    *os << ", SKIPPED printing BaseReply";
    *os << "}";
}

} // namespace joynr
