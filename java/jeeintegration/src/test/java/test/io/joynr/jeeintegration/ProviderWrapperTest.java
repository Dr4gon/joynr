package test.io.joynr.jeeintegration;

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

import static org.junit.Assert.assertArrayEquals;
import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertFalse;
import static org.junit.Assert.assertNotNull;
import static org.junit.Assert.assertTrue;
import static org.junit.Assert.fail;
import static org.mockito.Mockito.doReturn;
import static org.mockito.Mockito.eq;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import java.lang.reflect.Method;
import java.lang.reflect.Proxy;

import javax.enterprise.context.spi.CreationalContext;
import javax.enterprise.inject.spi.Bean;
import javax.enterprise.inject.spi.BeanManager;

import com.google.common.collect.Sets;
import com.google.inject.Injector;
import io.joynr.dispatcher.rpc.MultiReturnValuesContainer;
import io.joynr.exceptions.JoynrException;
import io.joynr.jeeintegration.ProviderWrapper;
import io.joynr.jeeintegration.api.ProviderQosFactory;
import io.joynr.jeeintegration.api.security.JoynrCallingPrincipal;
import io.joynr.jeeintegration.context.JoynrJeeMessageContext;
import io.joynr.messaging.JoynrMessageCreator;
import io.joynr.provider.Deferred;
import io.joynr.provider.DeferredVoid;
import io.joynr.provider.JoynrProvider;
import io.joynr.provider.Promise;
import io.joynr.provider.PromiseListener;
import io.joynr.provider.SubscriptionPublisher;
import io.joynr.provider.SubscriptionPublisherInjection;
import joynr.exceptions.ProviderRuntimeException;
import joynr.types.ProviderQos;
import org.junit.Assert;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.Mockito;
import org.mockito.runners.MockitoJUnitRunner;

/**
 * Unit tests for {@link ProviderWrapper}.
 */
@RunWith(MockitoJUnitRunner.class)
public class ProviderWrapperTest {

    private static final String USERNAME = "messageCreatorId";

    public static interface TestServiceProviderInterface extends SubscriptionPublisherInjection<SubscriptionPublisher> {
        String INTERFACE_NAME = "test";

        Promise<Deferred<String>> testServiceMethod(int paramOne, String paramTwo);

        Promise<Deferred<String>> testServiceMethodNoArgs();

        Promise<DeferredVoid> testServiceMethodVoidReturn();

        Promise<DeferredVoid> assertMessageContextActive();

        Promise<DeferredVoid> testThrowsProviderRuntimeException();

        Promise<Deferred<Object[]>> testMultiOutMethod();
    }

    public static interface TestServiceInterface {
        String INTERFACE_NAME = "test";

        String testServiceMethod(int paramOne, String paramTwo);

        String testServiceMethodNoArgs();

        void testServiceMethodVoidReturn();

        void assertMessageContextActive();

        void testThrowsProviderRuntimeException();

        public class MultiOutResult implements MultiReturnValuesContainer {
            public Object[] getValues() {
                return new Object[]{ "one", "two" };
            }
        }

        MultiOutResult testMultiOutMethod();
    }

    public static class TestServiceImpl implements TestServiceInterface,
            SubscriptionPublisherInjection<SubscriptionPublisher> {

        @Override
        public String testServiceMethod(int paramOne, String paramTwo) {
            return String.format("Result: %d %s", paramOne, paramTwo);
        }

        @Override
        public String testServiceMethodNoArgs() {
            return "test";
        }

        @Override
        public void testServiceMethodVoidReturn() {
        }

        @Override
        public void assertMessageContextActive() {
            assertTrue(JoynrJeeMessageContext.getInstance().isActive());
        }

        @Override
        public void testThrowsProviderRuntimeException() {
            throw new ProviderRuntimeException("test");
        }

        @Override
        public void setSubscriptionPublisher(SubscriptionPublisher subscriptionPublisher) {
            assertFalse(JoynrJeeMessageContext.getInstance().isActive());
        }

        public MultiOutResult testMultiOutMethod() {
            return new MultiOutResult();
        }

    }

    public static class TestProviderQosFactory implements ProviderQosFactory {
        private ProviderQos providerQos;

        public TestProviderQosFactory(ProviderQos providerQos) {
            this.providerQos = providerQos;
        }

        @Override
        public ProviderQos create() {
            return providerQos;
        }

        @Override
        public boolean providesFor(Class<?> serviceInterface) {
            return TestServiceInterface.class.isAssignableFrom(serviceInterface);
        }
    }

    @Mock
    private JoynrCallingPrincipal joynrCallingPincipal;

    @Test
    public void testInvokeVoidReturnMethod() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testServiceMethodVoidReturn");

        Object result = subject.invoke(proxy, method, new Object[0]);

        assertTrue(result instanceof Promise);
    }

    @Test
    public void testInvokeMethodThrowingProviderRuntimeException() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testThrowsProviderRuntimeException");

        Object result = subject.invoke(proxy, method, new Object[0]);
        assertNotNull(result);
        assertTrue(result instanceof Promise);
        assertTrue(((Promise) result).isRejected());
        ((Promise) result).then(new PromiseListener() {
            @Override
            public void onFulfillment(Object... values) {
                fail("Should never get here");
            }

            @Override
            public void onRejection(JoynrException error) {
                assertTrue(error instanceof ProviderRuntimeException);
            }
        });
    }

    @Test
    public void testInvokeMultiOutMethod() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testMultiOutMethod");

        Object result = subject.invoke(proxy, method, new Object[0]);

        assertTrue(result instanceof Promise);
        Promise promise = (Promise) result;
        assertTrue(promise.isFulfilled());
        promise.then(new PromiseListener() {
            @Override
            public void onFulfillment(Object... values) {
                assertArrayEquals(new Object[]{ "one", "two" }, values);
            }

            @Override
            public void onRejection(JoynrException error) {
                fail("Shouldn't be here.");
            }
        });
    }

    @Test
    public void testInvokeMethodWithParams() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testServiceMethod", new Class[]{ Integer.TYPE,
                String.class });

        Object result = subject.invoke(proxy, method, new Object[]{ 1, "one" });

        assertTrue(result instanceof Promise);
        assertPromiseEquals(result, new TestServiceImpl().testServiceMethod(1, "one"));
    }

    @Test
    public void testInvokeMethodNoArgs() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testServiceMethodNoArgs");

        Object result = subject.invoke(proxy, method, new Object[0]);

        assertTrue(result instanceof Promise);
        assertPromiseEquals(result, "test");
    }

    @Test
    public void testSetSubscriptionPublisherDoesNotActivateScope() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = SubscriptionPublisherInjection.class.getMethod("setSubscriptionPublisher",
                                                                       new Class[]{ SubscriptionPublisher.class });

        subject.invoke(proxy, method, new Object[]{ mock(SubscriptionPublisher.class) });
        assertFalse(JoynrJeeMessageContext.getInstance().isActive());
    }

    @Test
    public void testMessageScopeActivated() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("assertMessageContextActive");

        assertFalse(JoynrJeeMessageContext.getInstance().isActive());
        subject.invoke(proxy, method, new Object[0]);
        assertFalse(JoynrJeeMessageContext.getInstance().isActive());
    }

    @Test
    public void testPrincipalCopied() throws Throwable {
        ProviderWrapper subject = createSubject();
        JoynrProvider proxy = createProxy(subject);

        Method method = TestServiceProviderInterface.class.getMethod("testServiceMethodNoArgs");

        subject.invoke(proxy, method, new Object[0]);

        verify(joynrCallingPincipal).setUsername(USERNAME);
    }

    @SuppressWarnings("rawtypes")
    private void assertPromiseEquals(Object result, Object value) {
        assertTrue(((Promise) result).isFulfilled());
        Boolean[] ensureFulfilled = new Boolean[]{ false };
        ((Promise) result).then(new PromiseListener() {

            @Override
            public void onRejection(JoynrException error) {
                Assert.fail();
            }

            @Override
            public void onFulfillment(Object... values) {
                assertEquals(value, values[0]);
                ensureFulfilled[0] = true;
            }
        });
        assertTrue(ensureFulfilled[0]);
    }

    private ProviderWrapper createSubject() {
        return createSubject(Mockito.mock(BeanManager.class));
    }

    @SuppressWarnings("rawtypes")
    private ProviderWrapper createSubject(BeanManager beanManager) {
        Injector injector = mock(Injector.class);
        JoynrMessageCreator joynrMessageCreator = mock(JoynrMessageCreator.class);
        when(injector.getInstance(eq(JoynrMessageCreator.class))).thenReturn(joynrMessageCreator);
        when(joynrMessageCreator.getMessageCreatorId()).thenReturn(USERNAME);
        Bean<?> joynrCallingPrincipalBean = mock(Bean.class);
        when(beanManager.getBeans(JoynrCallingPrincipal.class)).thenReturn(Sets.newHashSet(joynrCallingPrincipalBean));
        when(beanManager.getReference(eq(joynrCallingPrincipalBean),
                                      eq(JoynrCallingPrincipal.class),
                                      Mockito.<CreationalContext> any())).thenReturn(joynrCallingPincipal);
        Bean<?> bean = mock(Bean.class);
        doReturn(TestServiceImpl.class).when(bean).getBeanClass();
        doReturn(new TestServiceImpl()).when(bean).create(null);
        ProviderWrapper subject = new ProviderWrapper(bean, beanManager, injector);
        return subject;
    }

    private JoynrProvider createProxy(ProviderWrapper forWrapper) {
        return (JoynrProvider) Proxy.newProxyInstance(ProviderWrapper.class.getClassLoader(), new Class[]{
                JoynrProvider.class, TestServiceInterface.class }, forWrapper);
    }

}
