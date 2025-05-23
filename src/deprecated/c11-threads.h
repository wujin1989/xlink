/** Copyright (c) 2025, Wu Jin <wujin.developer@gmail.com>
 *
 *  Permission is hereby granted, free of charge, to any person obtaining a copy
 *  of this software and associated documentation files (the "Software"), to
 *  deal in the Software without restriction, including without limitation the
 *  rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 *  sell copies of the Software, and to permit persons to whom the Software is
 *  furnished to do so, subject to the following conditions:
 *
 *  The above copyright notice and this permission notice shall be included in
 *  all copies or substantial portions of the Software.
 *
 *  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 *  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 *  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 *  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 *  IN THE SOFTWARE.
 */

_Pragma("once")

#include <time.h>
#include <stdlib.h>

#if defined(__linux__) || defined(__APPLE__)
#if defined(__APPLE__)

#include <errno.h>
#include <sched.h>
#include <pthread.h>

#define ONCE_FLAG_INIT PTHREAD_ONCE_INIT
#define thread_local __thread

enum {
    thrd_success = 0,
    thrd_busy = 1,
    thrd_error = 2,
    thrd_nomem = 3,
    thrd_timedout = 4,
};

enum {
    mtx_plain = 0,
    mtx_recursive = 1,
    mtx_timed = 2,
};

typedef pthread_cond_t  cnd_t;
typedef pthread_t       thrd_t;
typedef pthread_key_t   tss_t;
typedef pthread_mutex_t mtx_t;
typedef pthread_once_t  once_flag;

typedef struct thrdctx_s thrdctx_t;
typedef struct oncectx_s oncectx_t;

typedef int (*thrd_start_t)(void*);
typedef void (*tss_dtor_t)(void*);

struct thrdctx_s {
    thrd_start_t func;
    void*        arg;
};

struct oncectx_s {
    void (*func)(void);
};

static inline void* _thrd_start(void* arg) {
    thrdctx_t* pctx;
    thrdctx_t  ctx;

    pctx = arg;
    ctx = *pctx;

    free(pctx);
    pctx = NULL;
    return (void*)(intptr_t)ctx.func(ctx.arg);
}

static inline int thrd_create(thrd_t* thr, thrd_start_t func, void* arg) {
    thrdctx_t* pctx;
    pctx = malloc(sizeof(thrdctx_t));
    if (!pctx) {
        return thrd_nomem;
    }
    pctx->func = func;
    pctx->arg = arg;

    if (pthread_create(thr, NULL, _thrd_start, pctx)) {
        free(pctx);
        pctx = NULL;
        return thrd_error;
    }
    return thrd_success;
}

static inline thrd_t thrd_current(void) {
    return pthread_self();
}

static inline int thrd_detach(thrd_t thr) {
    return (pthread_detach(thr) == 0) ? thrd_success : thrd_error;
}

static inline int thrd_equal(thrd_t thr0, thrd_t thr1) {
    return pthread_equal(thr0, thr1);
}

static inline _Noreturn void thrd_exit(int res) {
    pthread_exit((void*)(intptr_t)res);
}

static inline int thrd_join(thrd_t thr, int* res) {
    void* code;
    if (pthread_join(thr, &code) != 0) {
        return thrd_error;
    }
    if (res) {
        *res = (int)(intptr_t)code;
    }
    return thrd_success;
}

static inline int
thrd_sleep(const struct timespec* duration, struct timespec* remaining) {
    int ret = nanosleep(duration, remaining);
    if (ret < 0) {
        if (errno == EINTR) {
            return -1;
        } else {
            return -2;
        }
    }
    return 0;
}

static inline void thrd_yield(void) {
    sched_yield();
}

static inline int tss_create(tss_t* key, tss_dtor_t dtor) {
    return (pthread_key_create(key, dtor) == 0) ? thrd_success : thrd_error;
}

static inline void tss_delete(tss_t key) {
    pthread_key_delete(key);
}

static inline void* tss_get(tss_t key) {
    return pthread_getspecific(key);
}

static inline int tss_set(tss_t key, void* val) {
    return (pthread_setspecific(key, val) == 0) ? thrd_success : thrd_error;
}

static inline int mtx_init(mtx_t* mtx, int type) {
    pthread_mutexattr_t attr;

    if (type != (mtx_plain) && type != (mtx_timed) &&
        type != (mtx_plain | mtx_recursive) &&
        type != (mtx_timed | mtx_recursive)) {
        return thrd_error;
    }
    if ((type & mtx_recursive) == 0) {
        pthread_mutex_init(mtx, NULL);
        return thrd_success;
    }
    pthread_mutexattr_init(&attr);
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
    pthread_mutex_init(mtx, &attr);
    pthread_mutexattr_destroy(&attr);
    return thrd_success;
}

static inline void mtx_destroy(mtx_t* mtx) {
    pthread_mutex_destroy(mtx);
}

static inline int mtx_lock(mtx_t* mtx) {
    return (pthread_mutex_lock(mtx) == 0) ? thrd_success : thrd_error;
}

static inline int mtx_trylock(mtx_t* mtx) {
    return (pthread_mutex_trylock(mtx) == 0) ? thrd_success : thrd_busy;
}

static inline int
mtx_timedlock(mtx_t* restrict mtx, const struct timespec* restrict ts) {
    time_t expire = time(NULL);
    expire += ts->tv_sec;

    while (mtx_trylock(mtx) != thrd_success) {
        time_t now = time(NULL);
        if (expire < now) {
            return thrd_busy;
        }
        thrd_yield();
    }
    return thrd_success;
}

static inline int mtx_unlock(mtx_t* mtx) {
    return (pthread_mutex_unlock(mtx) == 0) ? thrd_success : thrd_error;
}

static inline int cnd_wait(cnd_t* cond, mtx_t* mtx) {
    return (pthread_cond_wait(cond, mtx) == 0) ? thrd_success : thrd_error;
}

static inline int cnd_timedwait(
    cnd_t* restrict cond,
    mtx_t* restrict mtx,
    const struct timespec* restrict ts) {
    int ret;

    ret = pthread_cond_timedwait(cond, mtx, ts);
    if (ret == ETIMEDOUT) {
        return thrd_timedout;
    }
    return (ret == 0) ? thrd_success : thrd_error;
}

static inline int cnd_signal(cnd_t* cond) {
    return (pthread_cond_signal(cond) == 0) ? thrd_success : thrd_error;
}

static inline int cnd_init(cnd_t* cond) {
    return (pthread_cond_init(cond, NULL) == 0) ? thrd_success : thrd_error;
}

static inline void cnd_destroy(cnd_t* cond) {
    pthread_cond_destroy(cond);
}

static inline int cnd_broadcast(cnd_t* cond) {
    return (pthread_cond_broadcast(cond) == 0) ? thrd_success : thrd_error;
}

static inline void call_once(once_flag* flag, void (*func)(void)) {
    pthread_once(flag, func);
}
#endif

#if defined(__linux__)
#include <threads.h>
#endif
#endif

#if defined(_WIN32)
#include <threads.h>
#endif