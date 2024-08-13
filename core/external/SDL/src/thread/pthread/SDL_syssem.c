/*
  Simple DirectMedia Layer
  Copyright (C) 1997-2024 Sam Lantinga <slouken@libsdl.org>

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
*/
#include "SDL_internal.h"

#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <sys/time.h>
#include <time.h>

/* Wrapper around POSIX 1003.1b semaphores */

#if defined(SDL_PLATFORM_MACOS) || defined(SDL_PLATFORM_IOS)
/* macOS doesn't support sem_getvalue() as of version 10.4 */
#include "../generic/SDL_syssem.c"
#else

struct SDL_Semaphore
{
    sem_t sem;
};

/* Create a semaphore, initialized with value */
SDL_Semaphore *SDL_CreateSemaphore(Uint32 initial_value)
{
    SDL_Semaphore *sem = (SDL_Semaphore *)SDL_malloc(sizeof(SDL_Semaphore));
    if (sem) {
        if (sem_init(&sem->sem, 0, initial_value) < 0) {
            SDL_SetError("sem_init() failed");
            SDL_free(sem);
            sem = NULL;
        }
    }
    return sem;
}

void SDL_DestroySemaphore(SDL_Semaphore *sem)
{
    if (sem) {
        sem_destroy(&sem->sem);
        SDL_free(sem);
    }
}

int SDL_WaitSemaphoreTimeoutNS(SDL_Semaphore *sem, Sint64 timeoutNS)
{
    int retval = 0;
#ifdef HAVE_SEM_TIMEDWAIT
#ifndef HAVE_CLOCK_GETTIME
    struct timeval now;
#endif
    struct timespec ts_timeout;
#else
    Uint64 end;
#endif

    if (!sem) {
        return SDL_InvalidParamError("sem");
    }

    /* Try the easy cases first */
    if (timeoutNS == 0) {
        retval = SDL_MUTEX_TIMEDOUT;
        if (sem_trywait(&sem->sem) == 0) {
            retval = 0;
        }
        return retval;
    }
    if (timeoutNS < 0) {
        do {
            retval = sem_wait(&sem->sem);
        } while (retval < 0 && errno == EINTR);

        if (retval < 0) {
            retval = SDL_SetError("sem_wait() failed");
        }
        return retval;
    }

#ifdef HAVE_SEM_TIMEDWAIT
    /* Setup the timeout. sem_timedwait doesn't wait for
     * a lapse of time, but until we reach a certain time.
     * This time is now plus the timeout.
     */
#ifdef HAVE_CLOCK_GETTIME
    clock_gettime(CLOCK_REALTIME, &ts_timeout);

    /* Add our timeout to current time */
    ts_timeout.tv_sec += (timeoutNS / SDL_NS_PER_SECOND);
    ts_timeout.tv_nsec += (timeoutNS % SDL_NS_PER_SECOND);
#else
    gettimeofday(&now, NULL);

    /* Add our timeout to current time */
    ts_timeout.tv_sec = now.tv_sec + (timeoutNS / SDL_NS_PER_SECOND);
    ts_timeout.tv_nsec = SDL_US_TO_NS(now.tv_usec) + (timeoutNS % SDL_NS_PER_SECOND);
#endif

    /* Wrap the second if needed */
    while (ts_timeout.tv_nsec >= 1000000000) {
        ts_timeout.tv_sec += 1;
        ts_timeout.tv_nsec -= 1000000000;
    }

    /* Wait. */
    do {
        retval = sem_timedwait(&sem->sem, &ts_timeout);
    } while (retval < 0 && errno == EINTR);

    if (retval < 0) {
        if (errno == ETIMEDOUT) {
            retval = SDL_MUTEX_TIMEDOUT;
        } else {
            SDL_SetError("sem_timedwait returned an error: %s", strerror(errno));
        }
    }
#else
    end = SDL_GetTicksNS() + timeoutNS;
    while (sem_trywait(&sem->sem) != 0) {
        if (SDL_GetTicksNS() >= end) {
            retval = SDL_MUTEX_TIMEDOUT;
            break;
        }
        SDL_DelayNS(100);
    }
#endif /* HAVE_SEM_TIMEDWAIT */

    return retval;
}

Uint32 SDL_GetSemaphoreValue(SDL_Semaphore *sem)
{
    int ret = 0;

    if (!sem) {
        SDL_InvalidParamError("sem");
        return 0;
    }

    sem_getvalue(&sem->sem, &ret);
    if (ret < 0) {
        ret = 0;
    }
    return (Uint32)ret;
}

int SDL_SignalSemaphore(SDL_Semaphore *sem)
{
    int retval;

    if (!sem) {
        return SDL_InvalidParamError("sem");
    }

    retval = sem_post(&sem->sem);
    if (retval < 0) {
        SDL_SetError("sem_post() failed");
    }
    return retval;
}

#endif /* SDL_PLATFORM_MACOS */
