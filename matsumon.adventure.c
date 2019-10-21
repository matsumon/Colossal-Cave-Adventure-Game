#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
struct Room 
{
	char * name;
	char * connections [6];
	int num_connections;
	char * room_type;
};

void find_room_files (char file_paths[7][200],char * dir_path)
{
	DIR * dir;
	int i =0;
	int q;
	FILE * fp;
	struct dirent *entry;
	char * room_files [7];
//	char * dir_path = "matsumon.rooms.19860";
//	char file_paths [7][200];
	dir = opendir(dir_path);
	if(dir == NULL)
	{
		printf("couldnt open anythingi");
		exit(1);
	}
	while(entry=readdir(dir))
	{
		//printf("%d\n",entry->d_type);
		if(entry->d_type == 8)
		{
			room_files[i] = entry->d_name;
			i++;
		}
		//	printf("%s\n", entry->d_name);
	}
	for(i = 0; i < 7; i++)
	{
		//	printf("%s\n", room_files[i]);
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
	//	printf("%s\n",folder_path);
		int temp = strlen(dir_path)+ 1 + strlen(room_files[q]);
		for(i = 0; i < temp; i++)
		{
			file_paths[q][i]=folder_path[i];
		}
		file_paths[q][temp]='\0';
	}
/*	for(i = 0; i < 7; i++)
	{
		for(q=0; q < 200; q++)
		{
			if(file_paths[i][q] == '\0')
			{
				break;
			}
			printf("%c",file_paths[i][q]);
		}
		printf("\n");
	}*/

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
	for(i = 0; i < t; i++)
	{
//		printf("%s\n",dir_holder[i]);
	}
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
//	printf("\n");
	for(i = 0; i < s; i++)
	{
		printf("%s\n",relevant_dir[i]);
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
		printf("%d\n",time_holder[i]);
	}
	int most_recent_index= 0;
	for(i = 1; i <s; i++)
	{
		if(time_holder[most_recent_index] < time_holder[i])
		{
		 most_recent_index = i;
		}	
	}
	printf("%d\n",most_recent_index);
	return(relevant_dir[most_recent_index]);
}

int main()
{
	int i,q;
	char file_paths [7][200];
	char * dir=find_recent_dir();
	find_room_files(file_paths,dir);
	for(i = 0; i < 7; i++)
	{
		for(q=0; q < 200; q++)
		{
			if(file_paths[i][q] == '\0')
			{
				break;
			}
				printf("%c",file_paths[i][q]);
		}
			printf("\n");

	}
}
