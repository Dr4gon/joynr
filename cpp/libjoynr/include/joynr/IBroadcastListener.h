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
#ifndef IBROADCASTLISTENER_H
#define IBROADCASTLISTENER_H

#include "joynr/JoynrExport.h"

template <typename T>
class QList;
class QVariant;

template <typename T>
class QSharedPointer;

namespace joynr
{

class IBroadcastFilter;

class JOYNR_EXPORT IBroadcastListener
{
public:
    virtual ~IBroadcastListener()
    {
    }
    virtual void eventOccurred(const QList<QVariant>& values,
                               const QList<QSharedPointer<IBroadcastFilter>>& filters) = 0;
};

} // namespace joynr

#endif // IBROADCASTLISTENER_H
