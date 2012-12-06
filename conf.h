#ifndef __conf_h
#define __conf_h 1

#define VERSION		"0.96"
#define VERSION_DATE1	"2012-12-04"
#define VERSION_DATE	"2012-12-06"

// Default threads.
#define DEF_THREADS	8

// MAX_THREADS must be less than 500, 
// because usually there are only 1024 open files limit.
// the process usage 2 descriptors per thread
#define MAX_THREADS	500

// buffer for fgets()
#define MAX_BUFFER	1024 * 16

// minimal file part that is processed by single thread
#define MIN_FILE_PART	1024 * 64

#define PROGRESS_BAR_SIZE 1000000

#endif
