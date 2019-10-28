#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

//Room structure to hold file fields
struct Room 
{
	char * name;
	char * connections [6];
	int num_connections;
	char * room_type;
};

//input needs a room pointer array
//output makes the room files
void makeFiles(struct Room * holder [])
{
	int i,c,d,t;
//grabs the PID of the current process
	int pid = getpid();
	char buffer [100];
//appends the pid to the desired directory name
	sprintf(buffer,"matsumon.rooms.%d",pid);	
//makes the directory and sets the permisions
	mkdir(buffer,0777);
	char file [200];
//should have use memset to set everything to null
	for(i=0; i<200; i++)
	{
		file[i]='\0';
	}
//copies directory name to begninning of file path
	for(i=0; i<20; i++)
	{
		file[i]=buffer[i];	
	}
	t=0;
	for(i=0; i < 200; i++)
	{
		if(file[i] != '\0')
		{
			t++;	
		}	
		else
		{
			break;
		}
	}
	file[t]='/';
	t++;
	FILE * fp;
	for(c = 0; c < 7; c++)
	{
//creats a room file after getting the file path into the right syntax
		char source[100]="";
		char temp[100]="";
		memcpy(source, holder[c]->name, strlen(holder[c]->name));
		memcpy(temp, file, strlen(file));
		for(i=t; i<t+strlen(holder[c]->name); i++)
		{
			temp[i] = source[i-t];	
		}
		fp=fopen(temp,"w");
		chmod(temp,0777);
		if(fp == NULL)
		{
			printf("file messed up\n");
			exit(1);
		}
//sets the connection and room type fileds
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

//input needs a nonempty struct room array
//output return a 0 on true and a 1 on false
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

//input needs a room struct array, and two ints representing the indcies to be connected
//outputs a one for a duplicate and 0 for nonduplicates
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

//input a struct room array and two indices to be connected
void addConnect(struct Room * holder [],int connect_one, int connect_two)
{
//check for duplicates
	int check = check_for_dup(holder,connect_one,connect_two);
	if(check == 1)
	{
		return;
	}
//checks to make sure both connections are not greater than 6
	if(holder[connect_one]->num_connections == 6)
	{
		return;
	}
	if(holder[connect_two]->num_connections == 6)
	{
		return;
	}
//connects both rooms
	holder[connect_one]->connections[holder[connect_one]->num_connections]=
		holder[connect_two]->name;
	holder[connect_two]->connections[holder[connect_two]->num_connections]=
		holder[connect_one]->name;
	holder[connect_one]->num_connections = holder[connect_one]->num_connections + 1;
	holder[connect_two]->num_connections = holder[connect_two]->num_connections + 1;
	
}

//input a struct room holder 
// assigns random connections to allthe rooms
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

//input needs a struct room holder
//assigns room types to the rooms based on random numbers
//this function could be improved by simply swapping the incdices and then make the first two start and end
//and the rest mid type rooms
void assign_room_type (struct Room * holder [7])
{
	int i, rand_num, rand_num_two, check;
	rand_num = rand()%7;
//assign start room type
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
//assign end room type
			holder[rand_num_two]->room_type = "END_ROOM";
		}
	}
//assign the rest of the roomtypes as mid
	for (i = 0; i < 7; i++)
	{
		if(i != rand_num && i != rand_num_two )
		{
			holder[i]->room_type = "MID_ROOM";
		}
	}
}

//input needs a char * array, struct room array , and an int
//returns a room *
struct Room * assign_Name (char * random_names[],struct Room * holder [7], int k)
{
	int i,c,rand_num;
//put room on the heap so it doesn disappear
	struct Room * temp = (struct Room *) malloc(sizeof(struct Room));	
	int check = 0;
	while(check == 0)
	{
		rand_num = rand()%10;
		check = 1;
		for(i=0; i<k; i++)
		{
//assigns a struct room name based on a random var
			if(holder[i]->name == random_names[rand_num])
			{
				check = 0;
			}
		}
	}
	temp->name=random_names[rand_num];
//returns the  struct room
	return temp;
}

int main()
{
//seeding time for 0 at the start so numbers are random as possible
	srand(time(0));
	int i,c;
	char * random_names [10];
//statically allocating room names
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
//assigns 7 names randomly
	for(i=0;i<7;i++)
	{
		holder[i] = assign_Name(random_names,holder,i);
	}
	assign_room_type(holder);
//assign room types randomly
	assign_connections(holder);
//assign connections randomly
	makeFiles(holder);
//deallocate malloced memory
	for(i=0;i<7;i++)
	{
		free(holder[i]);
	}
	return 0;
}
