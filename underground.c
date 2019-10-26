#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global Variables
char* stations_file = "stations.in";



struct station
{
	int id;                 // Station identification number
	int connected_ids[10]; // The ids of stations connected to this station 

	char name[30];
 	char line[30];

 	int distance_from_start; // Temporary variable, used in the search progress to find the shortest route

};

void get_stations()
{
 	char *buffer;
    size_t bufsize = 32;
    ssize_t read;
    FILE * fp = fopen(stations_file, "r");


    buffer = (char *)malloc(bufsize * sizeof(char));

    if (fp == NULL)
    {
        printf("Could not open file %s", stations_file);
        exit(EXIT_FAILURE);
    }
    else 
   		while ((read = getline(&buffer, &bufsize, fp)) != -1) 
    {
     
    	    printf("'%s'\n",buffer);


    }

    fclose(fp);
}

int main()
{
	get_stations();

return 0;	
}