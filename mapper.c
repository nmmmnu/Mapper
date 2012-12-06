#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#include "conf.h"
#include "split.h"



void *thread_function(void *);
char *trimr(char *);
void print_usage(const char *);



pthread_mutex_t		mutex1		= PTHREAD_MUTEX_INITIALIZER;
unsigned long int	total_lines	= 0;
const char		*filename;
const char 		*user_process;
unsigned short int	max_threads 	= DEF_THREADS;

unsigned long int	map[MAX_THREADS];



#define use_stdin strcmp(filename, "-")



int main(const int argc, const char *argv[]){

	// Check argv[]
	
	if (argc < 1 + 2){
		print_usage(argv[0]);
		exit(100);
	}

	filename = argv[1];

	user_process = argv[2];
	
	if (argc >= 1 + 3){
		max_threads = atoi(argv[3]);

		if (max_threads < 1)
			max_threads = DEF_THREADS;
			
		if (max_threads > MAX_THREADS)
			max_threads = MAX_THREADS;
	}
	

	
	// Dump some info
	
	fprintf(stderr, "%-15s : %s\n", "Input file",	filename	);
	fprintf(stderr, "%-15s : %s\n", "User process",	user_process	);

	fprintf(stderr, "\n");



	if ( use_stdin ){
		// Split input file and determine number of threads

		split(filename, max_threads, MIN_FILE_PART, map);

		fprintf(stderr, "Threads dump\n");
		
		unsigned short int j;
		for (j = 0; j < max_threads; j++){
			if (! map[j + 1])
				break;

			fprintf(stderr, "\t%04d @ %10ld %10ld\n", j, map[j], map[j + 1]);
		}
			
		max_threads = j;
	}



	fprintf(stderr, "%-15s : %d\n", "Total threads", max_threads	);
	
	fprintf(stderr, "\n");



	// Prepare and start threads

	pthread_t thread_id[MAX_THREADS];
	int            data[MAX_THREADS];
		
	unsigned short int i;
	for (i=0; i < max_threads; i++){
		data[i] = i;
		
		pthread_create( &thread_id[i], NULL, thread_function, (void *) & data[i] );

		sleep(rand() % 2);
	}
	
	for (i=0; i < max_threads; i++){
		pthread_join( thread_id[i], NULL );
		fprintf(stderr, "%4d more thread to go...\n", (max_threads - i - 1) );
	}
	
	
	
	// Dump some info
	
	fprintf(stderr, "\n");
	fprintf(stderr, "Total lines %ld.\n", total_lines);
	
	return 0;
}

void *thread_function(void *data){
	const int no = *( (int *) data );
	
	char buff[MAX_BUFFER];
	
	unsigned long int lcounter   = 0;
	unsigned long int lfilesize  = 0;

	FILE *F = stdin;
	
	if ( use_stdin ){
		F = fopen(filename, "r");

		if (! F){
			fprintf(stderr, "ERROR: Can not open the input file %s...\n", filename);
			return 0;
		}
		
		fseek(F, map[no], SEEK_SET);
	}


	FILE *P = popen(user_process, "w");
	
	if (! P){
		fprintf(stderr, "ERROR: Can not open the user process %s...\n", user_process);
		return 0;
	}



	fprintf(stderr, "Thread %04d started...\n", no);
	
	
	
	unsigned long int filesize = use_stdin ? map[no + 1] - map[no] : 0 ;
	
	
	// feof is not thread safe...
	while(! feof(F)){
		const char *data = fgets(buff, MAX_BUFFER, F);
				
		if (data)
			fputs(data, P);

		if (data == NULL)
			break;



		lcounter++;



		// Counting % stuff

		lfilesize  = lfilesize  + strlen(data);

		if (lcounter % PROGRESS_BAR_SIZE == 0){
			// calc percent finished...
			
			if (filesize > 0){
				double pr = ((double)lfilesize) / filesize * 100;
			
				fprintf(stderr, "Thread %04d, Mapping %6.2f %% done (%10ld lines)...\n", no, pr, lcounter);
			}else{
				fprintf(stderr, "Thread %04d, Mapping %12ld bytes done (%10ld lines)...\n", no, lfilesize, lcounter);
			}
		}
		
		
		
		if ( use_stdin )
			if (ftell(F) >= map[no + 1])
				break;

		// pthread_self()
	}



	// Close files
	if ( use_stdin )
		fclose(F);

	pclose(P);



	// Update grand count
	
	pthread_mutex_lock( &mutex1 );
	total_lines = total_lines + lcounter;
	pthread_mutex_unlock( &mutex1 );



	// Return
	
	fprintf(stderr, "Thread %04d finished, %10ld lines read...\n", no, lcounter);
	
	return NULL;
}

/*
 * Print help information...
 */
void print_usage(const char *argv0){
	printf("Micro Mapper v.%s\n", VERSION);
	printf("Copyleft %s, Nikolay Mihaylov <nmmm@nmmm.nu>\n", VERSION_DATE);
	printf("Build on %s, %s\n", __DATE__, __TIME__);
	
	printf("\n");
	
	printf("Usage:\n");
	printf("\t%s [input_filename] [mapper_program] [[num_threads]]\n", argv0);
	
	printf("\n");

	printf("If you specify \"-\" as input filename, stdin will be used (without splitting).\n");
	
	printf("\n");

	printf("Build conf:\n");
	printf("\t%-20s : %5d\n", "Max threads",	MAX_THREADS);
	printf("\t%-20s : %5d\n", "Default threads",	DEF_THREADS);
	printf("\t%-20s : %5d\n", "Min file part",	MIN_FILE_PART);
	printf("\t%-20s : %5d\n", "Max line size",	MAX_BUFFER);

	printf("\n");
}





/*
 * Destructive trim-right()
 */
/*
char *trimr(char *s){
	if (s == NULL)
		return NULL;

	while( strlen(s) && isspace( s[ strlen(s) - 1 ] ) ){
		s[ strlen(s) - 1 ] = '\0';
	}
	
	return s;
}
*/

