#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <pthread.h>

pthread_mutex_t lock;

struct Room 
{
	char * name;
	char * connections [6];
	int num_connections;
	char * room_type;
};

void getTime()
{
	pthread_mutex_trylock(&lock);
	FILE * fd;
	fd=fopen("currentTime.txt","r");
	chmod("currentTime.txt",0777);
	char * buffer=NULL;
	size_t buff = 100;
	if(fd!=NULL)
	{
		getline(&buffer,&buff,fd);		
		printf("%s\n",buffer);
	}
	fclose(fd);
	pthread_mutex_unlock(&lock);
}

static void * writeTime(void * attr)
{
	pthread_mutex_trylock(&lock);
	FILE * atime;
	char* time_file = "currentTime.txt";
	atime = fopen(time_file,"w");
	time_t clock;
	clock = time(NULL);
	struct tm tm_clock = *localtime(&clock);
	int size = 100;
	char formatTime[size];
	strftime(formatTime,sizeof(formatTime),"%I:%M%P, %A, %B %d, %G",&tm_clock);
	if(formatTime[0] == '0')
	{
		formatTime[0]=' ';
	fprintf(atime,"%s",formatTime);
	}
	else
	{
		fprintf(atime," %s",formatTime);
	}
	fclose(atime);
	pthread_mutex_unlock(&lock);
	return NULL;
	
}

int find_current(struct Room holder[7],char * name )
{
	int i;
	for(i = 0; i < 7; i++)
	{
		if(strcmp(holder[i].name,name) == 0)
		{
			return i;
		}
	}	
}
int get_input(struct Room holder[7],char victory[100][100],int current,int * steps)
{
	int read_n = 100;
	char user_input [read_n];
	fflush(stdin);
	fgets(user_input,read_n,stdin);
	int i,check = 0;
	for(i =0; i <holder[current].num_connections; i++)
	{
		if(strcmp(holder[current].connections[i],user_input) ==0)
		{
			check = 1;
			strcpy(victory[*steps],holder[current].connections[i]);
			(*steps)++;
			current = find_current(holder,holder[current].connections[i]);
		}
	}
	if(check == 0)
	{
		if(strcmp(user_input,"time\n")==0)
		{
			pthread_t timeClock;
			const pthread_attr_t* attr = NULL;
			void * args = NULL;
			int threadOne =  pthread_create(&timeClock,
					NULL,
					writeTime,
					args);		
			pthread_t mainGame;
			void * gameArgs = NULL;			
			void * return_value;
			int i = pthread_join(timeClock,return_value);
			getTime();
		}
		else
		{
			printf("\nHUH? I DON'T UNDERSTAND THAT ROOM. TRY AGAIN.\n\n");
		}
	}
	return current;

}

void find_room_files (char file_paths[7][200], char * dir_path)
{
	DIR * dir;
	int i =0;
	int q;
	FILE * fp;
	struct dirent *entry;
	char * room_files [7];
	char temp_path[100];
	strcpy(temp_path,dir_path);	
	dir = opendir(temp_path);
	if(dir == NULL)
	{
		exit(1);
	}
	while(entry=readdir(dir))
	{
		if(entry->d_type == 8)
		{
			room_files[i] = entry->d_name;
			i++;
		}
	}
	closedir(dir);
	char folder_path [200];	
	for(q = 0; q < 7; q++)
	{
		for(i =0; i < strlen(dir_path); i++)
		{
			folder_path[i] = dir_path[i];
		}
		folder_path[strlen(dir_path)] ='/';
		int t = 0;
		for (i = strlen(dir_path) + 1; i < strlen(dir_path) + 1 + 
				strlen(room_files[q]); i++)
		{
			folder_path[i] = room_files[q][t];
			t++;

		}
		folder_path[strlen(dir_path)+1+strlen(room_files[q])] = '\0';
		int temp = strlen(dir_path)+ 1 + strlen(room_files[q]);
		for(i = 0; i < temp; i++)
		{
			file_paths[q][i]=folder_path[i];
		}
		file_paths[q][temp]='\0';
	}
}

char * find_recent_dir()
{
	DIR * dir;
	dir =opendir(".");
	struct dirent * entry;
	char * dir_holder[100];
	if (dir == NULL)
	{
		printf("dir is null");
		exit(1);	
	}
	int t =0;
	while(entry=readdir(dir))
	{
		if(entry->d_type == 4)
		{
			dir_holder[t] = entry->d_name;
			t++;
		}

	}
	closedir(dir);
	int i;
	char * check = "matsumon.rooms.";
	int q;
	int s =0;
	char * relevant_dir[t];
	for(q = 0; q < t; q++)
	{
		int r = 0;
		for(i = 0; i < 15; i ++)
		{
			if(dir_holder[q][i] != check[i])
			{
				r=1;
			}

		}
		if( r == 0)
		{
			relevant_dir[s] = dir_holder[q];				
			s++;
		}
	}
	int time_holder [s];
	for(i =0; i < s; i++)
	{
		int temp;
		struct stat buffer;
		temp = lstat(relevant_dir[i],&buffer);	
		if(temp == -1)
		{
			printf("somethign wrong lstat");
			exit(1);
		}
		time_t unchanged_time = buffer.st_mtime;
		int pretty = unchanged_time;
		time_holder[i] = pretty;
	}
	for(i =0; i < s; i++)
	{
	}
	int most_recent_index= 0;
	for(i = 1; i <s; i++)
	{
		if(time_holder[most_recent_index] < time_holder[i])
		{
			most_recent_index = i;
		}	
	}
	int length = strlen(relevant_dir[most_recent_index]);
	char * temptr ;
	temptr= (char *)malloc(length*sizeof(char));
	strcpy(temptr,relevant_dir[most_recent_index]);
	return temptr;	
}
void play_game()
{
	int i,q;
	struct Room holder[7];
	char file_paths [7][200];
	char * dir=find_recent_dir();
	find_room_files(file_paths,dir);
	char room_name[]="ROOM NAME:";
	char room_connection[]="CONNECTION ";
	char room_type[] ="ROOM TYPE: ";
	char * buffer;
	char * blue;
	for(i =0; i<7; i++)
	{
		FILE * fp;
		size_t buffsize = 80;
		fp = fopen(file_paths[i],"r");
		int c = 0;
		while(!feof(fp))
		{
			getline(&buffer,&buffsize,fp);
			if(strstr(buffer,room_name) != NULL)
			{
				char * temp = buffer+11;
				char * green=(char*)malloc(50*sizeof(char));
				strcpy(green,temp);
				holder[i].name = green;
				green = NULL;
			}
			else if(strstr(buffer,room_connection)!= NULL)
			{
				char * temp=(char*)malloc(50*sizeof(char));
				strcpy(temp, buffer+14);
				holder[i].connections[c]=temp;
				c++;
				temp = NULL;
			}
			else if(feof(fp) == 1)
			{
				break;
			}

			else if(strstr(buffer,room_type) != NULL)
			{
				char * temp = buffer+11;
				blue=(char*)malloc(25*sizeof(char));
				strcpy(blue, temp);
				holder[i].room_type=blue;
				blue = NULL;
			}

		}
		holder[i].num_connections=c;
		fclose(fp);
	}
	free(buffer);

	int c;	
	int current;
	char * start = "START_ROOM\n";
	char * end = "END_ROOM\n";
	char victory[100][100];
	int steps=0;
	int start_index, end_index;
	for(i =0; i < 7; i++)
	{
		if(strcmp(holder[i].room_type, start)== 0)
		{
			start_index = i;
		}
		else if(strcmp(holder[i].room_type, end)== 0)
		{
			end_index = i;
		}
	}	
	current = start_index;
	while(current != end_index)
	{
		printf("CURRENT LOCATION: %s",holder[current].name);
		printf("POSSIBLE CONNECTIONS: ");
		for(i=0;i<holder[current].num_connections - 1;i++)
		{
			char temp [15];
			strcpy(temp, holder[current].connections[i]);
			temp[strlen(holder[current].connections[i])-1]='\0';
			printf("%s, ",temp);
		}
		char temp [15];
		int num = holder[current].num_connections - 1;
		strcpy(temp, holder[current].connections[num]);
		temp[strlen(holder[current].connections[num])-1]='\0';
		printf("%s.\n",temp);
		printf("WHERE TO? >");
		current = get_input(holder,victory,current,&steps);
	}
	printf("\nYOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n");
	printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n",steps);
	for(i = 0; i < steps-1; i++)
	{
		printf("%s",victory[i]);
	}
	printf("%s",holder[current].name);
	free(dir);
	for(i = 0; i< 7; i++)
	{
		for(c = 0; c<holder[i].num_connections; c++)
		{
			free(holder[i].connections[c]);
		}	
		free(holder[i].room_type);
		free(holder[i].name);
	}
}

int main(void)
{
		play_game();
}
