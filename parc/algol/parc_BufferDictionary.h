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
 * @file parc_BufferDictionary.h
 * @ingroup datastructures
 * @brief A key/value dictionary built around PARCBuffer as the Key and the Value.
 *
 * The key/value dictionary models the Java MAP interface.  It is built around Put, Get, and Remove.
 * The dictionary stores references to the Key and Value, so the caller may destroy its references
 * if no longer needed.
 *
 * @author Marc Mosko, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */

#ifndef libparc_parc_BufferDictionary_h
#define libparc_parc_BufferDictionary_h

#include <parc/algol/parc_Buffer.h>

typedef struct parc_buffer_dictionary PARCBufferDictionary;

/**
 * Creates an empty dictionary.  You must use {@link parcBufferDictionary_Release} to destroy it.
 *
 * @return A pointer to the new `PARCBufferDictionary`
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBufferDictionary *parcBufferDictionary_Create(void);

/**
 * Increase the number of references to a `PARCBufferDictionary`.
 *
 * Note that a new `PARCBufferDictionary` is not created,
 * only that the given `PARCBufferDictionary` reference count is incremented.
 * Discard the reference by invoking {@link parcBufferDictionary_Release}.
 *
 * @param [in] dictionary is a pointer to a `PARCBufferDictionary` instance
 * @return The pointer to the @p dictionary instance.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBufferDictionary *parcBufferDictionary_Acquire(const PARCBufferDictionary *dictionary);

/**
 * Release a `PARCBufferDictionary` reference.
 *
 * Only the last invocation where the reference count is decremented to zero,
 * will actually destroy the `PARCBufferDictionary`.
 *
 * @param [in,out] dictionaryPtr is a pointer to the `PARCBufferDictionary` reference.
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
void parcBufferDictionary_Release(PARCBufferDictionary **dictionaryPtr);

/**
 * Add a key/value to the dictionary, returns previous value or NULL
 *
 * The Dictionary will store a reference (PARCBuffer::aquire) to the key and to the value.
 * The Key and Value must be non-NULL.  If a previous entry for the key is in the dictionary,
 * the previous value is returned.  THE CALLER MUST USE {@link parcBuffer_Release} on the returned
 * value if it is non-NULL;
 *
 * @param [in,out] dictionary The dictionary to modify
 * @param [in] key The dictionary key
 * @param [in] value The value for the key
 *
 * @return NULL The inserted key is unique
 * @return non-NULL Returns the previous value of the key, must use {@link parcBuffer_Release}
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBuffer *parcBufferDictionary_Put(PARCBufferDictionary *dictionary, PARCBuffer *key, PARCBuffer *value);

/**
 * Returns the value associated with the key, or NULL if does not exist
 *
 * The returned value is what's stored in the dictionary in a `PARCBuffer` instance.  If the user wishes to keep the
 * value beyond the current calling scope, she should use {@link parcBuffer_Acquire} on the
 * returned value.
 *
 * @param [in] dictionary The dictionary to query
 * @param [in] key The dictionary key
 *
 * @return NULL The key is not in the dictionary
 * @return non-NULL Returns the current value of the key, DO NOT use {@link parcBuffer_Release}
 *
 * Example:
 * @code
 * <#example#>
 * @endcode
 */
PARCBuffer *parcBufferDictionary_Get(PARCBufferDictionary *dictionary, PARCBuffer *key);

/**
 * Removes a key from the dictionary, returning the current value or NULL.  The caller MUST
 * call {@link parcBuffer_Release} on the returned value.
 *
 * @param [in,out] dictionary The dictionary to modify
 * @param [in] key The dictionary key
 *
 * @return NULL The inserted key is not in the dictionary
 * @return non-NULL Returns the current value of the key, DO NOT use {@link parcBuffer_Release}
 *
 * Example:
 * @code
 * {
 *     uint8_t phone[] = "6505551212";
 *     PARCBufferDictionary *dict = parcBufferDictionary_Create();
 *
 *     parcBuffer *key = parcBuffer_Wrap(phone, sizeof(phone), 0);
 *     parcBuffer_Release(parcBufferDictionary_Remove(dict, key));
 *
 *     parcBuffer_Release(&key);
 *     parcBufferDictionary_Destroy(&dict);
 * }
 * @endcode
 */
PARCBuffer *parcBufferDictionary_Remove(PARCBufferDictionary *dictionary, PARCBuffer *key);
#endif // libparc_parc_BufferDictionary_h
