# Overview

In this project, I built a concurrent web server using the sockets interface and POSIX threads. The base (single-threaded) server was obtained from [OSTEP](https://github.com/remzi-arpacidusseau/ostep-projects/tree/master/concurrency-webserver) and I implemented thread pools and schedulers to handle requests concurrently. Another resource used was [Computer Systems: A Programmer's Perspective] (https://csapp.cs.cmu.edu)..

# Features
- A scheduler with a fixed buffer size is used to store new requests in accordance with the chosen scheduling algorithm
- Two scheduling algorithms (First-In-First-Out and Shortest-File-First)
- A circular queue is implemented if the policy is FIFO (First-In-First-Out)
- A thread pool of fixed size is used to fetch requests from the scheduler and handle them
- Worker thread synchronization using mutex locks
- Condition variables to synchronize access to the scheduling buffer by the master and worker threads

# To Do
- Implement an efficient heap structure to handle insertions and removal of requests from the scheduler in log(n) time
- Make the web client mutlithreaded to test the web server

# Source Code Overview

- [`wserver.c`]: Contains `main()` for the web server and the basic serving loop. Initializes the thread pool and scheduler and continuously adds new requests to the scheduler
- [`request.c`]: Performs most of the work for handling requests in the basic
  web server.
- [`io_helper.h`] and [`io_helper.c`]: Contains wrapper functions for the system calls invoked by
  the basic web server and client. The convention is to add `_or_die` to an
  existing call to provide a version that either succeeds or exits. For
  example, the `open()` system call is used to open a file, but can fail for a
  number of reasons. The wrapper, `open_or_die()`, either successfully opens a
  file or exists upon failure.
- [`headers.h`]: Contains common headers and wrappers for the thread functions used: `pthread_create()`, `pthread_mutex_init()`,
`pthread_mutex_lock()`, `pthread_mutex_unlock()`, `pthread_cond_init()`,
`pthread_cond_wait()`, `pthread_cond_signal()`.
- [`structures.h`]: Contains type definitions for the thread pool, scheduler, job, and thread argument structs
- [`scheduler.c`]: Contains functions to initialize a scheduler, add requests, and get requests from the scheduler
- [`threads.c`]: Contains the method to initialize a thread pool and the thread worker function
- [`wclient.c`]: Simple single-threaded web client
- [`spin.c`]: A simple CGI program. Basically, it spins for a fixed amount
  of time.  
- [`Makefile`]
