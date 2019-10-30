#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 

// Global Variables
char* stations_file = "stations.in";
int total_stations;
int total_lines;

struct metro_station
{
	char name[30]; // The name of the current station
	bool is_inteline;
	int lines_connected[4]; // The ids of the lines this station is a part of
 	int distance_from_start; // Temporary variable, used in the search progress to find the shortest route

} station[400];

struct metro_line
{
	char name[30];
	int stations[60];
	int number_of_stations;

} line[13]; 

void get_stations()
{
 	char *buffer;
    size_t bufsize = 32;
    ssize_t read;
    FILE * fp = fopen(stations_file, "r");

    int id = 0;
    int number_of_stations;
    int line_id = 0 ;

    //Assigning 32 bytes of storage to prevent the program to run out of memory
    buffer = (char *)malloc(bufsize * sizeof(char)); 

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
    	     	// Switching to a new line, finnishing the previous cycle
    	     	line[line_id].number_of_stations = number_of_stations; 
    	     	
    	     	line_id++; // Line array starts at 1, feature not bug                              
    	     	number_of_stations = 0;                  
    	     	read = getline(&buffer, &bufsize, fp);
    	     	strcpy( line[line_id].name, buffer);
    	     	strncpy( line[line_id].name, line[line_id].name, 30);     

    	     }
    	     
    	     else
    	     	{
    	     		// Creating station data
    	     		strcpy (station[id].name, buffer); 
    	     		strncpy(station[id].name, station[id].name, strlen(station[id].name));
    	     		memset(station[id].lines_connected, 0, 4*sizeof(int));
    	     		station[id].lines_connected[0] = line_id;  

    	     		line [ line_id ]. stations [number_of_stations] = id; // Adding the station to the current line
    	     		
    	     		id++; 
    	     		number_of_stations++;

    	     	} 

    		}
    total_lines = line_id;
    total_stations = id - 1;
    fclose(fp);
}

void find_interline_stations() 
{
 	bool is_inteline;
 	int counter;

 	for (int i=0; i<= total_stations; i++)
 		{
 		counter = 0;
 		is_inteline = false;

 		for (int j=1; j<=total_lines; j++)
 			for (int t=0; t<=line[j].number_of_stations; t++)
 				if (strstr(station[i].name, station[line[j].stations[t]].name)!=0 && station[i].lines_connected[0]!=j)
 				{
 					counter++;
 					station[i].lines_connected[counter] = j;
 				}		 	

 		if (counter>0) station[i].is_inteline = true;

 		}

 		
}

void print_line(int line_number) //Debug function to test if the reading works
{
	printf("%d stations found for line %s \n", 
		line[line_number].number_of_stations, line[line_number].name );
	
	for (int i=0;i<line[line_number].number_of_stations;i++) 
		printf("id: %d name: %s",
			line[line_number]. stations[i],
			station[line[line_number].stations[i]].name );
	
}

void print_interline_stations()
{
for (int i=0; i<= total_stations; i++)
		if (station[i].is_inteline) 
		{
			printf("lines from %s", station[i].name);
			for (int j=0; j<=4; j++)
				if (station[i].lines_connected[j] != 0 )
					printf(":%s", line[station[i].lines_connected[j]].name);
			printf("\n");	
		}

}

int main()
{
	get_stations();
	find_interline_stations();
	print_interline_stations();
	//print_line(11);
	//printf("Name: %s", station[351].name );

return 0;	
}
