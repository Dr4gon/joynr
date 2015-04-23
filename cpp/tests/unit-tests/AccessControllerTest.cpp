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

#include "joynr/PrivateCopyAssign.h"
#include "tests/utils/MockObjects.h"
#include "gtest/gtest.h"
#include "cluster-controller/access-control/AccessController.h"
#include "cluster-controller/access-control/LocalDomainAccessStore.h"
#include "joynr/system/DiscoveryEntry.h"

using namespace ::testing;
using namespace joynr;
using namespace joynr::system;
using namespace joynr::infrastructure;

// Mock objects cannot make callbacks themselves but can make calls to methods
// with the same arguments as the mocked method call.
class ConsumerPermissionCallbackMaker
{
public:
    ConsumerPermissionCallbackMaker(Permission::Enum permission) :
        permission(permission)
    {}

    void consumerPermission(
            const QString& userId,
            const QString& domain,
            const QString& interfaceName,
            infrastructure::TrustLevel::Enum trustLevel,
            QSharedPointer<LocalDomainAccessController::IGetConsumerPermissionCallback> callback
    ) {
        Q_UNUSED(userId)
        Q_UNUSED(domain)
        Q_UNUSED(interfaceName)
        Q_UNUSED(trustLevel)
        callback->consumerPermission(permission);
    }

    void operationNeeded(
            const QString& userId,
            const QString& domain,
            const QString& interfaceName,
            infrastructure::TrustLevel::Enum trustLevel,
            QSharedPointer<LocalDomainAccessController::IGetConsumerPermissionCallback> callback
    ) {
        Q_UNUSED(userId)
        Q_UNUSED(domain)
        Q_UNUSED(interfaceName)
        Q_UNUSED(trustLevel)
        callback->operationNeeded();
    }

private:
    Permission::Enum permission;
};


class AccessControllerTest : public ::testing::Test {
public:
    AccessControllerTest() :
        localDomainAccessControllerMock(new LocalDomainAccessStore(
                        true // start with clean database
        )),
        accessControllerCallback(new MockConsumerPermissionCallback()),
        settings(),
        messagingSettingsMock(settings),
        localCapabilitiesDirectoryMock(messagingSettingsMock),
        accessController(
                localCapabilitiesDirectoryMock,
                localDomainAccessControllerMock
        )
    {

    }

    ~AccessControllerTest() {
    }

    void SetUp(){
        request.setMethodName(TEST_OPERATION);
        messagingQos = MessagingQos(5000);
        message = messageFactory.createRequest(fromParticipantId,
                                     toParticipantId,
                                     messagingQos,
                                     request);
        message.setHeaderCreatorUserId(DUMMY_USERID);

        ON_CALL(
                messagingSettingsMock,
                getDiscoveryDirectoriesDomain()
        )
                .WillByDefault(Return("fooDomain"));
        ON_CALL(
                messagingSettingsMock,
                getCapabilitiesDirectoryParticipantId()
        )
                .WillByDefault(Return("fooParticipantId"));

        requestStatus.setCode(joynr::RequestStatusCode::OK);
        discoveryEntry = DiscoveryEntry(
                TEST_DOMAIN,
                TEST_INTERFACE,
                toParticipantId,
                types::ProviderQos(),
                connections
        );
        EXPECT_CALL(
                localCapabilitiesDirectoryMock,
                lookup(_,_,toParticipantId)
        )
                .WillOnce(DoAll(
                          SetArgReferee<0>(requestStatus),
                          SetArgReferee<1>(discoveryEntry)
                ));
    }

    void TearDown(){
    }

protected:
    MockLocalDomainAccessController localDomainAccessControllerMock;
    QSharedPointer<MockConsumerPermissionCallback> accessControllerCallback;
    QSettings settings;
    MockMessagingSettings messagingSettingsMock;
    MockLocalCapabilitiesDirectory localCapabilitiesDirectoryMock;
    AccessController accessController;
    JoynrMessageFactory messageFactory;
    JoynrMessage message;
    Request request;
    RequestStatus requestStatus;
    MessagingQos messagingQos;
    DiscoveryEntry discoveryEntry;
    static const QString fromParticipantId;
    static const QString toParticipantId;
    static const QString replyToChannelId;
    static const QString DUMMY_USERID;
    static const QString TEST_DOMAIN;
    static const QString TEST_INTERFACE;
    static const QString TEST_OPERATION;
    static const QList<CommunicationMiddleware::Enum> connections;
private:
    DISALLOW_COPY_AND_ASSIGN(AccessControllerTest);
};

//----- Constants --------------------------------------------------------------
const QString AccessControllerTest::fromParticipantId("sender");
const QString AccessControllerTest::toParticipantId("receiver");
const QString AccessControllerTest::replyToChannelId("replyToId");
const QString AccessControllerTest::DUMMY_USERID("testUserId");
const QString AccessControllerTest::TEST_DOMAIN("testDomain");
const QString AccessControllerTest::TEST_INTERFACE("testInterface");
const QString AccessControllerTest::TEST_OPERATION("testOperation");
const QList<CommunicationMiddleware::Enum> AccessControllerTest::connections =
        QList<CommunicationMiddleware::Enum>() << CommunicationMiddleware::SOME_IP;

//----- Tests ------------------------------------------------------------------

TEST_F(AccessControllerTest, accessWithInterfaceLevelAccessControl) {
    ConsumerPermissionCallbackMaker makeCallback(Permission::YES);
    EXPECT_CALL(
            localDomainAccessControllerMock,
            getConsumerPermission(DUMMY_USERID, TEST_DOMAIN, TEST_INTERFACE, TrustLevel::HIGH, _)
    )
            .Times(1)
            .WillOnce(Invoke(&makeCallback, &ConsumerPermissionCallbackMaker::consumerPermission));

    EXPECT_CALL(*accessControllerCallback, hasConsumerPermission(true))
            .Times(1);

    accessController.hasConsumerPermission(
            message,
            accessControllerCallback.dynamicCast<IAccessController::IHasConsumerPermissionCallback>()
    );
}

TEST_F(AccessControllerTest, accessWithOperationLevelAccessControl) {
    ConsumerPermissionCallbackMaker makeCallback(Permission::YES);
    EXPECT_CALL(
            localDomainAccessControllerMock,
            getConsumerPermission(DUMMY_USERID, TEST_DOMAIN, TEST_INTERFACE, TrustLevel::HIGH, _)
    )
            .Times(1)
            .WillOnce(Invoke(&makeCallback, &ConsumerPermissionCallbackMaker::operationNeeded));

    Permission::Enum permissionYes = Permission::Enum::YES;
    DefaultValue<Permission::Enum>::Set(permissionYes);
    EXPECT_CALL(
            localDomainAccessControllerMock,
            getConsumerPermission(
                    DUMMY_USERID,
                    TEST_DOMAIN,
                    TEST_INTERFACE,
                    TEST_OPERATION,
                    TrustLevel::HIGH
            )
    )
            .WillOnce(Return(permissionYes));

    EXPECT_CALL(*accessControllerCallback, hasConsumerPermission(true))
            .Times(1);

    accessController.hasConsumerPermission(
            message,
            accessControllerCallback.dynamicCast<IAccessController::IHasConsumerPermissionCallback>()
    );
}

TEST_F(AccessControllerTest, accessWithOperationLevelAccessControlAndFaultyMessage) {
    ConsumerPermissionCallbackMaker makeCallback(Permission::YES);
    EXPECT_CALL(
            localDomainAccessControllerMock,
            getConsumerPermission(DUMMY_USERID, TEST_DOMAIN, TEST_INTERFACE, TrustLevel::HIGH, _)
    )
            .Times(1)
            .WillOnce(Invoke(&makeCallback, &ConsumerPermissionCallbackMaker::operationNeeded));

    EXPECT_CALL(*accessControllerCallback, hasConsumerPermission(false))
            .Times(1);

    QString payload("invalid serialization of Request object");
    QByteArray buff;
    buff.append(payload);
    message.setPayload(buff);

    accessController.hasConsumerPermission(
            message,
            accessControllerCallback.dynamicCast<IAccessController::IHasConsumerPermissionCallback>()
    );
}

