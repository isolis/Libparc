/*
 * Copyright (c) 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC)
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
 *
 * @author Nacho "Ignacio" Solis & Mike Slominski, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#include <config.h>

#include <string.h>

#include "parc_KeyValue.h"
#include "parc_Object.h"

#include <LongBow/runtime.h>

struct parc_key_value {
    PARCObject *key;
    PARCObject *value;
};

static void
_parcKeyValue_Destroy(PARCKeyValue **keyValuePointer)
{
    PARCKeyValue *keyValue = *keyValuePointer;
    parcObject_Release(&keyValue->key);
    if (keyValue->value != NULL) {
        parcObject_Release(&keyValue->value);
    }
}

parcObject_ExtendPARCObject(PARCKeyValue, _parcKeyValue_Destroy, parcKeyValue_Copy, NULL, parcKeyValue_Equals,
                            parcKeyValue_Compare, parcKeyValue_HashCode, NULL);

parcObject_ImplementAcquire(parcKeyValue, PARCKeyValue);

parcObject_ImplementRelease(parcKeyValue, PARCKeyValue);

PARCKeyValue *
parcKeyValue_Create(const PARCObject *key,
                    const PARCObject *value)
{
    assertNotNull(key, "Key may not be null in a KeyValue element");

    PARCKeyValue *keyValue = parcObject_CreateInstance(PARCKeyValue);
    assertNotNull(keyValue, "parcMemory_Allocate(%zu) returned NULL", sizeof(PARCKeyValue));

    keyValue->key = parcObject_Acquire(key);
    keyValue->value = NULL;
    if (value != NULL) {
        keyValue->value = parcObject_Acquire(value);
    }

    return keyValue;
}

PARCKeyValue *
parcKeyValue_Copy(const PARCKeyValue *source)
{
    PARCKeyValue *newKV = parcObject_CreateInstance(PARCKeyValue);
    newKV->key = parcObject_Copy(source->key);
    newKV->value = NULL;
    if (source->value != NULL) {
        newKV->value = parcObject_Copy(source->value);
    }

    return newKV;
}

void
parcKeyValue_SetValue(PARCKeyValue *keyValue, PARCObject *value)
{
    assertNotNull(keyValue, "Not a valid keyValue");
    PARCObject *oldValue = keyValue->value;
    if (value != NULL) {
        keyValue->value = parcObject_Acquire(value);
    } else {
        keyValue->value = NULL;
    }
    if (oldValue != NULL) {
        parcObject_Release(&oldValue);
    }
}

void
parcKeyValue_SetKey(PARCKeyValue *keyValue, PARCObject *key)
{
    assertNotNull(keyValue, "Not a valid keyValue");
    PARCObject *oldKey = keyValue->key;
    keyValue->key = parcObject_Acquire(key);
    parcObject_Release(&oldKey);
}

PARCObject *
parcKeyValue_GetValue(PARCKeyValue *keyValue)
{
    assertNotNull(keyValue, "Not a valid keyValue");
    return keyValue->value;
}

PARCObject *
parcKeyValue_GetKey(PARCKeyValue *keyValue)
{
    assertNotNull(keyValue, "Not a valid keyValue");
    return keyValue->key;
}

bool
parcKeyValue_Equals(const PARCKeyValue *a, const PARCKeyValue *b)
{
    bool result = parcObject_Equals(a->key, b->key);
    if ((a->value == NULL) || (b->value == NULL)) {
        result &= (a->value == b->value); // Only true if both are NULL
    } else {
        result &= parcObject_Equals(a->value, b->value);
    }
    return result;
}

int
parcKeyValue_Compare(const PARCKeyValue *a, const PARCKeyValue *b)
{
    if (a == NULL && b == NULL) {
        return 0;
    }
    if (a != NULL && b == NULL) {
        return 1;
    }
    if (a == NULL && b != NULL) {
        return -1;
    } else {
        return parcObject_Compare(a->key, b->key);
    }
}

PARCHashCode
parcKeyValue_HashCode(const PARCKeyValue *keyValue)
{
    return parcObject_HashCode(keyValue->key);
}

bool
parcKeyValue_EqualKeys(const PARCKeyValue *a, const PARCKeyValue *b)
{
    return parcObject_Equals(a->key, b->key);
}
