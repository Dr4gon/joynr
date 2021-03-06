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

define(
        [
            "joynr/proxy/SubscriptionQos",
            "joynr/proxy/PeriodicSubscriptionQos",
            "joynr/proxy/OnChangeSubscriptionQos",
            "joynr/proxy/OnChangeWithKeepAliveSubscriptionQos",
            "Date"
        ],
        function(
                SubscriptionQos,
                PeriodicSubscriptionQos,
                OnChangeSubscriptionQos,
                OnChangeWithKeepAliveSubscriptionQos,
                Date) {
            describe(
                    "libjoynr-js.joynr.proxy.SubscriptionQos",
                    function() {

                        var qosSettings = {
                            minIntervalMs : 50,
                            maxIntervalMs : 51,
                            expiryDateMs : 4,
                            alertAfterIntervalMs : 80,
                            publicationTtlMs : 100
                        };

                        it("is instantiable", function(done) {
                            expect(new OnChangeWithKeepAliveSubscriptionQos(qosSettings))
                                    .toBeDefined();
                            done();
                        });

                        it("is of correct type", function(done) {
                            var subscriptionQos =
                                    new OnChangeWithKeepAliveSubscriptionQos(qosSettings);
                            expect(subscriptionQos).toBeDefined();
                            expect(subscriptionQos).not.toBeNull();
                            expect(typeof subscriptionQos === "object").toBeTruthy();
                            expect(subscriptionQos instanceof OnChangeWithKeepAliveSubscriptionQos)
                                    .toEqual(true);
                            done();
                        });

                        function createSubscriptionQos(
                                minIntervalMs,
                                periodMs,
                                onChange,
                                expiryDateMs,
                                alertAfterIntervalMs,
                                publicationTtlMs) {
                            var returnValue;
                            if (onChange) {
                                returnValue = new OnChangeWithKeepAliveSubscriptionQos({
                                    minIntervalMs : minIntervalMs,
                                    maxIntervalMs : periodMs,
                                    expiryDateMs : expiryDateMs,
                                    alertAfterIntervalMs : alertAfterIntervalMs,
                                    publicationTtlMs : publicationTtlMs
                                });
                            } else {
                                returnValue = new PeriodicSubscriptionQos({
                                    periodMs : periodMs,
                                    expiryDateMs : expiryDateMs,
                                    alertAfterIntervalMs : alertAfterIntervalMs,
                                    publicationTtlMs : publicationTtlMs
                                });
                            }
                            return returnValue;
                        }

                        function testValues(
                                minIntervalMs,
                                periodMs,
                                onChange,
                                expiryDateMs,
                                alertAfterIntervalMs,
                                publicationTtlMs) {
                            var subscriptionQos =
                                    createSubscriptionQos(
                                            minIntervalMs,
                                            periodMs,
                                            onChange,
                                            expiryDateMs,
                                            alertAfterIntervalMs,
                                            publicationTtlMs);
                            var expectedMaxIntervalMs = periodMs;
                            if (minIntervalMs < OnChangeSubscriptionQos.MIN_MIN_INTERVAL_MS) {
                                minIntervalMs = OnChangeSubscriptionQos.MIN_MIN_INTERVAL_MS;
                            }
                            if (minIntervalMs > OnChangeSubscriptionQos.MAX_MIN_INTERVAL_MS) {
                                minIntervalMs = OnChangeSubscriptionQos.MAX_MIN_INTERVAL_MS;
                            }

                            if (expectedMaxIntervalMs < OnChangeWithKeepAliveSubscriptionQos.MIN_MAX_INTERVAL_MS) {
                                expectedMaxIntervalMs =
                                        OnChangeWithKeepAliveSubscriptionQos.MIN_MAX_INTERVAL_MS;
                            }
                            if (expectedMaxIntervalMs > OnChangeWithKeepAliveSubscriptionQos.MAX_MAX_INTERVAL_MS) {
                                expectedMaxIntervalMs =
                                        OnChangeWithKeepAliveSubscriptionQos.MAX_MAX_INTERVAL_MS;
                            }
                            if (expectedMaxIntervalMs < minIntervalMs) {
                                expectedMaxIntervalMs = minIntervalMs;
                            }
                            if (onChange) {
                                var expectedMinIntervalMs = minIntervalMs;

                                expect(subscriptionQos.minIntervalMs).toBe(expectedMinIntervalMs);

                                expect(subscriptionQos.maxIntervalMs).toBe(expectedMaxIntervalMs);
                            } else {
                                expect(subscriptionQos.periodMs).toBe(expectedMaxIntervalMs);
                            }
                            var expectedPublicationTtlMs = publicationTtlMs;
                            if (expectedPublicationTtlMs < SubscriptionQos.MIN_PUBLICATION_TTL_MS) {
                                expectedPublicationTtlMs = SubscriptionQos.MIN_PUBLICATION_TTL_MS;
                            }
                            if (expectedPublicationTtlMs > SubscriptionQos.MAX_PUBLICATION_TTL_MS) {
                                expectedPublicationTtlMs = SubscriptionQos.MAX_PUBLICATION_TTL_MS;
                            }
                            expect(subscriptionQos.publicationTtlMs).toBe(expectedPublicationTtlMs);

                            if (expiryDateMs < SubscriptionQos.MIN_EXPIRY_MS) {
                                expiryDateMs = SubscriptionQos.MIN_EXPIRY_MS;
                            }
                            expect(subscriptionQos.expiryDateMs).toBe(expiryDateMs);

                            var expectedAlertAfterIntervalMs = alertAfterIntervalMs;
                            if (expectedAlertAfterIntervalMs > OnChangeWithKeepAliveSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS) {
                                expectedAlertAfterIntervalMs =
                                        OnChangeWithKeepAliveSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS;
                            }
                            if (expectedAlertAfterIntervalMs !== OnChangeWithKeepAliveSubscriptionQos.NO_ALERT_AFTER_INTERVAL
                                && expectedAlertAfterIntervalMs < expectedMaxIntervalMs) {
                                expectedAlertAfterIntervalMs = expectedMaxIntervalMs;
                            }
                            expect(subscriptionQos.alertAfterIntervalMs).toBe(
                                    expectedAlertAfterIntervalMs);
                            return subscriptionQos;
                        }

                        it(
                                "constructs with correct member values",
                                function(done) {
                                    //wrong publicationTtlMs
                                    expect(function() {
                                        createSubscriptionQos(1, 2, false, 4, 5, -6);
                                    }).toThrow();
                                    //wrong periodMs
                                    expect(function() {
                                        createSubscriptionQos(1, 2, false, 4, 5, 100);
                                    }).toThrow();
                                    //wrong periodMs (exceeds MIN_PERIOD_MS)
                                    expect(
                                            function() {
                                                createSubscriptionQos(
                                                        1,
                                                        PeriodicSubscriptionQos.MIN_PERIOD_MS - 1,
                                                        false,
                                                        4,
                                                        5,
                                                        100);
                                            }).toThrow();
                                    //wrong periodMs (exceeds MAX_PERIOD_MS)
                                    expect(
                                            function() {
                                                createSubscriptionQos(
                                                        1,
                                                        PeriodicSubscriptionQos.MAX_PERIOD_MS + 1,
                                                        false,
                                                        4,
                                                        5,
                                                        100);
                                            }).toThrow();
                                    //wrong alertAfterIntervalMs (shall be higher then the periodMs)
                                    expect(
                                            createSubscriptionQos(1, 50, false, 4, 5, 100).alertAfterIntervalMs)
                                            .toEqual(50);
                                    //wrong alertAfterIntervalMs (exceed MAX_ALERT_AFTER_INTERVAL_MS)
                                    expect(
                                            createSubscriptionQos(
                                                    1,
                                                    50,
                                                    false,
                                                    4,
                                                    OnChangeWithKeepAliveSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS + 1,
                                                    100).alertAfterIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS);
                                    testValues(1, 50, false, 4, 51, 100);

                                    //wrong publicationTtlMs
                                    expect(testValues(-1, -2, true, -4, -5, -6).publicationTtlMs)
                                            .toEqual(SubscriptionQos.MIN_PUBLICATION_TTL_MS);
                                    //wrong publicationTtlMs
                                    expect(
                                            testValues(
                                                    60,
                                                    62,
                                                    true,
                                                    10,
                                                    100,
                                                    SubscriptionQos.MAX_PUBLICATION_TTL_MS + 1).publicationTtlMs)
                                            .toEqual(SubscriptionQos.MAX_PUBLICATION_TTL_MS);
                                    //wrong minIntervalMs
                                    expect(testValues(-1, -2, true, -4, -5, 200).minIntervalMs)
                                            .toEqual(OnChangeSubscriptionQos.MIN_MIN_INTERVAL_MS);
                                    //wrong minIntervalMs (exceeds MAX_MIN_INTERVAL_MS)
                                    expect(
                                            testValues(
                                                    OnChangeSubscriptionQos.MAX_MIN_INTERVAL_MS + 1,
                                                    62,
                                                    true,
                                                    10,
                                                    100,
                                                    200).minIntervalMs).toEqual(
                                            OnChangeSubscriptionQos.MAX_MIN_INTERVAL_MS);

                                    //wrong maxIntervalMs (shall be higher than minIntervalMs)
                                    expect(testValues(60, -2, true, -4, -5, 200).maxIntervalMs)
                                            .toEqual(60);
                                    //wrong maxIntervalMs (below OnChangeWithKeepAliveSubscriptionQos.MIN_MAX_INTERVAL_MS)
                                    expect(
                                            testValues(
                                                    10,
                                                    OnChangeWithKeepAliveSubscriptionQos.MIN_MAX_INTERVAL_MS - 1,
                                                    true,
                                                    -4,
                                                    -5,
                                                    200).maxIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.MIN_MAX_INTERVAL_MS);
                                    //wrong maxIntervalMs (exceeds OnChangeWithKeepAliveSubscriptionQos.MAX_MAX_INTERVAL_MS)
                                    expect(
                                            testValues(
                                                    10,
                                                    OnChangeWithKeepAliveSubscriptionQos.MAX_MAX_INTERVAL_MS + 1,
                                                    true,
                                                    -4,
                                                    -5,
                                                    200).maxIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.MAX_MAX_INTERVAL_MS);
                                    //wrong alertAfterIntervalMs (shall be higher than maxIntervalMs)
                                    expect(
                                            testValues(60, 62, true, -4, -5, 200).alertAfterIntervalMs)
                                            .toEqual(62);
                                    //wrong alertAfterIntervalMs (exceeds MAX_ALERT_AFTER_INTERVAL_MS)
                                    expect(
                                            testValues(
                                                    60,
                                                    62,
                                                    true,
                                                    -4,
                                                    PeriodicSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS + 1,
                                                    200).alertAfterIntervalMs).toEqual(
                                            PeriodicSubscriptionQos.MAX_ALERT_AFTER_INTERVAL_MS);
                                    //wrong expiryDate
                                    expect(testValues(60, -2, true, -4, 100, 200).expiryDateMs)
                                            .toEqual(SubscriptionQos.MIN_EXPIRY_MS);
                                    testValues(60, 62, true, 10, 100, 200);

                                    //wrong publicationTtlMs
                                    expect(function() {
                                        testValues(0, 0, false, 0, 0, 0);
                                    }).toThrow();
                                    //wrong periodMs
                                    expect(function() {
                                        testValues(0, 0, false, 0, 0, 100);
                                    }).toThrow();
                                    testValues(0, 50, false, 0, 0, 100);
                                    done();
                                });

                        it(
                                "constructs OnChangeWithKeepAliveSubscriptionQos with correct default values",
                                function(done) {
                                    var fixture = new OnChangeWithKeepAliveSubscriptionQos();
                                    expect(fixture.minIntervalMs).toEqual(
                                            OnChangeSubscriptionQos.DEFAULT_MIN_INTERVAL_MS);
                                    expect(fixture.maxIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.DEFAULT_MAX_INTERVAL_MS);
                                    expect(fixture.expiryDateMs).toEqual(
                                            SubscriptionQos.NO_EXPIRY_DATE);
                                    expect(fixture.alertAfterIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.DEFAULT_ALERT_AFTER_INTERVAL_MS);
                                    expect(fixture.publicationTtlMs).toEqual(
                                            SubscriptionQos.DEFAULT_PUBLICATION_TTL_MS);
                                    done();
                                });

                        it(
                                "constructs PeriodicSubscriptionQos with correct default values",
                                function(done) {
                                    var fixture = new PeriodicSubscriptionQos();
                                    expect(fixture.periodMs).toEqual(
                                            PeriodicSubscriptionQos.DEFAULT_PERIOD_MS);
                                    expect(fixture.expiryDateMs).toEqual(
                                            SubscriptionQos.NO_EXPIRY_DATE);
                                    expect(fixture.alertAfterIntervalMs)
                                            .toEqual(
                                                    OnChangeWithKeepAliveSubscriptionQos.DEFAULT_ALERT_AFTER_INTERVAL_MS);
                                    expect(fixture.publicationTtlMs).toEqual(
                                            SubscriptionQos.DEFAULT_PUBLICATION_TTL_MS);
                                    done();
                                });

                        it(
                                "SubscriptionQos.clearExpiryDate clears the expiry date",
                                function(done) {
                                    var fixture = new OnChangeWithKeepAliveSubscriptionQos({
                                        expiryDateMs : 1234
                                    });

                                    expect(fixture.expiryDateMs).toBe(1234);
                                    fixture.clearExpiryDate();
                                    expect(fixture.expiryDateMs).toBe(
                                            SubscriptionQos.NO_EXPIRY_DATE);
                                    done();
                                });

                        it(
                                "PeriodicSubscriptionQos.clearAlertAfterInterval clears the alert after interval",
                                function(done) {
                                    var alertAfterIntervalMs =
                                            PeriodicSubscriptionQos.DEFAULT_PERIOD_MS + 1;
                                    var fixture = new PeriodicSubscriptionQos({
                                        alertAfterIntervalMs : alertAfterIntervalMs
                                    });

                                    expect(fixture.alertAfterIntervalMs).toBe(alertAfterIntervalMs);
                                    fixture.clearAlertAfterInterval();
                                    expect(fixture.alertAfterIntervalMs).toBe(
                                            PeriodicSubscriptionQos.NO_ALERT_AFTER_INTERVAL);
                                    done();
                                });

                        it(
                                "OnChangeWithKeepAliveSubscriptionQos.clearAlertAfterInterval clears the alert after interval",
                                function(done) {
                                    var alertAfterIntervalMs =
                                            OnChangeWithKeepAliveSubscriptionQos.DEFAULT_MAX_INTERVAL_MS + 1;
                                    var fixture = new OnChangeWithKeepAliveSubscriptionQos({
                                        alertAfterIntervalMs : alertAfterIntervalMs
                                    });

                                    expect(fixture.alertAfterIntervalMs).toBe(alertAfterIntervalMs);
                                    fixture.clearAlertAfterInterval();
                                    expect(fixture.alertAfterIntervalMs)
                                            .toBe(
                                                    OnChangeWithKeepAliveSubscriptionQos.NO_ALERT_AFTER_INTERVAL);
                                    done();
                                });

                        it("create deprecated subscriptionQos objects", function(done) {
                            var deprecatedQos = new OnChangeWithKeepAliveSubscriptionQos({
                                minInterval : 0,
                                maxInterval : 50,
                                expiryDate : 1000,
                                alertAfterInterval : 200,
                                publicationTtl : 100

                            });
                            expect(deprecatedQos.expiryDateMs).toEqual(1000);
                            expect(deprecatedQos.publicationTtlMs).toEqual(100);
                            expect(deprecatedQos.alertAfterIntervalMs).toEqual(200);
                            expect(deprecatedQos.minIntervalMs).toEqual(0);
                            expect(deprecatedQos.maxIntervalMs).toEqual(50);
                            done();
                        });

                        it(
                                "subscription qos accepts validity instead of expiry date as constructor member",
                                function(done) {
                                    var fakeTime = 374747473;
                                    var validityMs = 23232;
                                    spyOn(Date, "now").and.callFake(function() {
                                        return fakeTime;
                                    });

                                    var fixture = new OnChangeWithKeepAliveSubscriptionQos({
                                        validityMs : validityMs
                                    });
                                    expect(fixture.validityMs).toBe(undefined);
                                    expect(fixture.expiryDateMs).toBe(fakeTime + validityMs);
                                    done();
                                });

                        it("throws on incorrectly typed values", function(done) {
                            // all arguments
                            expect(function() {
                                createSubscriptionQos(1, 50, false, 4, 80, 100);
                            }).not.toThrow();

                            // no arguments
                            expect(function() {
                                createSubscriptionQos(
                                        undefined,
                                        undefined,
                                        undefined,
                                        undefined,
                                        undefined,
                                        undefined);
                            }).not.toThrow();

                            // arguments 1 wrongly types
                            expect(function() {
                                createSubscriptionQos({}, 50, true, 4, 80, 100);
                            }).toThrow();

                            // arguments 2 wrongly types
                            expect(function() {
                                createSubscriptionQos(1, {}, false, 4, 80, 100);
                            }).toThrow();

                            // arguments 4 wrongly types
                            expect(function() {
                                createSubscriptionQos(1, 50, false, {}, 80, 100);
                            }).toThrow();

                            // arguments 5 wrongly types
                            expect(function() {
                                createSubscriptionQos(1, 50, false, 4, {}, 100);
                            }).toThrow();

                            // arguments 6 wrongly types
                            expect(function() {
                                createSubscriptionQos(1, 50, false, 4, 80, {});
                            }).toThrow();
                            done();

                        });
                    });

        }); // require
