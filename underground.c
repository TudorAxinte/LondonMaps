#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Global Variables
char* stations_file = "stations.in";



struct metro_station
{
	int id;                 // Station identification number
	int connected_ids[10]; // The ids of stations connected to this station 

	char name[30];
 	char line[30];

 	int distance_from_start; // Temporary variable, used in the search progress to find the shortest route

} station[400];

void get_stations()
{
 	char *buffer;
    size_t bufsize = 32;
    ssize_t read;
    FILE * fp = fopen(stations_file, "r");

    int id = 0;
    char *current_line;

    buffer = (char *)malloc(bufsize * sizeof(char)); //Assigning 32 bytes of storage to prevent the program to run out of memory
    current_line = (char *)malloc(bufsize * sizeof(char));

    if (fp == NULL)
    {
        printf("Could not open file %s", stations_file);
        exit(EXIT_FAILURE);
    }
    else 
   		while ((read = getline(&buffer, &bufsize, fp)) != -1) 
    		{
      			
    	     if (strchr(buffer, '*') != NULL)

    	     {	
    	     	read = getline(&buffer, &bufsize, fp);
    	     	strcpy( current_line, buffer);

    	     }
    	     
    	     else
    	     	{

    	     		strcpy (station[id].name, buffer);
    	     		strcpy (station[id].line, current_line); 
    	     		id++;

    	     	} 

    		}

    fclose(fp);
}

int main()
{
	get_stations();
	
return 0;	
}
