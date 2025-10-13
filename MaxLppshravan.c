#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main()
{
int MaxCoefficient[1][3]={10,7,-5};
char *Variable[3][1]={{"x"},{"y"},{"Z"}};
char *MaxZ= (char *)malloc(100);
for(int FirstArrRow=0;FirstArrRow < 1;FirstArrRow++)
  for(int SecondArrCol=0;SecondArrCol < 1;SecondArrCol++)
   for(int SecondArrRow=0;SecondArrRow < 3;SecondArrRow++)
	{
	char intToChar;
	if(MaxCoefficient[FirstArrRow][SecondArrRow]>0)
	   	strcat(MaxZ,"+");

	sprintf(&intToChar,"%d",MaxCoefficient[FirstArrRow][SecondArrRow]);
	strcat(MaxZ,&intToChar);
	//printf("%s\n",MaxZ);
	//printf("%s\n",Variable[SecondArrRow][SecondArrCol]);
	sprintf(&intToChar,"%s",Variable[SecondArrRow][SecondArrCol]);
	strcat(MaxZ,&intToChar);
	//printf("%s\n",MaxZ);
	}
printf("%s\n",MaxZ);

}
