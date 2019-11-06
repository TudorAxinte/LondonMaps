#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h> 
#include <math.h>

// Global Variables
char* stations_file;
char* service_operator;
float time;
float station_pass_time;
int algorithm;
int current_line;
int middle_line;
int destination_line;
int total_stations;
int total_lines;
int total_junctions;
int total_lenght;
int total_changes;
int start_id;
int destination_id;

struct metro_station
{
	char name[30]; 
	int lines_connected[6]; 
 	int distance_from_start;
 	int number_of_lines;
 
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

    char string[32];

    total_stations = 0;
    total_lines = 0;


    if (fp == NULL)
    {
        printf("Could not open file %s", stations_file);
        exit(EXIT_FAILURE);
    }
    else 
   		while (fgets(string, 64, fp) != NULL) 
    		{
      		 	
    	     if (strchr(string, '*') != NULL) 

    	     {	
    	     	// Switching to a new line, finnishing the previous cycle
    	     	line[line_id].number_of_stations = number_of_stations; 
    	   
    	     	line_id++; // Lines array starts at 1, feature not bug                             
    	     	number_of_stations = 0;                  
    	     	fgets(string, 64, fp);	
    	     	strcpy( string + strlen(string) - 2, string + strlen(string));//Remove spaces    	     	
    	     	strcpy( line[line_id].name, string);
    	     }
    	     
    	     else
    	     	{
    	     		// Creating station data
    	     		strcpy( string + strlen(string) - 2, string + strlen(string));//Remove spaces
    	     		strcpy (station[id].name, string); 
    	     		memset(station[id].lines_connected, 0, 6*sizeof(int));
    	     		station[id].lines_connected[0] = line_id;  

    	     		line[line_id].stations[number_of_stations] = id; // Adding the station to the current line
    	     		
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
 	int counter;

 	total_junctions = 0;

 	for (int id=0; id < total_stations; id++)
 		{
 		counter = 0;
 		station[id].number_of_lines = 1;

 		for (int line_id = 1; line_id <= total_lines; line_id++)
 		{
 			for (int t=0; t < line[line_id].number_of_stations; t++) // Variable t is an index for line[j].stations[]
 				
 				// Comparing every station with stations of each line, 
 				if (strcmp(station[id].name, station[line[line_id].stations[t]].name) == 0 
 					&& station[id].lines_connected[0] != line_id
 					&& station[id].lines_connected[counter] != line_id)
 				{
 					counter++;
 					station[id].lines_connected[counter] = line_id;
 					station[id].number_of_lines ++;

 					//break; The same station can appear on the same line twice, no need for break
 				}		 	
 		}

 		if (counter > 0) total_junctions++;
 		}
}

void print_line(int line_number) 
{
	printf("The line %s has %d stations: \n", 
		 line[line_number].name,
		 line[line_number].number_of_stations );
	
	for (int i=0;i<line[line_number].number_of_stations;i++) 
		printf("| index: %d | id %d | %d lines | %s  \n",
			i,
			line[line_number]. stations[i],
			station[line[line_number].stations[i]].number_of_lines,
			station[line[line_number].stations[i]].name

			 );
	
}

void print_station(int station_id)
{
	printf("\nStation[%d] is called %s and has %d connected lines: \n", 
		station_id,
		station[station_id].name,
		station[station_id].number_of_lines);
	
	for (int i=0; i< station[station_id].number_of_lines; i++)
		{
			printf("(Line-ID: %d) %s \n", 
			station[station_id].lines_connected[i],
			line[station[station_id].lines_connected[i]].name);
		}
	printf("\n");
}

void find_station()
{
	char typed_name[30];
	gets(typed_name);//Cleaning buffer

	printf("Type station name: ");
	gets(typed_name);

	printf("Printing all station instances of %s.\n", typed_name);

	for(int id=0; id < total_stations; id++)
		
		if ( strstr(station[id].name, typed_name) != 0) print_station(id);

}

void print_interline_stations(int n)
{
printf("Printing all the stations with at least %d lines connected:\n", n);
for (int i=0; i<= total_stations; i++)
		if (station[i].number_of_lines >= n) 
		{
			printf("(id:%d) %s \n", i, station[i].name);
			for (int j=0; j<=4; j++)
				if (station[i].lines_connected[j] != 0 )
					printf(":%s \n", line[station[i].lines_connected[j]].name);
			printf("\n");	
		}

}

void color_magenta () {
  printf("\033[0;35m");
}

void color_red () {
  printf("\033[1;31m");
}

void color_green (){
  printf("\033[0;32m");
}

void color_boldyellow (){
  printf("\033[0;33m");
}

void color_yellow (){
  printf("\033[01;33m");
}

void color_boldblue (){
  printf("\033[0;34m");
}

void color_blue (){
  printf("\033[1;34m");
}	

void color_reset () {
  printf("\033[0m");
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
			printf("Couldn't find any station called %s, please try again.\n", typed_name);
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
			printf("Couldn't find any station called %s, please try again.\n", typed_name);
	}

	
} 	

int check_input()
{
	algorithm = 0;
	
	int line_frecvence_array[total_lines];
	memset(line_frecvence_array, 0, total_lines*sizeof(int));

	for (int index = 0; index < station[start_id].number_of_lines; index++)
		line_frecvence_array[station[start_id].lines_connected[index]]++;
		
	for (int index = 0; index < station[destination_id].number_of_lines; index++)
		line_frecvence_array[station[destination_id].lines_connected[index]]++;
		

	// Test if the stations are on the same line
	for (int index = 1; index <= total_lines; index++)
		if (line_frecvence_array[index] >= 2) 
			{
				algorithm = 1;
				current_line= index;
				return 0;

			}	
	
	// Test if the stations differ by one line only
	for(int destination_line_index=0; destination_line_index < station[destination_id].number_of_lines; destination_line_index++)
	{		
		destination_line = station[destination_id].lines_connected[destination_line_index];
			
		for(int line_index=0; line_index < station[start_id].number_of_lines; line_index++)
		{
				current_line = station[start_id].lines_connected[line_index];

				for (int index = 0; index <= line[current_line].number_of_stations ; index++)
			
					for(int station_line_index=0; station_line_index< station[line[current_line].stations[index]].number_of_lines; station_line_index++)

						if (station[line[current_line].stations[index]].lines_connected[station_line_index] == destination_line)
						{
							algorithm = 2;
							return 0;
						}
		}				
	}	
   
   	// Test if the stations differ by 2 lines
	for (int starting_line_index=0; starting_line_index < station[start_id].number_of_lines; starting_line_index++)
	{
		current_line = station[start_id].lines_connected[starting_line_index];
		memset(line_frecvence_array, 0, total_lines*sizeof(int));

		// Update lines we can reach from the starting station
		for (int station_index=0; station_index < line[current_line].number_of_stations; station_index++)

			for (int line_index=0; line_index < station[line[current_line].stations[station_index]].number_of_lines; line_index++)

				line_frecvence_array[station[line[current_line].stations[station_index]].lines_connected[line_index]]++;

		// Verify each line for connections with destination's line
		for (int line_id=1; line_id <= total_lines; line_id++)

			if (line_frecvence_array[line_id] > 0) 

				for (int station_index=0; station_index < line[line_id].number_of_stations; station_index++)

					for(int line_index=0; line_index < station[line[line_id].stations[station_index]].number_of_lines; line_index++ )

						for(int destination_line_index=0; destination_line_index < station[destination_id].number_of_lines; destination_line_index++)
							
							if (station[line[line_id].stations[station_index]].lines_connected[line_index] 
								== station[destination_id].lines_connected[destination_line_index])
							{
								algorithm = 3;
								destination_line = station[destination_id].lines_connected[destination_line_index];
								middle_line = line_id;
								return 0; 
							}
	}


}

void print_path(int start_index, int destination_index, int lenght)
{
	if (start_index > destination_index)
	{
		color_reset();  printf("Take the undergrounder heading ");
		color_boldyellow(); printf("%s", station[line[current_line].stations[0]].name);
		color_reset();  printf(" (%d stops) \n\n", lenght);
		
		if (lenght <= 6)
		{
			color_red();
			for (int i=start_index; i > destination_index; i--)
			{
				printf("%s", station[line[current_line].stations[i]].name );
				color_reset(); printf(" -> ");
				color_green();
			}
			color_red();
			printf("%s \n", station[line[current_line].stations[destination_index]].name );
			color_reset();
		}
		else
		{
			color_red();
			for (int i=start_index; i > destination_index; i--)
			{
				printf(":%s \n", station[line[current_line].stations[i]].name );
				color_green();
			}
			color_red();
			printf(":%s \n", station[line[current_line].stations[destination_index]].name );
			color_reset();
		}

	}	
	else
	{   
		color_reset(); printf("Take the undergrounder heading ");
		color_boldyellow(); printf("%s",station[line[current_line].stations[line[current_line].number_of_stations - 1]].name);
		color_reset(); printf(" (%d stops) \n\n", lenght);	
		
		if (lenght <= 6)
		{	
			color_red();
			for (int i=start_index; i < destination_index; i++)
			{
				printf("%s", station[line[current_line].stations[i]].name );
				color_reset(); printf(" -> ");
				color_green();
			}
			color_red();
			printf("%s \n", station[line[current_line].stations[destination_index]].name );
			color_reset();
		}
		else
		{
			color_red();
			for (int i=start_index; i < destination_index; i++)
			{
				printf(":%s \n", station[line[current_line].stations[i]].name );
				color_green();
			}
			color_red();
			printf(":%s \n", station[line[current_line].stations[destination_index]].name );
			color_reset();
		}
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

void two_lines_path(int destination_line_id)
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
			if (station[line[current_line].stations[i]].lines_connected[index] == destination_line_id && abs(i - start_index) < lenght)
			{
				lenght = abs(i - start_index);
				destination_index=i;
			}

	time  += lenght * station_pass_time;
	total_lenght += lenght;

	print_path(start_index, destination_index, lenght);

	color_reset();    printf("\nAt the junction ");
	color_blue();     printf("%s ", station[line[current_line].stations[destination_index]].name);
	color_reset();    printf("you have to change lines from ");
	color_red(); printf("%s",line[current_line].name);
	color_reset();    printf(" to ");
	color_red(); printf("%s \n",line[destination_line_id].name);
	
	time += 5; // Adding some time accounting for the walk between lines
	total_changes++;
	start_id = line[current_line].stations[destination_index];
	current_line = destination_line_id;
	
}

void three_lines_path()
{
two_lines_path(middle_line);
two_lines_path(destination_line);
one_line_path();
}

void find_route()
{
	time = 0;
	total_lenght = 0;
	total_changes = 0;
	
	printf("\nShowing directions from ");
	color_blue();  printf("%s ", station[start_id].name);
	color_reset(); printf(" to ");
	color_blue();  printf("%s", station[destination_id].name);
	color_reset(); printf(".\n"); 

	switch (algorithm)
	{
		case 1:
		one_line_path();
		break;

		case 2:
		two_lines_path(destination_line);
		one_line_path();
		break;

		case 3:
		three_lines_path();
		break;

		default:
		printf("Didn't code that yet\n");
		break;
	}
	
	printf("\nYour journey will take approximately %g minutes. (%d stops, %d changes)", round(time), total_lenght, total_changes);
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
			int n;
			scanf("%d", &n);
			print_interline_stations(n);
			debug();
		}	
		
		else if (strcmp(command,"find")==0)
		{	
			find_station();
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
	// location(2);
	// get_stations();		
	// printf("Station list updated successfully with %d stations on %d lines.\n", total_stations, total_lines);
	// find_junctions();
	// printf("Junction list updated successfully with %d junction stations.\n", total_junctions);
	
	
	//get_user_input();            
	//check_input();   
	//find_route();
	
	debug();

return 0;	
}
