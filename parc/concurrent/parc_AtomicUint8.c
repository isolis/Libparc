/*
 * Copyright (c) 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
/**
 * @author Glenn Scott <Glenn.Scott@parc.com>, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_DisplayIndented.h>

#include <parc/concurrent/parc_AtomicUint8.h>

struct PARCAtomicUint8 {
    uint8_t value;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_t mutex;
#endif
};

static void
_parcAtomicUint8_Finalize(PARCAtomicUint8 **instancePtr)
{
    assertNotNull(instancePtr, "Parameter must be a non-null pointer to a PARCAtomicUint8 pointer.");

    parcAtomicUint8_OptionalAssertValid((*instancePtr));

    /* cleanup the instance fields here */
}

parcObject_ImplementAcquire(parcAtomicUint8, PARCAtomicUint8);

parcObject_ImplementRelease(parcAtomicUint8, PARCAtomicUint8);

parcObject_ExtendPARCObject(PARCAtomicUint8, _parcAtomicUint8_Finalize, parcAtomicUint8_Copy, NULL, parcAtomicUint8_Equals, parcAtomicUint8_Compare, parcAtomicUint8_HashCode, NULL);


void
parcAtomicUint8_AssertValid(const PARCAtomicUint8 *instance)
{
    assertTrue(parcAtomicUint8_IsValid(instance),
               "PARCAtomicUint8 is not valid.");
}

PARCAtomicUint8 *
parcAtomicUint8_Create(uint8_t value)
{
    PARCAtomicUint8 *result = parcObject_CreateAndClearInstance(PARCAtomicUint8);

#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_init(&result->mutex, NULL);
    result->value = value;
#else
    *result = value;
#endif

    return result;
}

int
parcAtomicUint8_Compare(const PARCAtomicUint8 *instance, const PARCAtomicUint8 *other)
{
    int8_t comparison = parcAtomicUint8_GetValue(instance) - parcAtomicUint8_GetValue(other);

    int result = 0;
    if (comparison < 0) {
        result = -1;
    } else if (comparison > 0) {
        result = 1;
    }

    return result;
}

PARCAtomicUint8 *
parcAtomicUint8_Copy(const PARCAtomicUint8 *original)
{
    PARCAtomicUint8 *result = parcAtomicUint8_Create(parcAtomicUint8_GetValue(original));

    return result;
}

bool
parcAtomicUint8_Equals(const PARCAtomicUint8 *x, const PARCAtomicUint8 *y)
{
    bool result = false;

    result = parcAtomicUint8_GetValue(x) == parcAtomicUint8_GetValue(y);

    return result;
}

uint32_t
parcAtomicUint8_HashCode(const PARCAtomicUint8 *instance)
{
    uint32_t result = (uint32_t) parcAtomicUint8_GetValue(instance);

    return result;
}

bool
parcAtomicUint8_IsValid(const PARCAtomicUint8 *instance)
{
    bool result = false;

    if (instance != NULL) {
        result = true;
    }

    return result;
}

uint8_t
parcAtomicUint8_GetValue(const PARCAtomicUint8 *instance)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    return instance->value;
#else
    return *instance;
#endif
}

uint8_t
parcAtomicUint8_AddImpl(PARCAtomicUint8 *value, uint8_t addend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value += addend;
    uint8_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_add_and_fetch(value, addend);
#endif
}

uint8_t
parcAtomicUint8_SubtractImpl(PARCAtomicUint8 *value, uint8_t subtrahend)
{
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    value->value -= subtrahend;
    uint8_t result = value->value;
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    return __sync_sub_and_fetch(value, subtrahend);
#endif
}

bool
parcAtomicUint8_CompareAndSwapImpl(PARCAtomicUint8 *value, uint8_t predicate, uint8_t newValue)
{
    bool result = false;
#ifdef PARCLibrary_DISABLE_ATOMICS
    pthread_mutex_lock(&value->mutex);
    if (value->value == predicate) {
        value->value = newValue;
        result = true;
    }
    pthread_mutex_unlock(&value->mutex);
    return result;
#else
    result = __sync_bool_compare_and_swap(value, predicate, newValue);
#endif
    return result;
}
