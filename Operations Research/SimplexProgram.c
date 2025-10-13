#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define FIXITERATION 12
#define MVALUE 50000
float **EnterMatrix();
int PrintArray(float **,int ,int );
int PrintArray1D(float *,int );
int **TransposeMatix(int **,int ,int );
int FindMinArray(float *,int );
int checkZcoffRate(float * );
int SimplexIteration(float **,int , int ,int );
int optimumsolution(float **,int ,int );
int PrintColumnsNames(int );
float **CreateFArray(int ,int );
float ** setZcofftoproperform(float **);
int Infeasibilitytest(float **);

int NoOfVariables,totalColumns,totalrows,NoOfEquation,PivotCol,nolessthaneq,noequaltoeq,nogreaterthaneq,typeofobfun,artfvarArr[100];

int main()
{
float **IterationArray;
int feasibilitystatus;
printf("For maximising objective function press 1\nFor minimising objective function press 2\n");
scanf("%d",&typeofobfun);
printf("enter the number of 'greater than' type of equation:");
scanf("%d",&nogreaterthaneq);
printf("enter the number of 'equal to' type of equation:");
scanf("%d",&noequaltoeq);
printf("enter the number of 'less than' type of equation:");
scanf("%d",&nolessthaneq);

printf("enter the number of variables in equation");
scanf("%d",&NoOfVariables);

totalColumns=NoOfVariables+2*nogreaterthaneq+noequaltoeq+nolessthaneq+1+1+1+1;// one for ratio,one for basis variable,one for objective function(Z) coff,1 for right side of of equations
NoOfEquation=nogreaterthaneq+noequaltoeq+nolessthaneq+1;// one extra for objective funtion
totalrows=(NoOfEquation+1)*FIXITERATION; //one extra for scrap work
// later on this string can be transmitted in the entermatrix function and can be printed whereever necessary inside that function												//noofequation is for slack variable numbers and no of variables for the variable coff
printf("enter the coefficients of objective function and equations in this form :");
printf("\n3 4 0\n3 0 4 5\n2 3 0 4\n for eqation of form 3x+4y+0z(objective function)\n3x+0y+4z<=5\n2x 3y 0z>=4\n");

//assuming that number of max function coeff are same as number of coef of ...
IterationArray=EnterMatrix();//startcol is 2 as we have leave one for basis and one for z's coff and ending is total- ratio col
//PrintArray(IterationArray, NoOfEquation, totalColumns);
setZcofftoproperform(IterationArray);
int iterationcounter;
for(iterationcounter=0;iterationcounter<FIXITERATION;iterationcounter++)
{
	int newItRarray=(iterationcounter)*(NoOfEquation+1);// for sending the arrary from where zrow starts for the subsequent iterations
	if((PivotCol=checkZcoffRate(IterationArray[newItRarray]))>=2)// check if z coffs are still less than zero and returns the pivotcol(pivot column number is absolute to total columns in table, not relative.) 
		{
		feasibilitystatus=SimplexIteration(IterationArray+newItRarray,totalColumns, PivotCol,NoOfEquation);
		if(feasibilitystatus==-1)
		printf("\nSolution is unbounded!\n");
		}
	else
		{	
			//PrintArray(IterationArray,2*newItRarray,totalColumns);
			feasibilitystatus=Infeasibilitytest(IterationArray+newItRarray);
			if(feasibilitystatus==-1)
				printf("Infeasible solution exists to this problem!\n");// the infeasible solution when basis has artificial variable and it is greater than 0
			else
			{		
				printf("Optimised solution is obtained\n");// for now but still need to consider the infeasible and unbounded solution
				optimumsolution(IterationArray+newItRarray,NoOfEquation,NoOfVariables);				
			}
			break;
		}
}
free(IterationArray);
}

int Infeasibilitytest(float **IterationArray)
{
	for(int temp=0;temp<NoOfEquation-1;temp++)
		{
			int temp1=0;
			while(artfvarArr[temp1]>0)
			{
				if(artfvarArr[temp1]==IterationArray[temp+1][0] && IterationArray[temp+1][totalColumns-2]>0)
				{	return -1;
				}
				temp1++;
			}
		}
	return 1;
}
int optimumsolution(float **IterationArray,int NoOfEquation,int NoOfVariables)
{
	printf("The optimum solution is %f\n",IterationArray[0][NoOfVariables+NoOfEquation+1]);
	for (int row=1;row<NoOfEquation;row++)
	{
		if(IterationArray[row][0]<=NoOfVariables)
		{
			printf("For X%d=",(int)IterationArray[row][0]);
			printf("%f\n\n",IterationArray[row][NoOfVariables+NoOfEquation+1]);
		}
	}
}

// to find the rate of z coff and return the pivot col that increases the z mostly..means the most negative element
// or retun code to specify that optimisation has reached, that is all are positive or zero.
// basisvariable|zcoff|var1|var2|....|RightsideOfEquation|ratio is the column structure of table
int checkZcoffRate(float *IterationArray)
{
	//PrintArray1D(IterationArray, noofcol);
	int minelement=FindMinArray(IterationArray+2,totalColumns-5);// to send the section of array that contains only the z's(objective function) coff
	printf("min element %f\n ",IterationArray[minelement+2]);
	if (IterationArray[minelement+2]<0)// means element is negative and still need to optimise
		return minelement+2;
	else
		return -1;
}

int SimplexIteration(float **IterationArray,int totalColumns, int PivotCol,int equationrows)
{	
	// for finding the minimum ratio
	int minrow=-1;
	//printf("%d",PivotCol);
	for (int row=1;row<equationrows;row++)
	{
		if(IterationArray[row][totalColumns-2]==0 || IterationArray[row][PivotCol]==0)
			continue;
		else
		{
			// saving min ratio in the last column
			IterationArray[row][totalColumns-1]=IterationArray[row][totalColumns-2]/IterationArray[row][PivotCol];
			if(IterationArray[row][totalColumns-1]<=0) continue;
			else if (minrow==-1) minrow=row;
			else if(IterationArray[row][totalColumns-1]<IterationArray[minrow][totalColumns-1] )
				minrow=row;		
				
		}
	}
	PrintArray(IterationArray,equationrows,totalColumns); // for printing the first array
	if(minrow==-1){
	 printf("returning as minrow is -1 which shouldn't be");return -1;
	}
	printf("Entering variable is X%d  ",PivotCol-1);
	printf("Leaving variable is X%d  \n",(int)IterationArray[minrow][0]);
	
	// for copying the leaving variable coff
	float *leavingbasis=(float *)calloc(equationrows,sizeof(float));
	for (int row=1;row<equationrows;row++)
	{
		leavingbasis[row]=IterationArray[row][(int)IterationArray[minrow][0]+1];
	}
	//PrintArray1D(leavingbasis,equationrows);
	
	// for creating next iteration table; copy everything except ratio column
	for (int row=0;row<equationrows;row++)
	for(int col=0;col<totalColumns-1;col++)
	{
		if(row==minrow)// pivot row will be copied by dividing it by pivot col to make pivot col value 1
			IterationArray[equationrows+1+row][col]=IterationArray[row][col]/IterationArray[row][PivotCol];
		else 
			IterationArray[equationrows+1+row][col]=IterationArray[row][col];
	}
//		printf("\npivotcol %d\n",PivotCol);
		IterationArray[equationrows+1+minrow][0]=PivotCol-2+1;// for copying new entering basis variable in relative terms x1,x2...
//	PrintArray(IterationArray,2*equationrows+1,totalColumns);
// for making entering variable coffs equal to  leaving variable coff
IterationArray=IterationArray+equationrows+1; // to begin the new array for new iteration
	for (int row=0;row<equationrows;row++)
	{
		if(leavingbasis[row]==0)
		{
			if(IterationArray[row][PivotCol]==0)continue;
			else
				{
					float colratio=IterationArray[row][PivotCol]/IterationArray[minrow][PivotCol];						
					for(int col=2;col<totalColumns-1;col++)
					{
						IterationArray[row][col]=IterationArray[row][col]-colratio*IterationArray[minrow][col];
						//if(col==PivotCol)
						//IterationArray[row][col]=0;// in case float is not making it zero!!
					}
				}
		}
	}
		PrintArray(IterationArray,equationrows,totalColumns); // for printing the second array
}
// to remove M from basis variables in z's coffs 
float ** setZcofftoproperform(float **IterationArray)
{
	int tcounter=0;
	while(artfvarArr[tcounter]>0)
	{						
		for(int col=2;col<totalColumns-1;col++)
		{
			IterationArray[0][col]=IterationArray[0][col] - MVALUE*IterationArray[tcounter+1][col];			
		}		
		tcounter++;
	}	
}
// returns the index of mininum element
int FindMinArray(float *array,int totalelement)
{
int minelement=0;
	int i;
	for(i=0;i<totalelement;i++)
	{
		if(array[minelement]>array[i+1])
		 minelement=i+1;	
	}
	return minelement;
}
int PrintArray(float **Array,int ArrRow,int ArrCol)
{
	int ArrRowTemp;
	printf("\n");
	PrintColumnsNames(ArrCol);
	for(ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
	  int ArrColTemp;
	  for(ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
			if(Array[ArrRowTemp][ArrColTemp]<0)
				printf("%011.3f  ",Array[ArrRowTemp][ArrColTemp]);
			else if (ArrColTemp==0||ArrColTemp==1)
				printf("%d          ",(int)Array[ArrRowTemp][ArrColTemp]);
			else
				printf("%011.3f  ",Array[ArrRowTemp][ArrColTemp]);
		}
	  printf("\n");
	}
	printf("\n");
}

int PrintColumnsNames(int cols)
{
 printf("Basis Var  Z's Coff    ");
 for(int ArrColTemp=1;ArrColTemp < cols-3;ArrColTemp++)
	{
	 printf("X%d           ",ArrColTemp);
	}	
 printf("Right Side    Ratio\n");
}

int PrintArray1D(float *Array,int ArrCol)
{
	int ArrColTemp;
	  for(ArrColTemp=0;ArrColTemp < ArrCol;ArrColTemp++)
		{
		printf("%f ",Array[ArrColTemp]);
		}	  
	
printf("\n");
}
//receives the size of matrix, creates and tells where to start entering the data in matrix.
float **EnterMatrix()
{	
	//printf("%d",ArrCol);
	float **Array=CreateFArray(totalrows,totalColumns);	
	
	printf("Enter objective funtion\n");
	int temprow=0;
	Array[temprow][1]=1;
	for(int ArrColTemp=2;ArrColTemp < NoOfVariables+2;ArrColTemp++)
	{
		scanf("%f",&Array[temprow][ArrColTemp]);
		if(typeofobfun==1)// for maximising
		Array[temprow][ArrColTemp]=(-1)*Array[temprow][ArrColTemp];
	}
	scanf("%f",&Array[temprow][totalColumns-1]);	
	int artificialvar=NoOfVariables+1;
	for(int ArrColTemp=1;ArrColTemp < nogreaterthaneq+1;ArrColTemp++)
	{
		artificialvar=artificialvar+ArrColTemp*2;
		Array[temprow][artificialvar]=MVALUE; // some max huge value for M							
	}
	for(int ArrColTemp=1;ArrColTemp < noequaltoeq+1;ArrColTemp++)
	{
		artificialvar=artificialvar+ArrColTemp;
		Array[temprow][artificialvar]=MVALUE; // some max huge value for M							
	}
	// for greater than equations
	artificialvar=0;
	temprow++;
	int tempbasis=NoOfVariables; 
	int countartfvar=0;
	if(nogreaterthaneq>0)
	{
		// +2 for keeping surplus first then artificial
		for(temprow;temprow<nogreaterthaneq+1;temprow++)
		{		
			printf("Enter greater than equation\n");
			tempbasis=tempbasis+2;
			artfvarArr[countartfvar]=tempbasis;
			Array[temprow][0]=tempbasis;
			for(int ArrColTemp=2;ArrColTemp < NoOfVariables+2;ArrColTemp++)
			{
				scanf("%f",&Array[temprow][ArrColTemp]);							
			}
			scanf("%f",&Array[temprow][totalColumns-2]);	
			artificialvar=NoOfVariables+temprow*2+1;
			Array[temprow][artificialvar-1]=-1; // some max huge value for M		
			Array[temprow][artificialvar]=1; // some max huge value for M	
			countartfvar++;
		}
	}
	// for equalto equations	
	if(noequaltoeq>0)	
	{	
		int equaltorow=temprow;
		for(temprow;temprow<noequaltoeq+equaltorow;temprow++)
		{		
			printf("Enter equal to equation\n");
			tempbasis=tempbasis+1; // +1 for artificial
			artfvarArr[countartfvar]=tempbasis;
			Array[temprow][0]=tempbasis;
			for(int ArrColTemp=2;ArrColTemp < NoOfVariables+2;ArrColTemp++)
			{
				scanf("%f",&Array[temprow][ArrColTemp]);							
			}
			scanf("%f",&Array[temprow][totalColumns-2]);	
			if(artificialvar==0)
				artificialvar=NoOfVariables+temprow+1;
			else
				artificialvar=artificialvar+1;			
		
			Array[temprow][artificialvar]=1; 
			countartfvar++;
		}
	}

	// for less than equations	
	if(nolessthaneq>0)
	{			
		int lessthanrow=temprow;
		int slackvar=artificialvar;
		for(temprow;temprow<nolessthaneq+lessthanrow;temprow++)
		{		
			printf("Enter less than equation\n");
			tempbasis=tempbasis+1; // +1 for slack
			Array[temprow][0]=tempbasis;
			for(int ArrColTemp=2;ArrColTemp < NoOfVariables+2;ArrColTemp++)
			{
				scanf("%f",&Array[temprow][ArrColTemp]);							
			}
			scanf("%f",&Array[temprow][totalColumns-2]);	
		
			if(artificialvar==0)
			{
				slackvar=NoOfVariables+temprow;
				artificialvar=slackvar;
			}
			else
				slackvar=slackvar+1;			
		
			Array[temprow][slackvar]=1; 
		}
	}
	return Array;
}

float **CreateFArray(int ArrRow,int ArrCol)
{
	float **Array=(float **)calloc(sizeof(float*),ArrRow);
	for(int ArrRowTemp=0;ArrRowTemp < ArrRow;ArrRowTemp++)
	{
		Array[ArrRowTemp]=(float *)calloc(sizeof(float),ArrCol);
	}
	return Array;
}

