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