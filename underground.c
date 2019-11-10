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
bool testing;
int algorithm;
int current_line;
int first_line;
int middle_line;
int destination_line;
int total_stations;
int total_lines;
int total_junctions;
int total_lenght;
int total_changes;
int start_id;
int destination_id;
int line_frecvence_array[20];
int line_frecvence_array2[20];

struct metro_station
{
	char name[30]; 
	int lines_connected[6]; 
 	int number_of_lines;
 
} station[400];

struct metro_line
{
	char name[30];
	int stations[60];
	int number_of_stations;

} line[13]; 

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

void location(int n)
{

	switch (n)
	{
		case 1:
			station_pass_time = 1.25;
			stations_file = "stations.in";
			service_operator= "TFL";
		break;

		case 2:
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
      		 if (strchr(string, '$') != NULL)
      		 {
      		 	line[line_id].number_of_stations = number_of_stations; 
      			total_lines = line_id;
    			total_stations = id;
    			fclose(fp);
      		 }
    	     
    	    else if (strchr(string, '*') != NULL) 

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
 			for (int t=0; t < line[line_id].number_of_stations; t++)
 				
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

void get_user_input()
{

	char typed_name[32];
	start_id = -1;
	destination_id = -1;

	while (start_id == -1)
	{
		printf("Type starting station: ");
		fgets(typed_name, 30, stdin);
		strcpy( typed_name + strlen(typed_name) - 1, typed_name + strlen(typed_name));//Remove spaces    	     	

		for (int id=0; id<= total_stations; id++)
			if (strstr(station[id].name, typed_name) != 0) 
				{
					start_id=id;
					break;
				}
		if (start_id == -1)
			printf("Couldn't find any station called %s, please try again.\n", typed_name);
		if (strlen(typed_name) < 4)
		{
			printf("Input lenght too short, please try again.\n");
			start_id = -1;
		}
	}	

	while (destination_id == -1)
	{
		printf("Type destination station: ");
		fgets(typed_name, 30, stdin);
		strcpy( typed_name + strlen(typed_name) - 1, typed_name + strlen(typed_name));//Remove spaces    	     	

		for (int id=0; id<= total_stations; id++)
			if (strstr(station[id].name, typed_name) != 0) 
				{
					destination_id=id;
					break;
				}
		if (destination_id == -1)
			printf("Couldn't find any station called %s, please try again.\n", typed_name);
		if (start_id == destination_id)
		{
			printf("Destination is the same as the start, please choose again.\n");
			destination_id = -1;
		}
		if (strlen(typed_name) < 4)
		{
			printf("Input lenght too short, please try again.\n");
			destination_id = -1;
		}
	}

	
} 	

int check_input()
{
	algorithm = 0;

	memset(line_frecvence_array, 0, (total_lines+1)*sizeof(int));

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
	
	// Test if the stations differ by one line 
	for(int destination_line_index=0; destination_line_index < station[destination_id].number_of_lines; destination_line_index++)
	{		
		destination_line = station[destination_id].lines_connected[destination_line_index];
			
		for(int line_index=0; line_index < station[start_id].number_of_lines; line_index++)
		{
				current_line = station[start_id].lines_connected[line_index];

				for (int station_index = 0; station_index < line[current_line].number_of_stations ; station_index++)
			
					for(int line_index2=0; line_index2 < station[line[current_line].stations[station_index]].number_of_lines; line_index2++)

						if (station[line[current_line].stations[station_index]].lines_connected[line_index2] == destination_line )
						{

							algorithm = 2; 
							return 0;
						}
		}				
	}	
   
	// Test if the stations differ by more lines
	for (int starting_line_index=0; starting_line_index < station[start_id].number_of_lines; starting_line_index++)
	{
		current_line = station[start_id].lines_connected[starting_line_index];
		for (int i=1; i<=total_lines; i++) line_frecvence_array[i] = 0;

		// Update lines we can reach from the starting station
		for (int station_index=0; station_index < line[current_line].number_of_stations; station_index++)

			for (int line_index=0; line_index < station[line[current_line].stations[station_index]].number_of_lines; line_index++)

				line_frecvence_array[station[line[current_line].stations[station_index]].lines_connected[line_index]]++;

		// Verify each line for connections with destination's line
		for (int line_id=1; line_id <= total_lines; line_id++)
		{
			memset(line_frecvence_array2, 0, (total_lines+1)*sizeof(int));

			if (line_frecvence_array[line_id] > 0) 
			{	
				for (int station_index=0; station_index < line[line_id].number_of_stations; station_index++)

					for(int line_index=0; line_index < station[line[line_id].stations[station_index]].number_of_lines; line_index++ )
					{	
						line_frecvence_array2[station[line[line_id].stations[station_index]].lines_connected[line_index]]++;
						
						for(int destination_line_index=0; destination_line_index < station[destination_id].number_of_lines; destination_line_index++)
							
							if (station[line[line_id].stations[station_index]].lines_connected[line_index] 
								== station[destination_id].lines_connected[destination_line_index])
							{
								// We found a route with 2 changes
								algorithm = 3;
								destination_line = station[destination_id].lines_connected[destination_line_index];
								middle_line = line_id;
								return 0; 
							}
					}
							
				for (int line_id2=1; line_id2 <= total_lines; line_id2++)

					if (line_frecvence_array2[line_id2] > 0 && line_id != current_line) 

						for (int station_index2=0; station_index2 < line[line_id2].number_of_stations; station_index2++)

							for (int line_index2=0; line_index2 < station[line[line_id2].stations[station_index2]].number_of_lines; line_index2++)

								for(int destination_line_index=0; destination_line_index < station[destination_id].number_of_lines; destination_line_index++)

									if (station[line[line_id2].stations[station_index2]].lines_connected[line_index2] 
								 		== station[destination_id].lines_connected[destination_line_index])
									{
									// We found a route with 3 changes
									algorithm = 4;
									destination_line = station[destination_id].lines_connected[destination_line_index];
									middle_line = line_id2;
									first_line = line_id;
									return 0; 
									}
			}
		}
	}							
}

void print_path(int start_index, int destination_index, int lenght)
{

if (lenght!=0)
	
	if (start_index > destination_index)
	{
		color_reset();  printf("Take the undergrounder heading ");
		color_magenta(); printf("%s:", line[current_line].name);
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
		color_magenta(); printf("%s:", line[current_line].name);
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
	int lenght = 999, start_index, destination_index;

	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strcmp( station[line[current_line].stations[i]].name, station[destination_id].name) == 0) 
			{
				destination_index = i;
				break;
			}

	// Extra check to make sure we find the shortest route for lines with duplicate stations
	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strcmp( station[line[current_line].stations[i]].name, station[start_id].name) == 0 &&
			(abs(i - destination_index) < lenght) ) 
				{ 
					start_index = i;
					lenght = abs(start_index - destination_index);
				}
	
	time  += lenght * station_pass_time;
	total_lenght += lenght;

	if (!testing) print_path(start_index, destination_index, lenght);
}

void two_lines_path(int destination_line_id)
{
	int lenght = 999, distance = 999, start_index = 0, destination_index = 0;

	for (int i=0; i < line[current_line].number_of_stations; i++)
		if (strcmp(station[line[current_line].stations[i]].name, station[start_id].name) == 0) 
			{
				start_index = i;
				break;
			}

	for (int i=0; i < line[current_line].number_of_stations; i++)
		
		for (int index=0; index < station[line[current_line].stations[i]].number_of_lines; index++)
			
			if (station[line[current_line].stations[i]].lines_connected[index] == destination_line_id 
				&& (abs(i - start_index) < lenght) ) 
			{
				lenght = abs(i - start_index);
				destination_index = i;
			}

	if (lenght == 999)
	{
		color_red();
		printf("Failed while searching route %s(id:%d) %s(id:%d)\n",station[start_id].name, start_id,station[destination_id].name,destination_id);
		color_reset();
		printf("Start_index: %d | Destination_index: %d | Algoritm: %d\n current line: %s | first line: %s | middle line: %s | dest line: %s \n", 
		start_index, destination_index, algorithm, line[current_line].name, line[first_line].name, line[middle_line].name, line[destination_line].name);
		exit(EXIT_FAILURE);
	}
	
	time  += lenght * station_pass_time;
	total_lenght += lenght;

	if (!testing)
	{
		print_path(start_index, destination_index, lenght);

		color_reset();    printf("\nAt the junction ");
		color_blue();     printf("%s ", station[line[current_line].stations[destination_index]].name);
		color_reset();    printf("you have to change lines from ");
		color_red();      printf("%s",line[current_line].name);
		color_reset();    printf(" to ");
		color_red();      printf("%s \n",line[destination_line_id].name);
	}

	time += 5; // Adding some time accounting for the walk between lines
	total_changes++;
	
	start_id = line[current_line].stations[destination_index];
	current_line = destination_line_id;
	
}

void find_route()
{
	time = 0;
	total_lenght = 0;
	total_changes = 0;

	if (!testing)
	{
		printf("\nShowing directions from ");
		color_blue();  printf("%s", station[start_id].name);
		color_reset(); printf(" to ");
		color_blue();  printf("%s", station[destination_id].name);
		color_reset(); printf(".\n"); 
  	}

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
		two_lines_path(middle_line); 
		two_lines_path(destination_line);
		one_line_path();
		break;

		case 4:
		two_lines_path(first_line);
		two_lines_path(middle_line);
		two_lines_path(destination_line);
		one_line_path();
		break;

		default:
		printf("Didn't code that yet\n");
		break;
	}
	
	if (!testing)
	{
		printf("\nYour journey will take approximately %g minutes. (%d stops, %d changes)", round(time), total_lenght, total_changes);
		printf("\nService run by %s. Check their website for ticket information.\n\n", service_operator);
	}
}

void test()
{
	testing = true;

	if (station_pass_time == 2)
	{

		start_id = 11; destination_id=21;
		check_input();
		find_route();
		if (algorithm != 1 || total_changes != 0 || total_lenght != 10) 
			{
			printf("Eroilor - Pantelimon test failed\n"); 
			printf("Algorithm %d Changes %d Lenght %d\n", algorithm, total_changes, total_lenght);
			exit(EXIT_FAILURE);
			} 

		start_id = 12; destination_id=22;
		check_input();
		find_route();
		if (algorithm != 2 || total_changes != 1 || total_lenght != 8) 
			{
			printf("Izvor - Berceni test failed\n"); 
			printf("Algorithm %d Changes %d Lenght %d\n", algorithm, total_changes, total_lenght);
			exit(EXIT_FAILURE);
			} 

		start_id = 36; destination_id=58;
		check_input();
		find_route();
		if (algorithm != 3 || total_changes != 2 || total_lenght != 15) 
			{
			printf("Preciziei - Straulesti test failed\n"); 
			printf("Algorithm %d Changes %d Lenght %d\n", algorithm, total_changes, total_lenght);
			exit(EXIT_FAILURE);
			} 	
	
		start_id = 58; destination_id=36;
		check_input();
		find_route();
		if (algorithm != 3 || total_changes != 2 || total_lenght != 15) 
			{
			printf("Straulesti - Preciziei test failed\n"); 
			printf("Algorithm %d Changes %d Lenght %d\n", algorithm, total_changes, total_lenght);
			exit(EXIT_FAILURE);
			} 

		start_id = 18; destination_id=70;
		check_input();
		find_route();
		if (algorithm != 3 || total_changes != 2 || total_lenght != 17) 
			{
			printf("Titan - Dobra test failed\n"); 
			printf("Algorithm %d Changes %d Lenght %d\n", algorithm, total_changes, total_lenght);
			exit(EXIT_FAILURE);
			} 
	
		printf("All tests passed successfully.\n");
	
	}
	else
		printf("The test was designed for Bucharest only.\n");

	testing = false;
}

void full_test()
{
	testing = true;
	
	int errors = 0, temp; 
	unsigned long long total = total_stations*total_stations;

	for (int i=0; i < total_stations; i++)
	{	
		for(int j=0; j< total_stations; j++)
			if (i != j)
			{
			start_id = i;
			destination_id = j;

			check_input();
			find_route();
			
			if (total_lenght>100 || algorithm==0)
				{
				errors++;
				color_red();
				printf("Test failed for route %s(id:%d) to %s(id:%d)\n",
					station[start_id].name, 
					start_id,
					station[destination_id].name, 
					destination_id);
				color_reset();
				}
			
			}
	}
	
	if (errors == 0) printf("All %llu tests passed successfully.\n", total);		
	else printf("%d tests failed out of %llu tried\n", errors, total);

	testing = false;
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
			station[line[line_number].stations[i]].name );
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
	fgets(typed_name, 30, stdin);//Cleaning buffer

	printf("Type station name: ");
	fgets(typed_name, 30, stdin);
	strcpy( typed_name + strlen(typed_name) - 1, typed_name + strlen(typed_name));//Remove spaces    	     	

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
			printf("(id:%d) %s has the following lines:\n", i, station[i].name);
			
			for (int j=0; j< station[i].number_of_lines; j++)
									
					printf(":%s \n", line[station[i].lines_connected[j]].name);
			
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
		fflush(stdin);
		printf("\nWaiting for a new command: ");
		scanf("%s", command);
		
		if (strcmp(command,"init")==0)
		{
			int n; 
			scanf("%d", &n);
			location(n);
			
			if (n==1) 
				printf("The location is now set to London. Mind the gap.\n");
			if (n==2) 
				printf("The location is now set to Bucharest.\n");
			
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
			fgets(command, 30, stdin);//cleaning buffer
			get_user_input();
			check_input();
			find_route();
			debug();
			break;
		}

		else if (strcmp(command,"r")==0)
		{	
			int s1,s2;
			scanf("%d %d",&s1,&s2);
			start_id = s1;
			destination_id = s2;
			check_input();
			find_route();
			debug();
			break;
		}
		
		else if (strcmp(command,"test")==0)
		{	
			test();
			debug();
			break;
		}

		else if (strcmp(command,"fulltest")==0)
		{	
			full_test();
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
	location(1);
	get_stations();		
	find_junctions();
	get_user_input();
	check_input();
	find_route();

	debug();

return 0;	
}
