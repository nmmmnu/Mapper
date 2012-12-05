#ifndef __conf_h
#define __conf_h 1

#define VERSION		"0.95"
#define VERSION_DATE	"2012-12-04"

#define DEF_THREADS	8

// MAX_THREADS must be less than 1024, 
// because usually there are only 1024 open files limit.
#define MAX_THREADS	500

// buffer for fgets()
#define MAX_BUFFER	1024 * 16

// 
#define MIN_FILE_PART	1024 * 64


#endif
