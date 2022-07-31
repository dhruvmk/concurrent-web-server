#ifndef __headers_h__
#define __headers_h__

#ifdef __linux__
#include <semaphore.h>
#endif

#include <pthread.h>
#include <assert.h>
#include <sched.h>

#include "structures.h"

#include "request.h"
#include "io_helper.h"
#include <pthread.h>
#include <assert.h>
#include <sched.h>
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

#define pthread_create_or_die(thread, attr, start_routine, arg) assert(pthread_create(thread, attr, start_routine, arg) == 0);
#define pthread_join_or_die(thread, value_ptr)                  assert(pthread_join(thread, value_ptr) == 0);

#define pthread_mutex_init_or_die(m, v)                         assert(pthread_mutex_init(m, v) == 0);
#define pthread_mutex_lock_or_die(m)                            assert(pthread_mutex_lock(m) == 0);
#define pthread_mutex_unlock_or_die(m)                          assert(pthread_mutex_unlock(m) == 0);

#define pthread_cond_init_or_die(cond, v)                       assert(pthread_cond_init(cond, v) == 0);
#define pthread_cond_signal_or_die(cond)                        assert(pthread_cond_signal(cond) == 0);
#define pthread_cond_wait_or_die(cond, mutex)                   assert(pthread_cond_wait(cond, mutex) == 0);

#define mutex_init_or_die(m)                                    assert(pthread_mutex_init(m, NULL) == 0);
#define mutex_lock_or_die(m)                                    assert(pthread_mutex_lock(m) == 0);
#define mutex_unlock_or_die(m)                                  assert(pthread_mutex_unlock(m) == 0);

#define cond_init_or_die(cond)                                  assert(pthread_cond_init(cond, NULL) == 0);
#define cond_signal_or_die(cond)                                assert(pthread_cond_signal(cond) == 0);
#define cond_wait_or_die(cond, mutex)                           assert(pthread_cond_wait(cond, mutex) == 0);


#endif