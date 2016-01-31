/*
 * Copyright (c) 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Patent rights are not granted under this agreement. Patent rights are
 *       available under FRAND terms.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL XEROX or PARC BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/** *
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
// Include the file(s) containing the functions to be tested.
// This permits internal static functions to be visible to this Test Framework.
#include "../parc_SigningAlgorithm.c"

#include <LongBow/testing.h>
#include <LongBow/debugging.h>

LONGBOW_TEST_RUNNER(test_parc_SigningAlgorithm)
{
    // The following Test Fixtures will run their corresponding Test Cases.
    // Test Fixtures are run in the order specified, but all tests should be idempotent.
    // Never rely on the execution order of tests or share state between them.
    LONGBOW_RUN_TEST_FIXTURE(Global);
}

// The Test Runner calls this function once before any Test Fixtures are run.
LONGBOW_TEST_RUNNER_SETUP(test_parc_SigningAlgorithm)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

// The Test Runner calls this function once after all the Test Fixtures are run.
LONGBOW_TEST_RUNNER_TEARDOWN(test_parc_SigningAlgorithm)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE(Global)
{
    LONGBOW_RUN_TEST_CASE(Global, parcSigningAlgorithm_ToFromString);
    LONGBOW_RUN_TEST_CASE(Global, parcSigningAlgorithm_ToFromString_NotFound);
    LONGBOW_RUN_TEST_CASE(Global, parcSigningAlgorithm_GetSigningAlgorithm);
    LONGBOW_RUN_TEST_CASE(Global, parcSigningAlgorithm_FromString_NotFound);
    LONGBOW_RUN_TEST_CASE(Global, parcSigningAlgorithm_GetSigningAlgorithm_BadAlgorithm);
}

LONGBOW_TEST_FIXTURE_SETUP(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_FIXTURE_TEARDOWN(Global)
{
    return LONGBOW_STATUS_SUCCEEDED;
}

LONGBOW_TEST_CASE(Global, parcSigningAlgorithm_ToFromString)
{
    PARCSigningAlgorithm expected = PARCSigningAlgorithm_HMAC;

    const char *string = parcSigningAlgorithm_ToString(expected);

    PARCSigningAlgorithm actual = parcSigningAlgorithm_FromString(string);

    assertTrue(expected == actual, "Expected %d, actual %d", expected, actual);
}

LONGBOW_TEST_CASE(Global, parcSigningAlgorithm_ToFromString_NotFound)
{
    PARCSigningAlgorithm expected = 123456;

    const char *string = parcSigningAlgorithm_ToString(expected);

    assertNull(string, "Expect parcSigningAlgorithm_ToString to return NULL");
}

LONGBOW_TEST_CASE(Global, parcSigningAlgorithm_FromString_NotFound)
{
    PARCSigningAlgorithm actual = parcSigningAlgorithm_FromString("garbage string of unknown stuff");

    assertTrue(actual == PARCSigningAlgorithm_UNKNOWN,
               "Expect parcSigningAlgorithm_FromString to return PARCSigningAlgorithm_UNKNOWN");
}

LONGBOW_TEST_CASE(Global, parcSigningAlgorithm_GetSigningAlgorithm)
{
    PARCSigningAlgorithm actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_DSA_SHA256);
    assertTrue(PARCSigningAlgorithm_DSA == actual, "Expected %d, actual %d", PARCSigningAlgorithm_DSA, actual);

    actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_RSA_SHA256);
    assertTrue(PARCSigningAlgorithm_RSA == actual, "Expected %d, actual %d", PARCSigningAlgorithm_RSA, actual);

    actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_RSA_SHA512);
    assertTrue(PARCSigningAlgorithm_RSA == actual, "Expected %d, actual %d", PARCSigningAlgorithm_RSA, actual);

    actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_HMAC_SHA256);
    assertTrue(PARCSigningAlgorithm_HMAC == actual, "Expected %d, actual %d", PARCSigningAlgorithm_HMAC, actual);

    actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_HMAC_SHA512);
    assertTrue(PARCSigningAlgorithm_HMAC == actual, "Expected %d, actual %d", PARCSigningAlgorithm_HMAC, actual);

    actual = parcSigningAlgorithm_GetSigningAlgorithm(PARCCryptoSuite_NULL_CRC32C);
    assertTrue(PARCSigningAlgortihm_NULL == actual, "Expected %d, actual %d", PARCSigningAlgortihm_NULL, actual);
}

LONGBOW_TEST_CASE_EXPECTS(Global, parcSigningAlgorithm_GetSigningAlgorithm_BadAlgorithm, .event = &LongBowTrapIllegalValue)
{
    parcSigningAlgorithm_GetSigningAlgorithm(-1);
}

int
main(int argc, char *argv[argc])
{
    LongBowRunner *testRunner = LONGBOW_TEST_RUNNER_CREATE(test_parc_SigningAlgorithm);
    int exitStatus = LONGBOW_TEST_MAIN(argc, argv, testRunner, NULL);
    longBowTestRunner_Destroy(&testRunner);
    exit(exitStatus);
}
