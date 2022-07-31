#include "headers.h"
#include "structures.h"

// Initiakizes a thread pool
// Creates each thread iteratively and binds them to the thread_worker function
// Passes the scheduler and thread pool as arguments into this function
thread_pool* init_thread_pool(scheduler* s, int threads) {
    thread_pool* t = (thread_pool*)malloc(sizeof(thread_pool));
    if (t == NULL) {
        return t;
    }
    t->num_threads = threads;
    t->working_threads = 0;
    t->pool = malloc(sizeof(pthread_t) * threads);

    pthread_mutex_init_or_die(&t->LOCK, NULL);
    pthread_cond_init_or_die(&t->FILL, NULL);
    pthread_cond_init_or_die(&t->EMPTY, NULL);

    for (int i = 0; i < t->num_threads; i++) {
        thread_arg* arg = (thread_arg*)malloc(sizeof(thread_arg));
        if(arg == NULL) {
            printf("Thread number %s: Creation failed", i);
            continue;
        }
        arg->thread_pool = t;
        arg->scheduler = s;
        pthread_create_or_die(&t->pool[i], NULL, thread_worker, arg);
    }

    return t;
}

// Each thread retrieves a request from the scheduler, handles it, then closes the connection
void* thread_worker(void* arg) {
    thread_arg* args = (thread_arg*)arg;
	while(1)
	{	
		int conn_fd = get_from_scheduler(args->thread_pool, args->scheduler);	
		request_handle(conn_fd);
		close_or_die(conn_fd);
	}
	return NULL;
}