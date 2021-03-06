# Joynr Java Developer Guide

## Conversion of Franca entries

### Place holders

Note that the following elements in the code examples below must be replaced by actual values from Franca:

```
// "<Attribute>" the Franca name of the attribute
// "<AttributeType>" the Franca name of the attribute type
// "<broadcast>" the Franca name of the broadcast, starting with a lowercase letter
// "<Broadcast>" the Franca name of the broadcast, starting with capital letter
// "BroadcastFilter<Attribute>" Attribute is the Franca attributes name
// "<Filter>" the Franca name of the broadcast filter
// "<interface>" the Franca interface name, starting with a lowercase letter
// "<Interface>" the Franca interface name, starting with capital letter
// "<method>" the Franca method name, starting with a lowercase letter
// "<Method>" the Franca method name, starting with capital letter
// "<OutputType>" the Franca broadcast output type name
// "<Package>" the Franca package name
// "<ProviderDomain>" the provider domain name used by provider and client
// "<ReturnType>" the Franca return type name
```

### Package name
The Franca ```<Package>``` will be transformed to the Java package ```joynr.<Package>```.

### Type collection name
The Franca ```<TypeCollection>``` will be transformed to the Java package ```joynr.<Package>.<TypeCollection>```.

### Complex type name

Any Franca complex type ```<TypeCollection>.<Type>``` will result in the creation of a ```class joynr.<Package>.<TypeCollection>.<Type>``` (see above).

The same ```<Type>``` will be used for all elements in the event that this type is used as an element of other complex types, as a method input or output argument, or as a broadcast output argument.

Getter and Setter methods will be created for any element of a struct type. Also a standard constructor, full arguments constructor and object argument constructor will be created automatically.

### Interface name

The Franca ```<Interface>``` will be used as a prefix to create the following Java classes or interfaces:

```java
public abstract class joynr.<Package>.<Interface>AbstractProvider
public class joynr.<Package>.Default<Interface>Provider
public interface joynr.<Package>.<Interface>Async
public interface joynr.<Package>.<Interface>BroadcastInterface
public interface joynr.<Package>.<Interface>FireAndForget
public interface joynr.<Package>.<Interface>
public abstract class joynr.<Package>.<Interface><Broadcast>BroadcastFilter
public interface joynr.<Package>.<Interface>Provider
public interface joynr.<Package>.<Interface>Proxy
public interface joynr.<Package>.<Interface>SubscriptionInterface
public interface joynr.<Package>.<Interface>SubscriptionPublisher
public class joynr.<Package>.<Interface>SubscriptionPublisherImpl
public interface joynr.<Package>.<Interface>Sync
```
# Setting up a joynr deployment
Choose how you want your application to connect to the joynr network by initializing the
JoynrRuntime with the appropriate joynr RuntimeModule, which uses guice to inject the desired
functionality.

If you have multiple nodes running locally, they can share a cluster controller, which handles
access control, local discovery and message routing for the local cluster. joynr currently supports
connecting Java nodes to a cluster controller via WebSockets. The individual applications are
configured using a ```LibjoynrWebSocketRuntimeModule```.

For a single node deployment, it may however be simpler to combine the cluster controller logic and
the application in a single Java process. Use a ```CCInProcessRuntimeModule``` in this case.

See the Radio example, in particular ```MyRadioConsumerApplication``` and
```MyRadioProviderApplication```, for a detailed example of how this is done.

See the [Java Configuration Reference](JavaSettings.md) for a complete listing of all available
configuration properties available to use in joynr Java applications.

## The external (global) transport middlewares
joynr is able to communicate to other clusters via HTTP using Atmosphere, or MQTT using Eclipe Paho,
both of which can be in operation at the same time. Guice is also used to inject the required
functionality.

After choosing which RuntimeModule you are using, override it with the
```AtmosphereMessagingModule``` and the ```MqttPahoModule```. See the Radio example, in particular
```MyRadioConsumerApplication``` and ```MyRadioProviderApplication``` for a detailed example of how
this is done.

If using more than one global transport middleware, PROPERTY_MESSAGING_PRIMARYGLOBALTRANSPORT (see
[Java Configuration Reference](JavaSettings.md)) has to be set to select the transport middleware
which is used to register providers. Providers will be reachable via the selected global transport
middleware.

# Building a Java consumer application

A java joynr application inherits from ```AbstractJoynrApplication``` class and contains at least a
```main()```, ```run()``` and ```shutdown()``` method.

## Required imports

The following base imports are required for a Java Consumer application:

```java
import io.joynr.arbitration.ArbitrationStrategy;
import io.joynr.arbitration.DiscoveryQos;
import io.joynr.arbitration.DiscoveryScope;
import io.joynr.exceptions.ApplicationException;
import io.joynr.exceptions.DiscoveryException;
import io.joynr.exceptions.JoynrCommunicationException;
import io.joynr.exceptions.JoynrRuntimeException;
import io.joynr.messaging.AtmosphereMessagingModule;
import io.joynr.messaging.MessagingPropertyKeys;
import io.joynr.messaging.MessagingQos;
import io.joynr.proxy.Callback;
import io.joynr.proxy.Future;
import io.joynr.proxy.ProxyBuilder;
import io.joynr.runtime.AbstractJoynrApplication;
import io.joynr.runtime.CCInProcessRuntimeModule;
import io.joynr.runtime.JoynrApplication;
import io.joynr.runtime.JoynrApplicationModule;
import io.joynr.runtime.JoynrInjectorFactory;
import java.io.IOException;
import java.util.Properties;
import joynr.<Package>.<Interface>Proxy;
import com.google.inject.Inject;
import com.google.inject.Module;
import com.google.inject.name.Named;
import com.google.inject.util.Modules;
```

## The base class

```java
// required imports
...
public class MyConsumerApplication extends AbstractJoynrApplication {

    @Inject
    @Named(APP_CONFIG_PROVIDER_DOMAIN)
    private String providerDomain;

    private <Interface>Proxy <interface>Proxy;

    public static void main(String[] args) throws IOException {
        // initialization, perhaps including setting the domain
        // for this instance of the application.
    }

    public void run() {
        // main application logic
    }

    public void shutdown() {
       // unregister and cleanup
    }
}
```

## The main method

The ```main()``` method must setup the configuration (provider domain etc.) and create the
```JoynrApplication``` instance by instantiating a new ```JoynrApplicationModule```. Then the
```run()``` method of the consumer application can be called to do the work.

As a prerequisite, the **provider** and **consumer domain** need to be defined using ```Properties```
as shown below.

```java
public static void main(String[] args) throws IOException {
    String providerDomain = "<ProviderDomain>";

    Properties joynrConfig = new Properties();
    joynrConfig.setProperty(MessagingPropertyKeys.PERSISTENCE_FILE, STATIC_PERSISTENCE_FILE);
    joynrConfig.setProperty(PROPERTY_JOYNR_DOMAIN_LOCAL, "my_local_domain");

    Properties appConfig = new Properties();
    appConfig.setProperty(APP_CONFIG_PROVIDER_DOMAIN, providerDomain);

    Module runtimeModule = Modules.override(new CCInProcessRuntimeModule()).with(new AtmosphereMessagingModule());

    JoynrApplication myConsumerApp =
      new JoynrInjectorFactory(joynrConfig, runtimeModule).createApplication(
        new JoynrApplicationModule(MyApplication.class, appConfig));

    myConsumerApp.run();
    myConsumerApp.shutdown();
}
```
## The discovery quality of service

The class ```DiscoveryQos``` configures how the search for a provider will be handled. It has the following members:

* **discoveryTimeoutMs**  Timeout for discovery process (milliseconds), afterwards triggers DiscoveryException
* **cacheMaxAgeMs** Defines the maximum allowed age of cached entries (milliseconds), only younger entries will be considered. If no suitable providers are found, then depending on the discoveryScope, a remote global lookup may be triggered.
* **arbitrationStrategy** The arbitration strategy (see below)
* **customParameters** special parameters, that must match, e.g. keyword (see below)
* **retryIntervalMs** The time to wait between discovery retries after encountering a discovery error.
* **discoveryScope** default: LOCAL_AND_GLOBAL (details see below)

The **discoveryScope** defines, whether a suitable provider will be searched only in the local capabilities directory or also in the global one.

Available values are as follows:

* **LOCAL_ONLY** Only entries from local capability directory will be searched
* **LOCAL_THEN_GLOBAL** Entries will be taken from local capabilities directory, unless no such entries exist, in which case global entries will be looked at as well.
* **LOCAL_AND_GLOBAL** Entries will be taken from local capabilities directory and from global capabilities directory.
* **GLOBAL_ONLY** Only the global entries will be looked at.

Whenever global entries are involved, they are first searched in the local cache. In case no global entries are found in the cache, a remote lookup is triggered.

The enumeration ```ArbitrationStrategy``` defines special options to select a Provider:
* **LastSeen** The participant that was last refreshed (i.e. with the most current last seen date)
will be selected
* **NotSet** (not allowed in the app, otherwise arbitration will throw DiscoveryException)
* **HighestPriority** Entries will be considered according to priority
* **Keyword** Only entries that have a matching keyword will be considered
* **FixedChannel** select provider which matches the participantId provided as custom parameter in
   DiscoveryQos, if existing
* **Custom** Allows you to provide a `ArbitrationStrategyFunction` to allow custom
selection of discovered entries

**Default arbitration strategy:** LastSeen

The priority is set by the provider through the call ```providerQos.setPriority()```.

Class ```ArbitrationConstants``` provides keys for the key-value pair for the custom Parameters of discoveryScope:

* **PRIORITY_PARAMETER** (apparently not implemented as of now)
* **KEYWORD_PARAMETER**
* **FIXEDPARTICIPANT_KEYWORD**

Example for **Keyword** arbitration strategy:

```java
discoveryQos.addCustomParameter(ArbitrationConstants.KEYWORD_PARAMETER, "keyword");
```
Example for **FixedChannel** arbitration strategy:

```java
discoveryQos.addCustomParameter(ArbitrationConstants.FIXED_PARTICIPANT_KEYWORD, participantId);
```

Example for the creation of a DiscoveryQos class object:

```java
DiscoveryQos discoveryQos = new DiscoveryQos();

discoveryQos.setDiscoveryTimeoutMs(10000); // optional, default 30000
discoveryQos.setCacheMaxAgeMs(Long.MAX_VALUE); // optional, default 0
discoveryQos.setArbitrationStrategy(ArbitrationStrategy.HighestPriority); // default HP
discoveryQos.addCustomParameter(key, value); // optional, default none
discoveryQos.setProviderMustSupportOnChange(true); // optional, default false
discoveryQos.setRetryIntervalMs(1000); // optional, default 1000
discoveryQos.setDiscoveryScope(DiscoveryScope.LOCAL_AND_GLOBAL); // optional, default as stated
```

## The message quality of service

The ```MesssagingQos``` class defines the roundtrip timeout for RPC requests in milliseconds
and allows definition of additional custom message headers.

If no specific setting is given, the default roundtrip timeout is 60 seconds.
The keys of custom message headers may contain ascii alphanumeric or hyphen.
The values of custom message headers may contain alphanumeric, space, semi-colon, colon,
comma, plus, ampersand, question mark, hyphen, dot, star, forward slash and back slash.
If a key or value is invalid, the API method called to introduce the custom message
header throws an IllegalArgumentException.

Example:

```java
long ttl_ms = 60000;

MessagingQos messagingQos = new MessagingQos(ttl_ms);
// optional custom headers
Map<String, String> customMessageHeaders = new Map<String, String>();
customMessageHeaders.put("key1", "value1");
...
customMessageHeaders.put("keyN", "valueN");
messagingQos.putAllCustomMessageHeaders(customMessageHeaders);
...
messagingQos.putCustomMessageHeader("anotherKey", "anotherValue");
```

## The run method

Inside the ```run()``` method, the consumer application instance must create one **proxy** per used Franca interface in order to be able to
* call its **methods** (RPC) either **synchronously** or **asynchronously**
* **subscribe** or **unsubscribe** to its **attributes** or **update** a subscription
* **subscribe** or **unsubscribe** to its **broadcasts** or **update** a subscription

In case no suitable provider can be found during discovery, a ```DiscoveryException``` is thrown.
In case of communication errors, a ```JoynrCommunicationException``` is thrown.

```java
@Override
public void run() {
    DiscoveryQos discoveryQos = new DiscoveryQos();
    // the qos can be fine tuned here by calling setters

    ProxyBuilder<<Interface>Proxy> proxyBuilder =
        runtime.getProxyBuilder(providerDomain, <Interface>Proxy.class);
    try {
        // Also can call proxyBuilder.build(callback)
        <interface>Proxy = proxyBuilder.
            setMessagingQos(new MessagingQos()).
            setDiscoveryQos(discoveryQos).
            build();
        // call methods, subscribe to broadcasts etc.
        // enter some event loop
    } catch (DiscoveryException e) {
        // no provider found
    } catch (JoynrCommunicationException e) {
        // could not send message
    }
}
```
A callback can also be added to the proxyBuilder.build() call, allowing application code to be
notified when the discovery process has completed.

It is also possible to obtain a proxy for targeting multiple providers. You can either do
this by specifying a set of domains, by providing a custom `ArbitrationStrategyFunction`
(see The discovery quality of service above) or a combination of the two.
When you create such a multi-proxy, a call to a method on that proxy will result in 'n'
calls to the providers, where 'n' is the number of providers targeted.  
It is only possible to send calls to multiple providers if the methods are fire-and-forget.
Attempts to make calls to non-fire-and-forget methods from a multi-proxy will result in an
exception being thrown.

So, for example, if we change the code above to target two domains, we get:

```java
...
	Set<String> domains = new HashSet<>();
	domains.add(providerDomainOne);
	domains.add(providerDomainTwo);
	ProxyBuilder<<Interface>Proxy> proxyBuilder =
		runtime.getProxyBuilder(domains, <Interface>Proxy.class);
...
```

## Synchronous Remote procedure calls
While the provider executes the call asynchronously in any case, the consumer will wait until the call is finished, i.e. the thread will be blocked.
Note that the message order on Joynr RPCs will not be preserved.

Example for calls with single return parameter:

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public void run() {
    // setup proxy named <interface>Proxy
    ...
    try {
        <ReturnType> retval;
        retval = <interface>Proxy.<method>([inputVal1, ..., inputValN]);
    } catch (ApplicationException e) {
        // optional special error handling in case model contains error enumeration
    } catch (JoynrRuntimeException e) {
        // error handling
    }
}
```

In case of multiple return parameters the parameters will be wrapped into a class named
```<Method>Returned```. Each parameter value is available through a public member variable inside this class.

Example:
```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public void run() {
    // setup proxy named <interface>Proxy
    ...
    try {
        <Method>Returned retval;
        retval = <interface>Proxy.<method>([inputVal1, ..., inputValN]);
        // handle return parameters
        //   retval.<returnParameter1>
        //   ...
        //   retval.<returnParameterN>
    } catch (ApplicationException e) {
        // optional special error handling in case model contains error enumeration
    } catch (JoynrRuntimeException e) {
        // error handling
    }
}
```

For methods which are modelled with error enumerations, additionally, ApplicationExceptions have to be caught.
The ApplicationException serves as container for the actual error enumeration which can be retrieved by calling e.getError().


## Asynchronous Remote Procedure calls
Using asynchronous method calls allows the current thread to continue its work. For this purpose a callback has to be provided for the API call in order to receive the result and error respectively. Note the current thread will still be blocked until the Joynr message is internally set up and serialized. It will then be enqueued and handled by a Joynr Middleware thread.
The message order on Joynr RPCs will not be preserved.
If no return type exists, the term ```Void``` is used instead.

### Asynchronous Remote Procedure calls with single return parameter
Example for calls with single return parameter:

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public class MyCallback implements Callback<<ReturnType>> {
    @Override
    void onSuccess(<ReturnType> result) {
        // handle result
    }

    @Override
    void onFailure(JoynrRuntimeException error) {
        // handle error
    }
}
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    public Future<<ReturnType>> future;
    MyCallback myCallback = new MyCallback();

    future = <interface>Proxy.<method>(
        myCallback,
        [inputVal1, ..., inputValN]
    );
    try {
        long timeoutInMilliseconds;
        // set timeout value here
        <ReturnType> result = future.get(timeOutInMilliseconds);
    } catch (InterruptedException|JoynrRuntimeException e) {
        // handle error
    }
    ...
}
```
If the Franca model includes error enums, then the Callback will also need to implement onFailure for the modeled error:

```java
@Override
public void onFailure(<Method>ErrorEnum errorEnum) {
    switch (errorEnum) {
    case <ENUM_LITERAL_A>:
        break;
    case <ENUM_LITERAL_B>:
        break;
    default:
    // handle default error case
        break;
    }

}
```

### Async Remote Procedure calls with Multiple Return Parameters

In case of multiple return parameters the parameters will be wrapped into a class named
```<Method>Returned```. Each parameter value is available through a public member variable inside this class.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection.<Type>;
...
public class MyCallback implements Callback<<Method>Returned> {
    void onSuccess(<Method>Returned result) {
        // handle result
    }

    void onFailure(JoynrRuntimeException error) {
        // handle error
    }
}
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    public Future<<Method>Returned> future;
    MyCallback myCallback = new MyCallback();

    future = <interface>Proxy.<method>(
        myCallback,
        [inputVal1, ..., inputValN]
    );
    try {
        long timeoutInMilliseconds;
        // set timeout value here
        <Method>Returned result = future.get(timeOutInMilliseconds);
        // handle return parameters
        //   result.<returnParameter1>
        //   ...
        //   result.<returnParameterN>
    } catch (InterruptedException|JoynrRuntimeException e) {
        // handle error
    }
    ...
}
```

If the Franca model includes error enums, then the Callback will also need to implement onFailure for the modeled error:

```java
@Override
public void onFailure(<Method>ErrorEnum errorEnum) {
    switch (errorEnum) {
    case <ENUM_LITERAL_A>:
        break;
    case <ENUM_LITERAL_B>:
        break;
    default:
		 // handle default error case
        break;
    }

}
```

## Quality of Service settings for subscriptions

### SubscriptionQos

The abstract class ```SubscriptionQos``` has the following members:

* **expiryDateMs** Absolute time until notifications will be send (milliseconds)
* **publicationTtlMs** Lifespan of a notification (milliseconds), it will be deleted afterwards

### PeriodicSubscriptionQos

The class ```PeriodicSubscriptionQos``` inherits from ```SubscriptionQos``` and has the following additional members:

* **periodMs** defines how long to wait before sending an update even if the value did not change
* **alertAfterIntervalMs** Timeout for notifications, afterwards a missed publication notification will be raised (milliseconds)

This class can be used for subscriptions to attributes.

Note that updates will be send only based on the specified interval and not based on changes of the attribute.

### OnChangeSubscriptionQos

The class ```OnChangeSubscriptionQos``` inherits from ```SubscriptionQos``` and has the following additional members:

* **minIntervalMs** Minimum time to wait between successive notifications (milliseconds)

This class should be used for subscriptions to broadcasts. It can also be used for subscriptions
to attributes if no periodic update is required.

### OnchangeWithKeepAliveSubscriptionQos

The class ```OnChangeWithKeepAliveSubscriptionQos``` inherits from ```OnChangeSubscriptionQos``` and has the following additional members:

* **maxIntervalMs** Maximum time to wait between notifications, if value has not changed
* **alertAfterIntervalMs** Timeout for notifications, afterwards a missed publication notification will be raised (milliseconds)

This class can be used for subscriptions to attributes. Updates will then be sent based both periodically and after a change (i.e. this acts like a combination of ```PeriodicSubscriptionQos``` and ```OnChangeSubscriptionQos```).

Using it for subscriptions to broadcasts is theoretically possible because of inheritance but makes no sense (in this case the additional members will be ignored).

## Subscribing to an attribute

Attribute subscription - depending on the subscription quality of service settings used - informs
the application either periodically and / or on change of an attribute about the current value.

The **subscriptionId** can be retrieved via the callback (onSubscribed) and via the future returned
by the subscribeTo call. It can be used later to update the subscription or to unsubscribe from it.
The subscriptionId will be available when the subscription is successfully registered at the
provider. If the subscription failed, a SubscriptionException will be returned via the callback
(onError) and thrown by future.get().

To receive the subscription, a **callback** has to be provided which is done providing a listener
class as outlined below. Since the callback is called by a communication middleware thread, it
should not be blocked, wait for user interaction, or do larger computation. The callback methods
(onReceive, onSubscribed, onError) are optional. Only the required methods have to be implemented.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
import joynr.OnChangeWithKeepAliveSubscriptionQos;
import io.joynr.pubsub.subscription.AttributeSubscriptionAdapter;
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    // Quality of service settings can be provided as either
    // "PeriodicSubscriptionQos",
    // "OnChangeSubscriptionQos" or "OnChangeWithKeepAliveSubscriptionQos" class
    // referenced by <QosClass> below

    <QosClass> qos = new <QosClass>(... parameters ...);
    ...
    Future<String> subscriptionIdFuture;
    try {
        subscriptionIdFuture = <interface>Proxy.subscribeTo<Attribute>(
            new AttributeSubscriptionAdapter<AttributeType>() {
                // Gets called on every received publication
                @Override
                public void onReceive(<AttributeType> value) {
                    // handle info
                }

                // Gets called when the subscription is successfully registered at the provider
                @Override
                public void onSubscribed(String subscriptionId) {
                    // save the subscriptionId for updating the subscription or unsubscribing from it
                    // the subscriptionId can also be taken from the future returned by the subscribeTo call
                }

                // Gets called on every error that is detected on the subscription
                @Override
                public void onError(JoynrRuntimeException e) {
                    // handle subscription error, e.g.:
                    // - SubscriptionException if the subscription registration failed at the provider
                    // - PublicationMissedException if a periodic subscription publication does not
                    //   arrive in time

                }
            },
            qos
            );
    } catch (JoynrRuntimeException e) {
        // handle error
    }
    ...
    // get the subscriptionId from the Future when needed
    String subscriptionId;
    if (subscriptionIdFuture != null) {
        try {
            subscriptionId = subscriptionIdFuture.get();
        } catch (JoynrRuntimeException | InterruptedException | ApplicationException e) {
            // handle error
        }
    }
}
```

## Updating an attribute subscription

The subscribeTo method can also be used to update an existing subscription, when the **subscriptionId** is given as additional parameter as follows:

```java
    try {
        subscriptionIdFuture = <interface>Proxy.subscribeTo<Attribute>(
            new AttributeSubscriptionAdapter<AttributeType>() {
                // Gets called on every received publication
                @Override
                public void onReceive(<AttributeType> value) {
                    // handle info
                }

                // Gets called when the subscription is successfully updated at the provider
                @Override
                public void onSubscribed(String subscriptionId) {
                    // save the subscriptionId for updating the subscription or unsubscribing from it
                    // the subscriptionId can also be taken from the future returned by the subscribeTo call
                }

                // Gets called on every error that is detected on the subscription
                @Override
                public void onError(JoynrRuntimeException e) {
                    // handle subscription error, e.g.:
                    // - SubscriptionException if the subscription registration failed at the provider
                    // - PublicationMissedException if a periodic subscription publication does not
                    //   arrive in time
                }
            },
            qos,
            subscriptionId
        );
    } catch (JoynrRuntimeException e) {
        // handle error
    }
```

## Unsubscribing from an attribute

Unsubscribing from an attribute subscription also requires the **subscriptionId** returned by the ealier subscribeTo call.

```java
// for required imports see subscription info
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    private String subscriptionId;
    ...
    try {
        // subscriptionId must have been assigned by previous call
        ...
        <interface>Proxy.unsubscribeFrom<Attribute>(subscriptionId);
    } catch (JoynrRuntimeException e) {
        // handle error
    }
}
```

## Subscribing to a broadcast unconditionally

Broadcast subscription informs the application in case a broadcast is fired from provider side and
returns the output values via callback.

The **subscriptionId** can be retrieved via the callback (onSubscribed) and via the future returned
by the subscribeTo call. It can be used later to update the subscription or to unsubscribe from it.
The subscriptionId will be available when the subscription is successfully registered at the
provider. If the subscription failed, a SubscriptionException will be returned via the callback
(onError) and thrown by future.get().

To receive the subscription, a **callback** has to be provided which is done providing a listener
class as outlined below. Since the callback is called by a communication middleware thread, it
should not be blocked, wait for user interaction, or do larger computation. The callback methods
(onReceive, onSubscribed, onError) are optional. Only the required methods have to be implemented.

```java
import joynr.OnChangeSubscriptionQos;
...
// for any Franca broadcast named "<Broadcast>" used
import joynr.<Package>.<Interface>BroadcastInterface.<Broadcast>BroadcastAdapter;
...
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    private Future<String> subscriptionIdFuture;
    ...
    try {
        int minIntervalMs;
        long expiryDateMs;
        int publicationTtlMs;
        ...
        // provide values for minIntervalMs, expiryDateMs, publicationTtlMs here
        ...
        OnChangeSubscriptionQos qos =
            new OnChangeSubscriptionQos(minIntervalMs, expiryDateMs, publicationTtlMs);
        ...
        subscriptionIdFuture = <interface>Proxy.subscribeTo<Broadcast>Broadcast(
            new <Broadcast>BroadcastAdapter() {
                // Gets called on every received publication
                @Override
                public void onReceive(<AttributeType> value) {
                    // handle broadcast info
                }

                // Gets called when the subscription is successfully registered at the provider
                @Override
                public void onSubscribed(String subscriptionId) {
                    // save the subscriptionId for updating the subscription or unsubscribing from it
                    // the subscriptionId can also be taken from the future returned by the subscribeTo call
                }

                // Gets called on every error that is detected on the subscription
                @Override
                public void onError(JoynrRuntimeException e) {
                    // handle error
                }
            },
            qos
        );
        ...
    } catch (DiscoveryException e) {
        // handle error
    } catch (JoynrCommunicationExceptin e) {
        // handle error
    }
    ...
    // get the subscriptionId from the Future when needed
    String subscriptionId;
    if (subscriptionIdFuture != null) {
        try {
            subscriptionId = subscriptionIdFuture.get();
        } catch (JoynrRuntimeException | InterruptedException | ApplicationException e) {
            // handle error
        }
    }
}
```

## Updating an unconditional broadcast subscription

The subscribeTo method can also be used to update an existing subscription, when the **subscriptionId** is given as additional parameter as follows:

```java
subscriptionIdFuture = <interface>Proxy.subscribeTo<Broadcast>Broadcast(
    new <Broadcast>BroadcastAdapter() {
        // Gets called on every received publication
        @Override
        public void onReceive(<AttributeType> value) {
            // handle broadcast info
        }

        // Gets called when the subscription is successfully updated at the provider
        @Override
        public void onSubscribed(String subscriptionId) {
            // save the subscriptionId for updating the subscription or unsubscribing from it
            // the subscriptionId can also be taken from the future returned by the subscribeTo call
        }

        // Gets called on every error that is detected on the subscription
        @Override
        public void onError(JoynrRuntimeException e) {
            // handle error
        }
    },
    qos,
    subscriptionId
);
```

## Subscribing to a broadcast with filter parameters

Selective Broadcasts use filter logic implemented by the provider and filter parameters set by the
consumer to send only those broadcasts from the provider to the consumer that pass the filter. The
broadcast output values are passed to the consumer via callback.

The **subscriptionId** can be retrieved via the callback (onSubscribed) and via the future returned
by the subscribeTo call (see section
[Subscribing to a broadcast unconditionally](#subscribing-to-a-broadcast-unconditionally)).

To receive the subscription, a **callback** has to be provided (cf. section
[Subscribing to a broadcast unconditionally](#subscribing-to-a-broadcast-unconditionally)).

In addition to the normal broadcast subscription, the filter parameters for this broadcast must be
created and initialized as additional parameters to the ```subscribeTo``` method. These filter
parameters are used to receive only those broadcasts matching the provided filter criteria.

```java
import joynr.OnChangeSubscriptionQos;
...
// for any Franca broadcast named "<Broadcast>" used
import joynr.<Package>.<Interface>BroadcastInterface.<Broadcast>BroadcastAdapter;
import joynr.<Package>.<Interface>BroadcastInterface.<Broadcast>FilterParameters;
...
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    private Future<String> subscriptionIdFuture;
    ...
    try {
        int minIntervalMs;
        long expiryDateMs;
        int publicationTtlMs;
        ...
        // provide values for minIntervalMs, expiryDateMs, publicationTtlMs here
        ...
        OnChangeSubscriptionQos qos =
            new OnChangeSubscriptionQos(minIntervalMs, expiryDateMs, publicationTtlMs);

        <Broadcast>FilterParameters filter = new <Broadcast>FilterParameters();
        // foreach BroadcastFilterAttribute of that filter
        filter.setBroadcastFilter<Attribute>(value);
        ...
        subscriptionIdFuture = <interface>Proxy.subscribeTo<Broadcast>Broadcast(
            new <Broadcast>BroadcastAdapter() {
                // Gets called on every received publication
                @Override
                public void onReceive(<AttributeType> value) {
                    // handle broadcast info
                }

                // Gets called when the subscription is successfully registered at the provider
                @Override
                public void onSubscribed(String subscriptionId) {
                    // save the subscriptionId for updating the subscription or unsubscribing from it
                    // the subscriptionId can also be taken from the future returned by the subscribeTo call
                }

                // Gets called on every error that is detected on the subscription
                @Override
                public void onError(JoynrRuntimeException e) {
                    // handle error
                }
            },
            qos,
            filter
        );
        ...
    } catch (DiscoveryException e) {
        // handle error
    } catch (JoynrCommunicationExceptin e) {
        // handle error
    }
    ...
    // to retrieve the subscriptionId, please refer to section "subscribing to a broadcast unconditionally"
}
```

## Updating a broadcast subscription with filter parameters

The subscribeTo method can also be used to update an existing subscription, when the
**subscriptionId** is given as additional parameter as follows:

```java
        subscriptionId = <interface>Proxy.subscribeTo<Broadcast>Broadcast(
            new <Broadcast>BroadcastAdapter() {
                // Gets called on every received publication
                @Override
                public void onReceive(... OutputParameters ...) {
                    // handle broadcast info
                }

                // Gets called when the subscription is successfully updated at the provider
                @Override
                public void onSubscribed(String subscriptionId) {
                    // save the subscriptionId for updating the subscription or unsubscribing from it
                    // the subscriptionId can also be taken from the future returned by the subscribeTo call
                }

                // Gets called on every error that is detected on the subscription
                @Override
                public void onError(JoynrRuntimeException e) {
                    // handle error
                }
            },
            qos,
            filter,
            subscriptionId
        );
```

## Unsubscribing from a broadcast

Unsubscribing from a broadcast subscription requires the **subscriptionId** returned by the earlier
subscribe call.

```java
public void run() {
    // setup proxy named "<interface>Proxy"
    ...
    private String subscriptionId;
    ...
    try {
        <interface>Proxy.unsubscribeFrom<Broadcast>Broadcast(subscriptionId);
        ...
    } catch (DiscoveryException e) {
        // handle error
    } catch (JoynrCommunicationExceptin e) {
        // handle error
    }
    ...
}
```

## The shutdown method
The shutdown method should be called on exit of the application. Inside the ```shutdown()```
method , the consumer should unsubscribe from any attributes and broadcasts it was subscribed to and
terminate the instance.

```java
@Override
public void shutdown() {
    // for all proxies
    if (<interface>Proxy != null) {
         if (subscribed) {
             // unsubscribe from attributes
             // unsubscribe from broadcasts
        }
    }

    runtime.shutdown(true);
    try {
         Thread.sleep(3000);
    } catch (InterruptedException e) {
          // handle error
    }
    System.exit(0);
}
```

# Building a Java Provider application

The Java Provider mainly consists of the following classes:

* A  generic **Provider Application Class**
* One **Provider Class** for each Franca interface to be supported

## The MyProviderApplication class

The provider application class is used to register a provider class for each Franca interface to be
supported.

### Required Imports

```java
import io.joynr.accesscontrol.StaticDomainAccessControlProvisioning;
import io.joynr.accesscontrol.StaticDomainAccessControlProvisioningModule;
import io.joynr.exceptions.JoynrRuntimeException;
import io.joynr.messaging.MessagingPropertyKeys;
import io.joynr.runtime.AbstractJoynrApplication;
import io.joynr.runtime.JoynrApplication;
import io.joynr.runtime.JoynrApplicationModule;
import io.joynr.runtime.JoynrInjectorFactory;
import java.io.IOException;
import java.util.Properties;
import com.google.inject.Inject;
import com.google.inject.Module;
import edu.umd.cs.findbugs.annotations.SuppressWarnings;
import com.fasterxml.jackson.databind.ObjectMapper;
import com.fasterxml.jackson.databind.ObjectMapper.DefaultTyping;
```

### The base class
The class must extend ```AbstractJoynrApplication``` and can theoretically serve multiple Franca
interfaces.

For each Franca interface implemented, the providing application creates an instance of
```My<Interface>Provider```, which implements the service for that particular interface, and
registers it as a provider at the Joynr Middleware.

The example below shows the code for one interface:

```java
package myPackage;
...
// required imports
...
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public class MyProviderApplication extends AbstractJoynrApplication {
    private static final String AUTH_TOKEN = "MyProvider_authToken";
    public static final String STATIC_PERSISTENCE_FILE = "provider-joynr.properties";
    private My<Interface>Provider <interface>provider = null;

    public static void main(String[] args) {
        // ...
    }

    @Override
    public void run() {
        // ...
    }

    @Override
    public void shutdown() {
        // ...
    }
}

```

### The main method

```java

public static void main(String[] args) {
    String localDomain = "<ProviderDomain>";
    Properties joynrConfig = new Properties();
    joynrConfig.setProperty(MessagingPropertyKeys.PERSISTENCE_FILE, STATIC_PERSISTENCE_FILE);
    joynrConfig.setProperty(PROPERTY_JOYNR_DOMAIN_LOCAL, localDomain);
    Properties appConfig = new Properties();
    provisionAccessControl(joynrConfig, localDomain);
    Module runtimeModule = Modules.override(new CCInProcessRuntimeModule()).with(new AtmosphereMessagingModule());
    JoynrApplication joynrApplication =
        new JoynrInjectorFactory(joynrConfig,
            runtimeModule,
            new StaticDomainAccessControlProvisioningModule()).createApplication(
            new JoynrApplicationModule(MyProviderApplication.class, appConfig)
        );
    joynrApplication.run();
    joynrApplication.shutdown();
}
```

### The run method
The run method registers the interface specific provider class instance. From that
time on, the provider will be reachable from outside and react on incoming requests (e.g. method
RPC etc.). It can be found by consumers through Discovery.
Any specific broadcast filters must be added prior to registry.

```java
@Override
public void run() {
    <interface>provider = new My<Interface>Provider();

    // for any filter of a broadcast with filter
    <interface>provider.addBroadcastFilter(new <Filter>BroadcastFilter());
    ProviderQos providerQos = new ProviderQos();
    // use setters on providerQos as required
    // set the priority, used for (default) arbitration by highest priority
    long priorityValue;
    // set priorityValue
    providerQos.setPriority(priorityValue);

    runtime.registerProvider(localDomain, <interface>provider, providerQos);

    // loop here
}
```

### The shutdown method
The ```shutdown``` method should be called on exit of the application. It should cleanly unregister
any providers the application had registered earlier.

```java
@Override
@SuppressWarnings(value = "DM_EXIT", justification = "WORKAROUND to be removed")
public void shutdown() {
    if (<interface>provider != null) {
        try {
            runtime.unregisterProvider(localDomain, <interface>provider);
        } catch (JoynrRuntimeException e) {
            // handle error
        }
    }
    runtime.shutdown(true);
    try {
        Thread.sleep(3000);
    } catch (InterruptedException e) {
        // do nothing; exiting application
    }
    System.exit(0);
}
```

### Access control

The following allows anyone to access interface:

```java
private static void provisionAccessControl(Properties properties, String domain) throws Exception {
   ObjectMapper objectMapper = new ObjectMapper();
   objectMapper.enableDefaultTypingAsProperty(DefaultTyping.JAVA_LANG_OBJECT, "_typeName");
   MasterAccessControlEntry newMasterAccessControlEntry = new MasterAccessControlEntry(
       "*",
       domain,
       MyProvider.INTERFACE_NAME,
       TrustLevel.LOW,
       Arrays.asList(TrustLevel.LOW),
       TrustLevel.LOW,
       Arrays.asList(TrustLevel.LOW),
       "*",
       Permission.YES,
       Arrays.asList(Permission.YES)
   );
   MasterAccessControlEntry[] provisionedAccessControlEntries = { newMasterAccessControlEntry };
   String provisionedAccessControlEntriesAsJson = objectMapper.writeValueAsString(provisionedAccessControlEntries);
   properties.setProperty(StaticDomainAccessControlProvisioning.PROPERTY_PROVISIONED_MASTER_ACCESSCONTROLENTRIES,
       provisionedAccessControlEntriesAsJson);
}
```

## The My&lt;Interface>Provider class

The provider class implements the **attributes**, **methods** and **broadcasts** of a particular
Franca interface.

### Required imports
The following Joynr Java imports are required:

```java
import io.joynr.provider.Deferred;
import io.joynr.provider.DeferredVoid;
import io.joynr.provider.Promise;
import joynr.<Package>.<Interface>AbstractProvider;
```

### The Provider quality of service

The ```ProviderQos``` has the following members:

* **customParameters** e.g. the key-value for the arbitration strategy Keyword during discovery
* **providerVersion** the version of the provider
* **priority** the priority used for arbitration strategy HighestPriority during discovery
* **scope** the Provider scope (see below), used in discovery
* **supportsOnChangeSubscriptions** whether the provider supports subscriptions on changes

The **ProviderScope** can be
* **LOCAL** The provider will be registered in the local capability directory
* **GLOBAL** The provider will be registered in the local and global capability directory

Example:

```java
ProviderQos providerQos = new ProviderQos();
providerQos.setCustomParameters(customParameters);
providerQos.setProviderVersion(1);
providerQos.setPriority(100);
providerQos.setScope(ProviderScope.GLOBAL);
providerQos.setSupportsOnChangeSubscriptions(true);
```

### The base class
The provider class must extend the generated class ```<Interface>AbstractProvider``` (or
Default<Interface>Provider) and implement getter methods for each Franca attribute and a method for
each method of the Franca interface. In order to send broadcasts the generated code of the super
class ```<Interface>AbstractProvider``` can be used.

```java
package myPackage;
...
// required imports
...
public class My<Interface>Provider extends <Interface>AbstractProvider {
    // member variables realizing the Franca interfaces Attributes go here, if any
    <AttributeType> <Attribute>;
    ...
    // default constructor
    public My<Interface>Provider() {
        // initialize members and attributes here, if any
    }
    ...
    // foreach Franca interface "<Attribute>" provide a getter method
    ...
    // foreach Franca interface "<method>" provide an implementation
    ...
    // foreach Franca "<broadcast>" you can use the provided method to fire the event
    ...
}
```

### Providing attribute getters
The asynchronous getter methods return the current value of an attribute. Since the current thread
is blocked while the getter runs, activity should be kept as short as possible. In most cases, when
a simple element is returned, the method can resolve the Promise immediately. However, if longer
activity is required, it should be done in the background and the deferred should also be resolved
by a background thread.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
@Override
public Promise<Deferred<<AttributeType>>> get<Attribute>() {
    Deferred<<AttributeType>> deferred = new Deferred<<AttributeType>>();
    <AttributeType> value;
    ...
    // start some activity to get the value
    // if complex, execute this asynchronously;
    // once the value is available, resolve the Promise
    // may be run from background thread, if required
    deferred.resolve(value);
    // if an error occurs, the Deferred can be rejected with a ProviderRuntimeException
    deferred.reject(new ProviderRuntimeException(<errorMessage>));
    ...
    // from current thread
    return new Promise<Deferred<<AttributeType>>>(deferred);
}
```

### Providing attribute setters
Since the current thread is blocked while the setter runs, activity should be kept as short as
possible. In most cases, when a simple element is returned, the method can resolve the Promise
immediately. However, if longer activity is required, it should be done in the background and the
deferred should also be resolved by a background thread.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
@Override
public Promise<DeferredVoid> set<Attribute>(<AttributeType> <attribute>) {
    DeferredVoid deferred = new DeferredVoid();
    ...
    // start some activity to set the value
    // if complex, execute this asynchronously;
    // once the value is set, resolve the Promise
    // may be run from background thread, if required
    deferred.resolve();
    // if an error occurs, the Deferred can be rejected with a ProviderRuntimeException
    deferred.reject(new ProviderRuntimeException(<errorMessage>));
    // if attribute is notifiable (not marked as noSubscriptions in the Franca model),
    // inform subscribers about the value change
    <Attribute>Changed(<Attribute>);
    ...
    // from current thread
    return new Promise<DeferredVoid>(deferred);
}
```

### Implementing a Franca RPC method
The provider should always implement RPC calls asynchronously in order to not block the main thread
longer than required. Also it needs to take care not to overload the server, e.g. it must not accept
unlimited amount of RPC requests causing background activity. After exceeding a limit, further calls
should be rejected until the number of outstanding activities falls below the limit again.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
// in case of single return parameter

@Override
public Promise<Deferred<<ReturnType>>> <method>(... parameters ...) {
    Deferred<<ReturnType>> deferred = new Deferred<<ReturnType>>();
    <ReturnType> returnValue;
    ...
    // start some activity to perform the task;
    // if complex, execute this asynchronously by background thread;
    // once the task is finished, resolve the Promise providing
    // the returnValue, if any (see following line).
    deferred.resolve(returnValue);

    // For methods which are modelled with error enumerations, the Promise can be rejected with such
    // an error enumeration. It is then wrapped in an ApplicationException which serves as container
    // for the actual error enumeration.
    deferred.reject(<ErrorEnum>.<VALUE>);

    // If no errors are modelled, the Deferred can be rejected with a ProviderRuntimeException
    deferred.reject(new ProviderRuntimeException(<errorMessage>));
    ...
    // from current thread
    return new Promise<Deferred<<ReturnType>>>(deferred);
}

// in case of multiple return parameters

@Override
public Promise<<Method>Deferred>> <method>(... parameters ...) {
    <Method>Deferred deferred = new <Method>Deferred();
    <ReturnType1> returnValue1;
    ...
    <ReturnTypeN> returnValueN;
    ...
    // start some activity to perform the task;
    // if complex, execute this asynchronously by background thread;
    // once the task is finished, resolve the Promise providing
    // the returnValue, if any (see following line).
    deferred.resolve(returnValue1, ..., returnValueN);

    // For methods which are modelled with error enumerations, the Promise can be rejected with such
    // an error enumeration. It is then wrapped in an ApplicationException which serves as container
    // for the actual error enumeration.
    deferred.reject(<ErrorEnum>.<VALUE>);

    // If no errors are modelled, the Deferred can be rejected with a ProviderRuntimeException
    deferred.reject(new ProviderRuntimeException(<errorMessage>));
    ...
    // from current thread
    return new Promise<<Method>Deferred>(deferred);
}
```

### Firing a broadcast
Firing a broadcast blocks the current thread until the message is serialized.

```java
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public void fire<Broadcast>Event {
    <OutputValueType1> outputValue1;
    ...
    <OutputValueTypeN> outputValueN;
    ...
    // setup outputValue(s)
    ...
    // use the method provided by generators to send the broadcast
    fire<Broadcast>(outputValue1, ... , outputValueN);
}
```
## Selective (filtered) broadcasts
In contrast to unfiltered broadcasts, to realize selective (filtered) broadcasts, the filter logic
has to be implemented and registered by the provider. If multiple filters are registered on the same
provider and broadcast, all filters are applied in a chain and the broadcast is only delivered if
all filters in the chain return true.

### The broadcast filter classes
A broadcast filter class implements a filtering function called ```filter()``` which returns a
boolean value indicating whether the broadcast should be delivered. The input parameters of the
```filter()``` method reflect the output values of the broadcast.

```java
import joynr.<Package>.<Interface>BroadcastInterface.<Broadcast>BroadcastFilterParameters;
import joynr.<Package>.<Interface><Broadcast>BroadcastFilter;
// for any Franca type named "<Type>" used
import joynr.<Package>.<TypeCollection>.<Type>;
...
public class <Filter>BroadcastFilter extends <Interface><Broadcast>BroadcastFilter {
    ...
    @Override
    public boolean filter(
        <OutputValueType1> outputValue1,
        ...
        <OutputValueTypeN> outputValueN,
        <Broadcast>BroadcastFilterParameters filterParameters
    ) {
        boolean returnValue;
        ...
        // calculate result
        ...
        return returnValue;
    }
}
```
