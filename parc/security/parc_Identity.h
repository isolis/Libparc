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
 * @file parc_Identity.h
 * @ingroup security
 * @brief A generic cryptographic identity that is assigned to an entity
 * (user, group, process) and is associated with a set of cryptographic
 * material, e.g., public and private keys.
 *
 * Identities are used for authentication and authorization purposes.
 * To illustrate their use, consider the following model. Digital signatures
 * are computed with a private key owned by some entity. This private
 * key is associated with an identity. It is said that the digital signature
 * in this case was procured by an entity with the identity associated
 * with the private key. Moreover, verifying this digital signature with
 * the corresponding public key is analogous to verifying that the signature
 * was generated by an entity with the corresponding identity.
 *
 * The relationship between identities and entities means that an entity may have
 * multiple identities, each of which is associated with its own set of cryptographic
 * information.
 *
 * Finally, an identity is typically backed by a file which stores the set of
 * cryptographic material. For instance, once an identity may be represented as a
 * PKCS12 (public and private) key store. Other concrete identity implementations
 * may have different backing stores (i.e., not files, but services) with
 * different notions of secret passwords.
 *
 * @author Glenn Scott, Christopher A. Wood, Palo Alto Research Center (Xerox PARC)
 * @copyright 2013-2014, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#ifndef libparc_parc_Identity_h
#define libparc_parc_Identity_h

#include <parc/security/parc_Signer.h>

struct parc_identity;
typedef struct parc_identity PARCIdentity;

typedef struct parc_identity_interface {
    /**
     * @see parcIdentity_Acquire
     */
    PARCIdentity *(*Acquire)(void *identity);

    /**
     * @see parcIdentity_Release
     */
    void (*Release)(void **identityPtr);

    /**
     * @see parcIdentity_GetPassWord
     */
    void *(*GetPassWord)(const void *original);

    /**
     * @see parcIdentity_GetFileName
     */
    void *(*GetFileName)(const void *original);

    /**
     * @see parcIdentity_GetSigner
     */
    PARCSigner *(*GetSigner)(const void *identity);

    /**
     * @see parcIdentity_Equals
     */
    bool (*Equals)(const void *a, const void *b);

    /**
     * @see `parcIdentity_Display`
     */
    void (*Display)(const void *identity, size_t indentation);
} PARCIdentityInterface;

#ifdef PARCLibrary_DISABLE_VALIDATION
#  define parcIdentity_OptionalAssertValid(_instance_)
#else
#  define parcIdentity_OptionalAssertValid(_instance_) parcIdentity_AssertValid(_instance_)
#endif

/**
 * Determine if an instance of `PARCIdentity` is valid.
 *
 * Valid means the internal state of the type is consistent with its required current or future behaviour.
 * This may include the validation of internal instances of types.
 *
 * @param [in] identity A pointer to a `PARCIdentity` instance.
 *
 * @return true The instance is valid.
 * @return false The instance is not valid.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *instance = parcIdentity_Create();
 *
 *     if (parcIdentity_IsValid(instance)) {
 *         printf("Instance is valid.\n");
 *     }
 * }
 * @endcode
 */
bool parcIdentity_IsValid(const PARCIdentity *identity);

/**
 * Assert that the given `PARCIdentity` instance is valid.
 *
 * @param [in] identity A pointer to a valid PARCIdentity instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *a = parcIdentity_Create();
 *
 *    parcIdentity_AssertValid(a);
 *
 *     printf("Instance is valid.\n");
 *
 *     parcIdentity_Release(&b);
 * }
 * @endcode
 */
void parcIdentity_AssertValid(const PARCIdentity *identity);

/**
 * Create an instance of PARCIdentity from the given pointer to a subtype
 * and the subtype's `PARCIdentityInterface` instance.
 *
 * A new reference to @p instance is acquired.
 *
 * @param [in] instance A pointer to a suitable subtype of `PARCIdentity`.
 * @param [in] interface A poitner to the subtype's `PARCIdentityInterface` instance.
 *
 * @return NULL Memory could not be allocated.
 * @return non-NULL A pointer to a `PARCIdentity` instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 */
PARCIdentity *parcIdentity_Create(void *instance, const PARCIdentityInterface *interface);

/**
 * Increase the number of references to a `PARCIdentity` instance.
 *
 * Note that new `PARCIdentity` is not created,
 * only that the given `PARCIdentity` reference count is incremented.
 * Discard the reference by invoking `parcIdentity_Release`.
 *
 * @param [in] identity A pointer to the original instance.
 * @return The value of the input parameter @p instance.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *     PARCIdentity *i2 = parcIdentity_Acquire(identity);
 *     // use both as needed
 *     parcIdentity_Release(&i2);
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 *
 * @see parcIdentity_Release
 */
PARCIdentity *parcIdentity_Acquire(const PARCIdentity *identity);

/**
 * Release a previously acquired reference to the specified instance,
 * decrementing the reference count for the instance.
 *
 * The pointer to the instance is set to NULL as a side-effect of this function.
 *
 * If the invocation causes the last reference to the instance to be released,
 * the instance is deallocated and the instance's implementation will perform
 * additional cleanup and release other privately held references.
 *
 * @param [in,out] identityPtr A pointer to a pointer to the instance to release.
 *
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 */
void parcIdentity_Release(PARCIdentity **identityPtr);

/**
 * Retrieve the file name associated with this identity.
 *
 * In the case of an identity file, this is the actual file name.
 * Other concrete identity implementations may have different notions of secret passwords.
 *
 * NOTE: This function is set to be removed from the PARCIdentity API.
 *
 * @param [in] identity A `PARCIdentity` instance.
 *
 * @return A nul-terminated string containing the file name.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *     char *fileName = parcIdentity_GetFileName(identity);
 *     // use the filename
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 */
const char *parcIdentity_GetFileName(const PARCIdentity *identity);

/**
 * Retrieve the secret password associated with this identity..
 *
 * In the case of an identity file, the password will be one that opens the file for access.
 * Other concrete identity implementations may have different notions of secret passwords.
 *
 * NOTE: This function is set to be removed from the PARCIdentity API.
 *
 * @param [in] identity A `PARCIdentity` instance.
 *
 * @return A character array containing the identity password.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *     char *pw = parcIdentity_GetPassWord(identity);
 *     // use the password pw
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 */
const char *parcIdentity_GetPassWord(const PARCIdentity *identity);

/**
 * Create an instance of `PARCSigner` from the given `PARCIdentity`.
 *
 * The `PARCSigner` instance must be released via `parcSignature_Release()`.
 *
 * @param [in] identity A pointer to a PARCIdentity instance.
 *
 * @return PARCSigner A newly allocated `PARCSigner` instance based off this identity.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *identity = parcIdentity_Create(...);
 *     PARCSigner *signer = parcIdentity_GetSigner(identity);
 *
 *     // use the signer as needed...
 *
 *     parcSigner_Release(&signer);
 *     parcIdentity_Release(&identity);
 * }
 * @endcode
 */
PARCSigner *parcIdentity_GetSigner(const PARCIdentity *identity);

/**
 * Determine if two PARCIdentity are equal.
 *
 * The following equivalence relations on non-null `XXX` instances are maintained: *
 *   * It is reflexive: for any non-null reference value x, parcIdentity_Equals(x, x) must return true.
 *
 *   * It is symmetric: for any non-null reference values x and y, PARCIdentity_Equals(x, y) must return true if and only if
 *        parcIdentity_Equals(y x) returns true.
 *
 *   * It is transitive: for any non-null reference values x, y, and z, if
 *        parcIdentity_Equals(x, y) returns true and
 *        parcIdentity_Equals(y, z) returns true,
 *        then  parcIdentity_Equals(x, z) must return true.
 *
 *   * It is consistent: for any non-null reference values x and y, multiple invocations of parcIdentity_Equals(x, y)
 *         consistently return true or consistently return false.
 *
 *   * For any non-null reference value x, parcIdentity_Equals(x, NULL)) must return false.
 *
 * @param a A pointer to a PARCIdentity instance.
 * @param b A pointer to a PARCIdentity instance.
 * @return True if the referenced PARCIdentity are equal.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *a = parcIdentity_Create(...);
 *     PARCIdentity *b = parcIdentity_Create(...);
 *     parcIdentity_Equals(a, b)
 *     if (parcIdentity_Equals(a, b)) {
 *         // this is expected
 *     } else {
 *         // this is not expected
 *     }
 *     parcIdentity_Release(&a);
 *     parcIdentity_Release(&b);
 * }
 * @endcode
 */
bool parcIdentity_Equals(const PARCIdentity *a, const PARCIdentity *b);

/**
 * Print a human readable representation of the given `PARCIdentity`.
 *
 * @param [in] identity A pointer to the instance to display.
 * @param [in] indentation The level of indentation to use to pretty-print the output.
 *
 * Example:
 * @code
 * {
 *     PARCIdentity *instance = parcIdentity_Create(...);
 *
 *     parcIdentity_Display(instance, 0);
 *
 *     parcIdentity_Release(&instance);
 * }
 * @endcode
 */
void parcIdentity_Display(const PARCIdentity *identity, int indentation);
#endif // libparc_parc_Identity_h
