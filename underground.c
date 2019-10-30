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

    	     	//printf("\nLine %s", line[line_id].name );   

    	     }
    	     
    	     else
    	     	{
    	     		// Creating station data
    	     		strcpy (station[id].name, buffer); 
					//printf("  id: %d name: %s", id, station[id].name );

    	     		memset(station[id].lines_connected, 0, 4*sizeof(int));
    	     		station[id].lines_connected[0] = line_id;  

    	     		line [ line_id ].stations [number_of_stations] = id; // Adding the station to the current line
    	     		
    	     		id++; 
    	     		number_of_stations++;

    	     	} 

    		}
    line[line_id].number_of_stations = number_of_stations; 
		
    total_lines = line_id;
    total_stations = id - 1;
    fclose(fp);
}

int find_interline_stations() 
{
 	bool is_inteline;
 	int counter;
 	int total_junctions = 0;

 	for (int id=0; id < total_stations; id++)
 		{
 		counter = 0;
 		is_inteline = false;

 		for (int line_id = 1; line_id <= total_lines; line_id++)
 			
 			for (int t=0; t < line[line_id].number_of_stations; t++) // Variable t is an index for line[j].stations[]
 				
 				// Comparing every station with stations of each line, 
 				if (strstr(station[id].name, station[ line[line_id].stations[t] ].name) != 0 
 					&& station[id].lines_connected[0] != line_id)
 				{
 					counter++;
 					station[id].lines_connected[counter] = line_id;
 					line_id++; break; //We won't find the same station 2 times on a line, search the next one
 				}		 	

 		if (counter>0) { station[id].is_inteline = true; total_junctions++;}

 		}

 	return total_junctions;	
}

void print_line(int line_number) 
{
	printf("%d stations found for line %s \n", 
		line[line_number].number_of_stations, line[line_number].name );
	
	for (int i=0;i<line[line_number].number_of_stations;i++) 
		printf("id: %d | is_junction: %d | name: %s",
			line[line_number]. stations[i],
			station[line[line_number].stations[i]].is_inteline,
			station[line[line_number].stations[i]].name );
	
}

void print_station(int station_id)
{
	printf("\nSTATION_NAME: %s", station[station_id].name);
	printf("STATION_ID:   %d \n", station_id);
	printf("IS_INTERLINE: %d \n", station[station_id].is_inteline);
	printf("LINES CONNECTED:\n");
		for (int i=0;i<=4;i++)
		{
			if (station[station_id].lines_connected[i] != 0)
				printf("(ID: %d) %s", 
					station[station_id].lines_connected[i],
					line[station[station_id].lines_connected[i]].name);
		}
	printf("\n");
}


void print_interline_stations()
{
for (int i=0; i<= total_stations; i++)
		if (station[i].is_inteline) 
		{
			printf("lines from (id:%d) %s", i, station[i].name);
			for (int j=0; j<=4; j++)
				if (station[i].lines_connected[j] != 0 )
					printf(":%s", line[station[i].lines_connected[j]].name);
			printf("\n");	
		}

}

void debug()
{
	char command[20];
	int id;
	bool debuging = true;
	
	while (debuging)
	{			
		printf("\nWaiting for a new command: ");
		scanf("%s", command);
		
		if (strcmp(command,"station")==0)
		{
			scanf("%d", &id);
			
			if (id <= total_stations) 
					print_station(id);
			else 
					printf("There are only %d stations, try again\n", total_stations);
		}
		
		else if (strcmp(command,"line")==0)
		{	
			scanf("%d", &id);
			
			if (id <= total_lines) 
					print_line(id);
			else 
					printf("There are only %d lines, try again\n", total_lines);
		}
		
		else if (strcmp(command,"junctions")==0)
		{	
			print_interline_stations();
		}	
		
		else if (strcmp(command,"name")==0)
		{	
			scanf("%d", &id);
			printf("Found %ld characters in the station name %s \n", 
				    strlen(station[id].name), station[id].name );
		}
		
		else if (strcmp(command,"read")==0)
		{	
			get_stations();
			printf("\nStation list updated successfully with %d stations on %d lines.\n", total_stations, total_lines);
		}

		else if (strcmp(command,"update")==0)
		{	
			int counter = find_interline_stations();
			printf("\nSuccessfully checked and updated the stations list with %d junction stations.\n", counter);
		}
		else if (strcmp(command,"quit")==0)
		{	
			debuging = false;
			break;
		}
		else
		{	
			printf("Unknown command, try again\n");
		}
	}
}


int main()
{
	
	debug();

return 0;	
}
