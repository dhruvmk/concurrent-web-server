#include <stdio.h>
#include "request.h"
#include "io_helper.h"
#include "headers.h"
#include "structures.h"

char default_root[] = ".";

//
// ./wserver [-d <basedir>] [-p <portnum>] 
// 
int main(int argc, char *argv[]) {
    int c;
    char *root_dir = default_root;
    int port = 10000;
	int threads = 1;
	int buffers = 1;
	char* schedalg = 'FIFO';
    
    while ((c = getopt(argc, argv, "d:p:t:b:s:")) != -1)
	switch (c) {
	case 'd':
	    root_dir = optarg;
	    break;
	case 'p':
	    port = atoi(optarg);
	    break;
	case 't':
		threads = atoi(optarg);
		break;
	case 'b':
		buffers = atoi(optarg);
		break;
	case 's':
		schedalg = optarg;
		if (strcmp("FIFO", optarg) != 0 && strcmp("SFF", optarg) != 0) {
			fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b Buffer Size] [-s FIFO or SFF]\n");
			exit(1);
		}
		break;
	default:
	    fprintf(stderr, "usage: wserver [-d basedir] [-p port] [-t threads] [-b Buffer Size] [-s FIFO or SFF]\n");
	    exit(1);
	}

    // run out of this directory
    chdir_or_die(root_dir);

	// Initalize scheduler to hold and serve new requests according to the chosen scheduling algorithm
	scheduler* scheduler = init_scheduler(schedalg, buffers);

	// Initialize thread pool to pick up requests from the scheduler
	thread_pool* thread_pool = init_thread_pool(scheduler, threads);


    // now, get to work
    int listen_fd = open_listen_fd_or_die(port);
    while (1) {
		struct sockaddr_in client_addr;
		int client_len = sizeof(client_addr);
		int conn_fd = accept_or_die(listen_fd, (sockaddr_t *) &client_addr, (socklen_t *) &client_len);

		// Give the request to the scheduler to store
		give_to_scheduler(thread_pool, scheduler, conn_fd);
    }
    return 0;
}


    


 
