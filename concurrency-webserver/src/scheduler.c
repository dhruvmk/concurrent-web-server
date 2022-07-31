#include "headers.h"
#include "structures.h"

// Initialize a circular queue if the scheduling algorithm is FIFO
job* init_jobs_fifo(int buffer_size) {
    job* jobQueue = (job*)malloc(sizeof(job) * buffer_size);
    if (jobQueue == NULL) {
        exit(1);
    }
    for(int i = 0; i < buffer_size; i++){
        jobQueue[i].fd = 0;
    }
    return jobQueue;
}

// (To Be Implemented) Initialize a min-heap if the scheduling algorithm is SFF 
job* init_jobs_sff(int buffer_size) {
    job* jobHeap = (job*)malloc(sizeof(job) * buffer_size);
    return jobHeap;
}

// Initialize a scheduler with the appropriate jobs array (circular queue or min-heap)
scheduler* init_scheduler(char* policy, int buffer_size) {
    scheduler* s = (scheduler*)malloc(sizeof(scheduler));
    if (s == NULL) {
        return s;
    }
    s->policy = policy;
    s->curr_size = 0;
    s->buffer_size = buffer_size;
    s->rear = -1;
    s->front = 0;

    if (strcmp("FIFO", policy) == 0) {        
        s->jobs = init_jobs_fifo(buffer_size);  
    } else if (strcmp("SFF", policy) == 0) {
        s->jobs = init_jobs_sff(buffer_size);
    }
    return s;
}

// (To Be Implemented) Add a new request to the min-heap if the scheduling algorithm is SFF
void add_job_sff(scheduler* s, int conn_fd, int size) {
    return NULL;
}

// Add a new request to the circular queue if the scheduling algorithm is FIFO
void add_job_fifo(scheduler* s, int conn_fd) {
    assert(s->curr_size < s->buffer_size);

    s->rear = (s->rear + 1) % s->buffer_size;
    s->jobs[s->rear].fd = conn_fd;
    s->curr_size++;
}

// Find file size for SFF
off_t get_file_size(int conn_fd) {
    struct stat buf;
    fstat(conn_fd, &buf);
    return buf.st_size;
}

// Add a new request to the scheduler
// If the scheduler is currently full, the thread is blocked and can only add the request when the buffer frees up
void give_to_scheduler(thread_pool* t, scheduler* s, int conn_fd) {
    pthread_mutex_lock_or_die(&t->LOCK);
    while (s->curr_size == s->buffer_size) {
        pthread_cond_wait_or_die(&t->FILL, &t->LOCK);
    }
    if (strcmp(s->policy, "SFF") == 0) {
        off_t size = get_file_size(conn_fd);
        add_job_sff(s, conn_fd, size);
    }
    else {
        add_job_fifo(s, conn_fd);
    }
    pthread_cond_signal_or_die(&t->EMPTY);
    pthread_mutex_unlock_or_die(&t->LOCK);
}

// (To Be Implemented) Fetches a request from the min-heap
int get_job_sff(scheduler* s) {
    return 0;
}

// Fetches a request from the top of the queue
int get_job_fifo(scheduler* s) {
    int conn_fd = s->jobs[s->front].fd;
    s->front = (s->front + 1) % s->buffer_size;
    s->curr_size--;

    return conn_fd;
}

// Retrieves a request from the scheduler
// If the scheduler is currently empty, the worker thread waits until a request comes in
int get_from_scheduler(thread_pool* t, scheduler* s) {
    pthread_mutex_lock_or_die(&t->LOCK);
    while (s->curr_size == 0) {
        pthread_cond_wait_or_die(&t->EMPTY, &t->LOCK);
    }
    int conn_fd;
    if (strcmp(s->policy, "SFF") == 0) {
        conn_fd = get_job_sff(s);
    }
    else {
        conn_fd = get_job_fifo(s);
    }

    pthread_cond_signal_or_die(&t->FILL);
    pthread_mutex_unlock_or_die(&t->LOCK);

    return conn_fd;
}

