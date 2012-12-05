#include <stdio.h>

#include "split.h"

unsigned long int *split(const char *filename, unsigned short int parts, unsigned int min_part_size, unsigned long int map[]){
	FILE *F = fopen(filename, "rb");

	fseek(F, 0, SEEK_END);

	unsigned long int size = ftell(F);
			
	unsigned long int part_size = size / parts;
	
	if (part_size < min_part_size)
		part_size = min_part_size;

	rewind(F);

	unsigned short int i;
	
	for (i = 0; i < parts; i++){
		if (i == 0){
			map[i] = 0;
			continue;
		}
		
		const unsigned int buff_size = 16 * 1024;
		char buff[buff_size];
		
		fseek(F, part_size, SEEK_CUR);
		fgets(buff, buff_size, F);
		
		unsigned long int pos = ftell(F);
		if (pos >= size)
			break;

		map[i] = pos;
	}
	
	map[i] = size;
	map[i + 1] = 0;
	
	fclose(F);

	return map;
}

