#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<time.h>

struct Network
{
	int **iarray;	
	int nodes;
	int *degrees;	
};
int **CreateIArray(int ,int );
int PrintArray(int **,int ,int,FILE * );
int createRegularNetwork(struct Network *,int);
int createfile(char *,int **,int ) ;
double randProb();

int main()
{
	int idegree;
	struct Network *regNetwork=(struct Network *)calloc(1,sizeof(struct Network));
	
	printf("Please enter the number of nodes in network:");
	scanf("%d",&regNetwork->nodes);
	printf("Please enter degree of a Regular network node:");
	scanf("%d",&idegree);
	if(idegree>regNetwork->nodes-1)
	printf("degree of a node cannot exceed total nodes-1");
	
	regNetwork->iarray=CreateIArray(regNetwork->nodes,regNetwork->nodes);	
	regNetwork->degrees=(int *)calloc(sizeof(int),regNetwork->nodes);
	srand( time(0));
	createRegularNetwork(regNetwork,idegree);
	char filename[100]="RegularNetwork.txt";
	FILE *fp = fopen(filename, "w+");
	//time_t t;
	//struct tm a;
	//t=time();
	
	//createfile(filename,regNetwork->iarray,regNetwork->nodes);	
	PrintArray(regNetwork->iarray, regNetwork->nodes,regNetwork->nodes,fp);
		
	getchar();
	free(regNetwork);
}
int createRegularNetwork(struct Network *regNetwork,int idegree)
 { 	
 	int randomnode=0;
 	for(int ArrRowTemp=0;ArrRowTemp < regNetwork->nodes;ArrRowTemp++)
	{
		int k=idegree-regNetwork->degrees[ArrRowTemp];
	  for(int ArrColTemp=0;ArrColTemp < k;ArrColTemp++)
		{
			randomnode=rand()%regNetwork->nodes;
			if( randomnode==ArrRowTemp || regNetwork->iarray[ArrRowTemp][randomnode]==1)// no selfloop and no previous connection should be repeated
			{ArrColTemp--;continue;
			}
			if(regNetwork->degrees[randomnode]<idegree ) //since it is an undirected network we need to check the other node degree aswell
			{
				regNetwork->iarray[ArrRowTemp][randomnode]=1;		
				regNetwork->iarray[randomnode][ArrRowTemp] =1;//symmetric matrix for undirected network
				regNetwork->degrees[ArrRowTemp]++;
				regNetwork->degrees[randomnode]++;				
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

int PrintArray(int **Array,int ArrRow,int ArrCol,FILE *fp)
{
	
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  for(int ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
		fprintf(fp,"%d\t",Array[ArrRowTemp][ArrColTemp]);
		}
	  fprintf(fp,"\n");
	}
fprintf(fp,"\n");
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
