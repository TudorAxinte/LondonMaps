#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <math.h>

// Global Variables
char* stations_file;
char *buffer;
char* service_operator;
size_t bufsize = 32;
ssize_t read;
float time;
float station_pass_time;
int algorithm = 0;
int current_line;
int destination_line;
int total_stations;
int total_lines;
int total_junctions;
int total_lenght;
int start_id;
int destination_id;

struct metro_station
{
	char name[30]; 
	bool is_inteline;
	int lines_connected[6]; 
 	int distance_from_start; 

} station[400];

struct metro_line
{
	char name[30];
	int stations[60];
	int number_of_stations;

} line[13]; 

void location(int n)
{

	switch (n)
	{
		case 1:
		printf("The location is now set to London. Mind the gap.\n");
		station_pass_time = 1.25;
		stations_file = "stations.in";
		service_operator= "TFL";
		break;

		case 2:
		printf("The location is now set to Bucharest.\n");
		station_pass_time = 2.2;
		stations_file = "bucharest_stations.in";
		service_operator= "Metrorex";
		break;

		default:
		printf("No station data for that location\n");
		break;

	}
}

void get_stations()
{
    FILE * fp = fopen(stations_file, "r");

    int id = 0;
    int number_of_stations;
    int line_id = 0 ;

    total_stations = 0;
    total_lines = 0;

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

    	     		memset(station[id].lines_connected, 0, 6*sizeof(int));
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

void find_junctions() 
{
 	bool is_inteline;
 	int counter;

 	total_junctions = 0;

 	for (int id=0; id < total_stations; id++)
 		{
 		counter = 0;
 		is_inteline = false;

 		for (int line_id = 1; line_id <= total_lines; line_id++)
 		{
 			for (int t=0; t < line[line_id].number_of_stations; t++) // Variable t is an index for line[j].stations[]
 				
 				// Comparing every station with stations of each line, 
 				if (strstr(station[id].name, station[line[line_id].stations[t]].name) != 0 
 					&& station[id].lines_connected[0] != line_id)
 				{
 					counter++;
 					station[id].lines_connected[counter] = line_id;
 					//break; //We won't find the same station 2 times on a line, search the next one
 				}		 	
 		}

 		if (counter>0) { station[id].is_inteline = true; total_junctions++;}


 		}
}

void print_line(int line_number) 
{
	printf("%d stations found for line %s \n", 
		line[line_number].number_of_stations, line[line_number].name );
	
	for (int i=0;i<line[line_number].number_of_stations;i++) 
		printf("index: %d | id: %d | is_junction: %d | name: %s",
			i,
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
		for (int i=0;i<=6;i++)
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

void get_user_input()
{

	char typed_name[32];
	start_id = -1;
	destination_id = -1;
	gets(typed_name);//Cleaning buffer

	while (start_id == -1)
	{
		printf("Type starting station: ");
		gets(typed_name);

		for (int id=0; id<= total_stations; id++)
			if (strstr(station[id].name, typed_name) != 0) 
				{
					start_id=id;
					break;
				}
		if (start_id == -1)
			printf("\nCouldn't find any station called %s, please try again.\n", typed_name);
	}	

	while (destination_id == -1)
	{
		printf("Type destination station: ");
		gets(typed_name);

		for (int id=0; id<= total_stations; id++)
			if (strstr(station[id].name, typed_name) != 0) 
				{
					destination_id=id;
					break;
				}
		if (destination_id == -1)
			printf("\nCouldn't find any station called %s, please try again.\n", typed_name);
	}

	
} 	

int check_input()
{
	algorithm = 0;
	
	int line_frecvence_array[total_lines];
	memset(line_frecvence_array, 0, total_lines*sizeof(int));

	for (int index = 0; index <= 6; index++)
		{
		if (station[start_id].lines_connected[index] != 0) line_frecvence_array[station[start_id].lines_connected[index]]++;
		if (station[destination_id].lines_connected[index] != 0) line_frecvence_array[station[destination_id].lines_connected[index]]++;
		}

	// Test if the stations are on the same line
	for (int index = 1; index <= total_lines; index++)
		if (line_frecvence_array[index] >= 2) 
			{
				algorithm = 1;
				current_line= index;
				return 0;

			}	
	
	// Test if the stations differ by one line only
	for(int destination_line_index=0; destination_line_index<=6; destination_line_index++)
	{
		if (station[destination_id].lines_connected[destination_line_index] != 0)

			destination_line = station[destination_id].lines_connected[destination_line_index];
			
		for(int line_index=0; line_index <= 6; line_index++)
	
			if (station[start_id].lines_connected[line_index] != 0)
			{
				current_line = station[start_id].lines_connected[line_index];

				for (int index = 0; index <= line[current_line].number_of_stations ; index++)
			
					for(int station_line_index=0; station_line_index<= 6; station_line_index++)

						if (station[line[current_line].stations[index]].lines_connected[station_line_index] == destination_line)
						{
							algorithm = 2;
							return 0;
						}
		}				
	}	


}

void print_path(int start_index, int destination_index, int lenght)
{
	if (start_index > destination_index)
	{
		printf("Take the undergrounder %d stations heading towards %s \n", 
			lenght,
			station[line[current_line].stations[0]].name);
		for (int i=start_index; i >= destination_index; i--)
			printf(":%s", station[line[current_line].stations[i]].name );
		
	}	
	else
	{
		printf("Take the undergrounder %d stations heading towards %s \n",
			lenght,
			station[line[current_line].stations[line[current_line].number_of_stations - 1]].name);

		for (int i=start_index; i <= destination_index; i++)
			printf(":%s", station[line[current_line].stations[i]].name );
	}
}

void one_line_path()
{
	int lenght = 9999, start_index, destination_index;

	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strstr( station[line[current_line].stations[i]].name, station[destination_id].name) != 0) 
			{
				destination_index = i;
				break;
			}

	// Extra check to make sure we find the shortest route for lines with duplicate stations
	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strstr( station[line[current_line].stations[i]].name, station[start_id].name) != 0 &&
			abs(i - destination_index) < lenght) 
				{ 
					start_index = i;
					lenght = abs(start_index - destination_index);
				}
	
	time  += lenght * station_pass_time;
	total_lenght += lenght;

	print_path(start_index, destination_index, lenght);
}

void two_lines_path()
{
	int lenght = 9999, start_index, destination_index;

	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strstr( station[line[current_line].stations[i]].name, station[start_id].name) != 0) 
			{
				start_index = i;
				break;
			}

	for (int i=0; i < line[current_line].number_of_stations; i++)
		for (int index=0; index<=6; index++)
			if (station[line[current_line].stations[i]].lines_connected[index] == destination_line && abs(i - start_index) < lenght)
			{
				lenght = abs(i - start_index);
				destination_index=i;
			}

	time  += lenght * station_pass_time;
	total_lenght += lenght;

	print_path(start_index, destination_index, lenght);

	printf("\nGet off the current train at %s", station[line[current_line].stations[destination_index]].name);
	printf("Mind the gap. You will have to switch to line %s", line[destination_line].name );
	
	start_id = line[current_line].stations[destination_index];
	current_line = destination_line;
	one_line_path();

}

void find_route()
{
	time = 0;
	total_lenght = 0;
	printf("\n");

	switch (algorithm)
	{
		case 1:
		one_line_path();
		break;

		case 2:
		two_lines_path();
		break;

		default:
		printf("Didn't code that yet\n");
		break;
	}
	
	printf("\nYour journey will take %g minutes (%d stops)", round(time), total_lenght);
	printf("\nService run by %s. Check their website for ticket information.\n", service_operator);
}

void debug()
{
	char command[20];
	int id;
	bool debuging = true;
	
	while (debuging)
	{					   
		fflush(stdin);
		printf("\nWaiting for a new command: ");
		scanf("%s", command);
		
		if (strcmp(command,"init")==0)
		{
		int n; scanf("%d", &n);
		
		location(n);
		get_stations();
		printf("Station list updated successfully with %d stations on %d lines.\n", total_stations, total_lines);
		find_junctions();
		printf("Junction list updated successfully with %d junction stations.\n", total_junctions);
		debug();

		}

		else if (strcmp(command,"station")==0) 
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
			debug();
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
			printf("Station list updated successfully with %d stations on %d lines.\n", total_stations, total_lines);
		}

		else if (strcmp(command,"update")==0)
		{	
			printf("Junction list updated successfully with %d junction stations.\n", total_junctions);
		}
		
		else if (strcmp(command,"location")==0)
		{	
			int n;
			scanf("%d", &n);
			location(n);
			debug();
			break;
		}
		
		else if (strcmp(command,"input")==0)
		{	
			get_user_input();
			check_input();
			find_route();
			debug();
			break;
		}
		
		else if (strcmp(command,"quit")==0)
		{	
			debuging = false;
			exit(0);
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
	// get_stations();  
	// find_junctions();
	// get_user_input();            
	// check_input();   
	// find_route();
	debug();
	// CREATE A FILE WITH SMART PRECOMPILED LINKERS BEFOREHAND, WILL TAKE A LOT TO COMPILE BUT INSTANT AFTER

return 0;	
}
