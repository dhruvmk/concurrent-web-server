#ifndef __structures_h__
#define __structures_h__

// Constants
#define MAXBUF 16384
#define MAXFILETYPE 8192

// Structure of each job in the scheduler
typedef struct __job_t {

    int fd;
    off_t parameter;

} job;

// Thread pool structure
typedef struct __thread_pool_t {

    int num_threads;
    int working_threads;
    pthread_t* pool;
    pthread_mutex_t LOCK;
    pthread_cond_t FILL;
    pthread_cond_t EMPTY;

} thread_pool;

// Scheduler structure
typedef struct __scheduler_t {

    char* policy;
    int buffer_size;
    int curr_size;
    int front;
    int rear;
    job* jobs;

} scheduler;

// Arguments passed to workder threads
typedef struct __thread_arg {

    scheduler* scheduler;
    thread_pool* thread_pool;

} thread_arg;

// Scheduler methods
scheduler* init_scheduler(char* policy, int buffer_size);
job* init_jobs_fifo(int buffer_size);
job* init_jobs_sff(int buffer_size);
int pick_request(scheduler* s);
void give_to_scheduler(thread_pool* t, scheduler* s, int conn_fd);
int get_from_scheduler(thread_pool* t, scheduler* s);
int get_job_sff(scheduler* s);
int get_job_fifo(scheduler* s);
off_t get_file_size(int fd);

// Thread Pool methods
thread_pool* init_thread_pool(scheduler* s, int num_threads);
void* thread_worker(void* arg);

#endif