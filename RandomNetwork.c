#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

int **CreateIArray(int ,int );
int PrintArray(float **,int ,int );
int createRandomNetwork(int **,int,float);
int createfile(char *,int **,int ) ;
double randProb();

int main()
{
	int nodes;
	float iedgeprob;
	printf("Please enter the number of nodes in network:");
	scanf("%d",&nodes);
	printf("Please enter probability of an edge between two nodes:");
	scanf("%f",&iedgeprob);
	int **iarray=CreateIArray(nodes,nodes);
	srand( time(0));
	createRandomNetwork(iarray,nodes,iedgeprob);
	char filename[100]="RandomNetwork.txt";

	//time_t t;
	//struct tm a;
	//t=time();
	
	createfile(filename,iarray,nodes);
	
	//PrintArray(testmat->farray, matRow, matCol);
		
	getchar();
	free(iarray);
}
 int createRandomNetwork(int **iarray,int nodes,float iedgeprob)
 {
 	
 	for(int ArrRowTemp=0;ArrRowTemp < nodes;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < nodes;ArrColTemp++)
		{
			
			if(randProb()<=iedgeprob && ArrRowTemp!=ArrColTemp)
			{
				iarray[ArrColTemp][ArrRowTemp] =1;
				iarray[ArrRowTemp][ArrColTemp]=1;
			}
		}	  
	}
 	
 }

int createfile(char *filename,int **iarray,int nodes) 
{
	FILE *fp = fopen(filename, "w+");
	char *rowoffile=(char *)calloc(sizeof(char),nodes*2+2);
	for(int ArrRowTemp=0;ArrRowTemp < nodes;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < nodes;ArrColTemp++)
		{			
			char s[3];
			strcpy(s,(iarray[ArrRowTemp][ArrColTemp]==1?"1":"0"));
			strcat(rowoffile,strcat(s,"\t"));
		}
		strcat(rowoffile,"\n");	  
		fputs(rowoffile,fp);
		for(int initialiser=0;initialiser<nodes*2+2;initialiser++)
		rowoffile[initialiser] = '\0'; 
	}
	fclose(fp);
	free(rowoffile);
}

double randProb()
{
	
    return (double)rand() / (double)RAND_MAX ;
}
int PrintArray(float **Array,int ArrRow,int ArrCol)
{
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
		printf("%f ",Array[ArrRowTemp][ArrColTemp]);
		}
	  printf("\n");
	}
printf("\n");
}
// can be used a bit array
int **CreateIArray(int ArrRow,int ArrCol)
{
	int **Array=(int **)calloc(sizeof(int*),ArrRow);
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
		Array[ArrRowTemp]=(int *)calloc(sizeof(int),ArrCol);
	}
	return Array;
}
