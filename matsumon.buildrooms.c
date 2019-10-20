#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>


struct Room 
{
	char * name;
	char * connections [6];
	int num_connections;
	char * room_type;
};

void makeFiles(struct Room * holder [])
{
	int i,c,d;
	int pid = getpid();
	char buffer [100];
	sprintf(buffer,"matsumon.rooms.%d",pid);	
	mkdir(buffer,0700);
	char file [200];
	for(i=0; i<20; i++)
	{
		file[i]=buffer[i];	
	}
	file[20]='/';
	FILE * fp;
	for(c = 0; c < 7; c++)
	{
		char source[100]="";
		char temp[100]="";
		memcpy(source, holder[c]->name, strlen(holder[c]->name));
		memcpy(temp, file, strlen(file));
		for(i=21; i<21+strlen(holder[c]->name); i++)
		{
			temp[i] = source[i-21];	
		}
		printf("%s\n",temp);
		fp=fopen(temp,"w");
		if(fp == NULL)
		{
			printf("file messed up");
			exit(1);
		}
		fprintf(fp,"ROOM NAME: %s\n",holder[c]->name);
		for(d=0; d<holder[c]->num_connections; d++)
		{
			fprintf(fp,"CONNECTION %d: %s\n",d+1,
			holder[c]->connections[d]);
		}
		fprintf(fp,"ROOM TYPE: %s\n", holder[c]->room_type);
		fclose(fp);
	}
}

int isConnected (struct Room * holder [])
{
	int i;
	for( i = 0; i<7; i++)
	{
		if(holder[i]->num_connections < 3)
		{
			return 0;
		}
	}
	return 1;
}
int check_for_dup(struct Room * holder [],int connect_one, int connect_two)
{
	int i ,c;
	for(i=0; i<holder[connect_one]->num_connections; i++)
	{
			if(holder[connect_one]->connections[i] == holder[connect_two]->name)
			{
				return 1;
			}
	}
	return 0;
}
void addConnect(struct Room * holder [],int connect_one, int connect_two)
{
	int check = check_for_dup(holder,connect_one,connect_two);
	if(check == 1)
	{
		return;
	}
	if(holder[connect_one]->num_connections == 6)
	{
		return;
	}
	if(holder[connect_two]->num_connections == 6)
	{
		return;
	}
	holder[connect_one]->connections[holder[connect_one]->num_connections]=
		holder[connect_two]->name;
	holder[connect_two]->connections[holder[connect_two]->num_connections]=
		holder[connect_one]->name;
	holder[connect_one]->num_connections = holder[connect_one]->num_connections + 1;
	holder[connect_two]->num_connections = holder[connect_two]->num_connections + 1;
	
}
void assign_connections(struct Room * holder [])
{
	int connect_one, connect_two;
	while(isConnected(holder) == 0)
	{
		connect_one = rand()%7;
		connect_two = rand()%7;	
		while(connect_one == connect_two)
		{
			connect_two = rand()%7;
		}
		addConnect(holder,connect_one,connect_two);
	}

}
void assign_room_type (struct Room * holder [7])
{
	int i, rand_num, rand_num_two, check;
	rand_num = rand()%7;
	holder[rand_num]->room_type="START_ROOM";
	rand_num_two = rand()%7;
	check = 0;
	while(check == 0)
	{
		check = 1;
		if (rand_num == rand_num_two)
		{
			check = 0;
			rand_num_two = rand()%7;
		}
		else 
		{
			holder[rand_num_two]->room_type = "END_ROOM";
		}
	}
	for (i = 0; i < 7; i++)
	{
		if(i != rand_num && i != rand_num_two )
		{
			holder[i]->room_type = "MID_ROOM";
		}
	}
}

struct Room * assign_Name (char * random_names[],struct Room * holder [7], int k)
{
	int i,c,rand_num;
	struct Room * temp = (struct Room *) malloc(sizeof(struct Room));	
	int check = 0;
	while(check == 0)
	{
		rand_num = rand()%10;
		check = 1;
		for(i=0; i<k; i++)
		{
			if(holder[i]->name == random_names[rand_num])
			{
				check = 0;
			}
		}
	}
	temp->name=random_names[rand_num];
	return temp;
}

int main()
{
	srand(time(0));
	int i,c;
	char * random_names [10];
	random_names[0] = "one";
	random_names[1] = "two";
	random_names[2] ="three";
	random_names[3] ="four";
	random_names[4] ="five";
	random_names[5] ="six";
	random_names[6] ="seven";
	random_names[7] ="eight";
	random_names[8] ="nine";
	random_names[9] ="ten";
	struct Room * holder[7];
	for(i=0;i<7;i++)
	{
		holder[i] = assign_Name(random_names,holder,i);
		printf("%s\n",holder[i]->name);
	}
	assign_room_type(holder);
	assign_connections(holder);
	for(i=0;i<7;i++)
	{
		printf("%d",i);
		for(c=0; c<holder[i]->num_connections; c++)
		{
			printf("%s\n",holder[i]->connections[c]);
		}
	}
	makeFiles(holder);

	for(i=0;i<7;i++)
	{
		free(holder[i]);
	}
	return 0;
}
