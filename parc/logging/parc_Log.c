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
 * @author Glenn Scott, Palo Alto Research Center (Xerox PARC)
 * @copyright 2015, Xerox Corporation (Xerox)and Palo Alto Research Center (PARC).  All rights reserved.
 */
#include <config.h>

#include <stdio.h>
#include <sys/time.h>

#include <LongBow/runtime.h>

#include <parc/algol/parc_Object.h>
#include <parc/algol/parc_Memory.h>

#include <parc/logging/parc_Log.h>
#include <parc/logging/parc_LogReporter.h>

struct PARCLog {
    char *hostName;
    char *applicationName;
    char *processId;
    uint64_t messageId;
    PARCLogLevel level;
    PARCLogReporter *reporter;
};

static void
_parcLogger_Destroy(PARCLog **loggerPtr)
{
    PARCLog *logger = *loggerPtr;

    parcMemory_Deallocate((void **) &logger->hostName);
    parcMemory_Deallocate((void **) &logger->applicationName);
    parcMemory_Deallocate((void **) &logger->processId);
    parcLogReporter_Release(&logger->reporter);
}

parcObject_ExtendPARCObject(PARCLog, _parcLogger_Destroy, NULL, NULL, NULL, NULL, NULL, NULL);

static const char *_nilvalue = "-";

PARCLog *
parcLog_Create(const char *hostName, const char *applicationName, const char *processId, PARCLogReporter *reporter)
{
    if (applicationName == NULL) {
        applicationName = _nilvalue;
    }
    if (hostName == NULL) {
        hostName = _nilvalue;
    }
    if (processId == NULL) {
        processId = _nilvalue;
    }

    PARCLog *result = parcObject_CreateInstance(PARCLog);
    if (result == NULL) {
        trapOutOfMemory("Creating an instance of PARCLog.");
    }

    result->hostName = parcMemory_StringDuplicate(hostName, strlen(hostName));
    result->applicationName = parcMemory_StringDuplicate(applicationName, strlen(applicationName));
    result->processId = parcMemory_StringDuplicate(processId, strlen(processId));
    result->messageId = 0;
    result->level = PARCLogLevel_Off;
    result->reporter = parcLogReporter_Acquire(reporter);
    return result;
}

parcObject_ImplementAcquire(parcLog, PARCLog);

parcObject_ImplementRelease(parcLog, PARCLog);


PARCLogLevel
parcLog_GetLevel(const PARCLog *log)
{
    return log->level;
}

PARCLogLevel
parcLog_SetLevel(PARCLog *logger, const PARCLogLevel level)
{
    PARCLogLevel oldLevel = logger->level;
    logger->level = level;
    return oldLevel;
}

static PARCLogEntry *
_parcLog_CreateEntry(PARCLog *log, PARCLogLevel level, uint64_t messageId, const char *format, va_list ap)
{
    char *cString;
    int nwritten = vasprintf(&cString, format, ap);
    assertTrue(nwritten >= 0, "Error calling vasprintf");

    struct timeval timeStamp;
    gettimeofday(&timeStamp, NULL);

    PARCBuffer *payload = parcBuffer_AllocateCString(cString);
    PARCLogEntry *result = parcLogEntry_Create(level,
                                               log->hostName,
                                               log->applicationName,
                                               log->processId,
                                               messageId,
                                               timeStamp,
                                               payload);
    parcBuffer_Release(&payload);

    free(cString);
    return result;
}

bool
parcLog_MessageVaList(PARCLog *log, PARCLogLevel level, uint64_t messageId, const char *format, va_list ap)
{
    bool result = false;

    if (parcLog_IsLoggable(log, level)) {
        PARCLogEntry *entry = _parcLog_CreateEntry(log, level, messageId, format, ap);

        parcLogReporter_Report(log->reporter, entry);
        parcLogEntry_Release(&entry);
        result = true;
    }
    return result;
}

bool
parcLog_Message(PARCLog *log, PARCLogLevel level, uint64_t messageId, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(log, level, messageId, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Warning(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Warning, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Info(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Info, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Notice(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Notice, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Debug(PARCLog *logger, const char *format, ...)
{
    bool result = false;

    if (parcLog_IsLoggable(logger, PARCLogLevel_Debug)) {
        va_list ap;
        va_start(ap, format);
        result = parcLog_MessageVaList(logger, PARCLogLevel_Debug, 0, format, ap);
        va_end(ap);
    }

    return result;
}

bool
parcLog_Error(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Error, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Critical(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Critical, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Alert(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Alert, 0, format, ap);
    va_end(ap);

    return result;
}

bool
parcLog_Emergency(PARCLog *logger, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);
    bool result = parcLog_MessageVaList(logger, PARCLogLevel_Emergency, 0, format, ap);
    va_end(ap);

    return result;
}
